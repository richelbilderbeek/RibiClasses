//---------------------------------------------------------------------------
/*
QtConceptMap, Qt classes for display and interaction with ConceptMap
Copyright (C) 2013-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppQtConceptMap.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtconceptmap.h"

#include <iostream>
#include <set>

#include <boost/bind.hpp>
#include <boost/graph/isomorphism.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include <QApplication>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QDebug>

#include "fuzzy_equal_to.h"
#include "qtconceptmapcollect.h"

#include "container.h"
#include "conceptmapconceptfactory.h"
#include "conceptmapconcept.h"
#include "conceptmapfactory.h"
#include "conceptmap.h"
#include "conceptmapedgefactory.h"
#include "count_edges_with_selectedness.h"
#include "conceptmapedge.h"
#include "conceptmaphelper.h"
#include "conceptmapnodefactory.h"
#include "conceptmapnode.h"
#include "qtconceptmapcommanddeleteselected.h"
#include "qtarrowitem.h"
#include "qtconceptmapdisplaystrategy.h"
#include "qtconceptmapbrushfactory.h"
#include "qtconceptmapcommandcreatenewedge.h"
#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmapcommandselectrandomnode.h"
#include "qtconceptmapcenternode.h"
#include "qtconceptmapconcepteditdialog.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmapexamplesitem.h"
#include "qtconceptmapitemhighlighter.h"
#include "qtconceptmapnewarrow.h"
#include "count_vertices_with_selectedness.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaptoolsitem.h"
#include "qtquadbezierarrowitem.h"
#include "qtscopeddisable.h"
#include "trace.h"
#include "xml.h"

#pragma GCC diagnostic pop

///Returns a sorted vector
template <class T>
std::vector<T> Sort(const std::vector<T>& v)
{
  std::vector<T> w(v);
  std::sort(w.begin(),w.end());
  return w;
}

template <>
std::vector<ribi::cmap::QtNode*>
  Sort(
    const std::vector<ribi::cmap::QtNode*>& v)
{
  typedef std::vector<ribi::cmap::QtNode*>::iterator Iterator;
  std::vector<ribi::cmap::QtNode*> w(v);
  std::sort(w.begin(),w.end());
  const Iterator i = std::find_if(w.begin(),w.end(),
    [](const ribi::cmap::QtNode* const node)
    {
      return dynamic_cast<const ribi::cmap::QtCenterNode*>(node);
    }
  );
  if (i != w.end())
  {
    std::swap(*i,*w.begin());
    assert(dynamic_cast<const ribi::cmap::QtCenterNode*>(*w.begin()));
  }
  return w;
}

ribi::cmap::QtConceptMap::QtConceptMap(QWidget* parent)
  : QtKeyboardFriendlyGraphicsView(parent),
    m_arrow{new QtNewArrow},
    m_conceptmap{},
    m_examples_item(new QtExamplesItem),
    m_highlighter{new QtItemHighlighter},
    m_tools{new QtTool}
{
  #ifndef NDEBUG
  this->SetVerbosity(false);
  #endif
  this->setScene(new QGraphicsScene(this));
  assert(!m_highlighter->GetItem());

  //Add QtNewArrow
  assert(!m_arrow->scene());
  scene()->addItem(m_arrow); //Add the QtNewArrow so it has a parent
  m_arrow->hide();
  assert(!m_arrow->isVisible());

  //Add QtExamplesItem
  assert(!m_examples_item->scene());
  scene()->addItem(m_examples_item); //Add the examples so it has a parent

  //Add QtTool
  assert(!m_tools->scene());
  this->scene()->addItem(m_tools);

  assert(Collect<QtNode>(scene()).empty());

  //Without this line, mouseMoveEvent won't be called
  this->setMouseTracking(true);

  {
    assert(this->scene());
    QLinearGradient linearGradient(-500,-500,500,500);
    linearGradient.setColorAt(0.0,QColor(214,214,214));
    linearGradient.setColorAt(1.0,QColor(255,255,255));
    this->scene()->setBackgroundBrush(linearGradient);
    //this->scene()->setBackgroundBrush(QBrush(QColor(255,255,255)));
  }

  //Connect the scene
  #if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
  QObject::connect(scene(),SIGNAL(focusItemChanged(QGraphicsItem*,QGraphicsItem*,Qt::FocusReason)),this,SLOT(onFocusItemChanged(QGraphicsItem*,QGraphicsItem*,Qt::FocusReason)));
  #else

  #endif
  QObject::connect(scene(),SIGNAL(selectionChanged()),this,SLOT(onSelectionChanged()));

  m_examples_item->SetCenterPos(123,456); //Irrelevant where
}

ribi::cmap::QtConceptMap::~QtConceptMap()
{
  delete m_examples_item;
  m_examples_item = nullptr;

  delete m_tools;
  m_tools = nullptr;
  if (m_highlighter) m_highlighter->SetItem(nullptr); //Do this before destroying items
  delete m_arrow;
  m_arrow = nullptr;

}

void ribi::cmap::QtConceptMap::HideExamplesItem() noexcept
{
  m_examples_item->hide();
}

void ribi::cmap::QtConceptMap::RemoveConceptMap()
{
  assert(m_arrow);
  m_arrow->hide();
  assert(m_examples_item);
  m_examples_item->hide();
  if (m_highlighter) m_highlighter->SetItem(nullptr); //Do this before destroying items
  assert(m_tools);
  m_tools->SetBuddyItem(nullptr);
  assert(!m_arrow->isVisible());
}

#ifdef NOT_NOW_20151230
void ribi::cmap::QtConceptMap::DeleteEdge(const Edge& edge)
{
  //Already deleted
  if (!GetQtEdge(edge)) { return; }

  const auto qtedge = GetQtEdge(edge);

  assert(qtedge->scene() == this->GetScene());
  const auto qtfrom = qtedge->GetFrom();
  const auto qtto = qtedge->GetFrom();
  const auto from = qtfrom->GetNode();
  const auto to = qtto->GetNode();

  //Delete the QtNode
  DeleteQtEdge(qtedge);

  if (this->GetConceptMap().HasNode(from))
  {
    const_cast<QtNode*>(qtfrom)->SetSelected(true);
    const_cast<QtNode*>(qtfrom)->setFocus();
  }
  else if (this->GetConceptMap().HasNode(to))
  {
    const_cast<QtNode*>(qtto)->SetSelected(true);
    const_cast<QtNode*>(qtto)->setFocus();
  }
  else if (!this->GetQtNodes().empty())
  {
    this->GetQtNodes().front()->SetSelected(true);
    this->GetQtNodes().front()->setFocus();
  }
  else
  {
    this->m_tools->hide();
  }
}

void ribi::cmap::QtConceptMap::DeleteQtEdge(const QtEdge * const qtedge)
{
  assert(qtedge);
  if (GetVerbosity()) { TRACE("Start of DeleteQtEdge"); }

  if (GetVerbosity()) { TRACE("Does the ConceptMap still have the Edge?"); }
  if (GetConceptMap().HasEdge(qtedge->GetEdge()))
  {
    if (GetVerbosity()) { TRACE("Yes, delete the Edge from the ConceptMap"); }
    GetConceptMap().DeleteEdge(qtedge->GetEdge());
    return;
  }
  else
  {
    if (GetVerbosity()) { TRACE("No, the Edge is already absent int the ConceptMap"); }
  }

  if (GetVerbosity()) { TRACE("Set QtEdge to be not selected"); }
  const_cast<QtEdge*>(qtedge)->SetSelected(false); //Remove const instead of using const-correct std::find on GetQtNodes

  if (GetVerbosity()) { TRACE("Is the QtEdge still present in a QScene?"); }
  if (qtedge->scene())
  {
    if (GetVerbosity()) { TRACE("Yes, QtEdge is still present in a QScene"); }
    assert(qtedge->scene() == GetScene());
    if (GetVerbosity()) { TRACE("Remove QtEdge from QScene"); }
    this->scene()->removeItem(const_cast<QtEdge*>(qtedge));

    if (qtedge->GetQtNode()->scene())
    {
      if (GetVerbosity()) { TRACE("Remove QtEdge its QtNode from QScene"); }
      this->scene()->removeItem(const_cast<QtEdge*>(qtedge)->GetQtNode().get());
    }
    if (qtedge->GetArrow()->scene())
    {
      if (GetVerbosity()) { TRACE("Remove QtEdge its QtQuadBezierArrow from QScene"); }
      this->scene()->removeItem(const_cast<QtEdge*>(qtedge)->GetArrow().get());
    }
  }
}

void ribi::cmap::QtConceptMap::DeleteQtNode(const QtNode * const qtnode)
{
  assert(qtnode);
  //Delete the edges connected to this node
  {
    const std::vector<QtEdge *> qtedges = GetQtEdges();
    const std::size_t sz = qtedges.size();
    for (std::size_t i=0; i!=sz; ++i)
    {
      QtEdge * const qtedge = qtedges[i];
      assert(qtedge);
      if (*qtedge->GetFrom() == *qtnode || *qtedge->GetTo() == *qtnode)
      {
        DeleteQtEdge(qtedge);
      }
    }
  }

  //Remove node from model
  const_cast<QtNode*>(qtnode)->SetSelected(false); //Remove const instead of using const-correct std::find on GetQtNodes
  GetConceptMap().DeleteNode(qtnode->GetNode());

  //Remove node from view
  if (qtnode->scene())
  {
    assert(qtnode->scene() == GetScene());
    this->scene()->removeItem(const_cast<QtNode*>(qtnode));
  }
}
#endif // NOT_NOW_20151230

int ribi::cmap::CountQtEdges(const QGraphicsScene * const scene) noexcept
{
  int cnt{0};
  for (auto item: scene->items()) {
    if (dynamic_cast<QtEdge*>(item)) {
      ++cnt;
    }
  }
  return cnt;
}

int ribi::cmap::CountQtNodes(const QGraphicsScene * const scene) noexcept
{
  int cnt{0};
  for (auto item: scene->items()) {
    if (dynamic_cast<QtNode*>(item)) {
      ++cnt;
    }
  }
  return cnt;
}

int ribi::cmap::CountSelectedQtEdges(const QGraphicsScene * const scene) noexcept
{
  int cnt{0};
  for (auto item: scene->items()) {
    if (dynamic_cast<QtEdge*>(item)
      && dynamic_cast<QtEdge*>(item)->isSelected()
    ) {
      ++cnt;
    }
  }
  return cnt;
}

int ribi::cmap::CountSelectedQtNodes(const QGraphicsScene * const scene) noexcept
{
  int cnt{0};
  for (auto item: scene->items()) {
    if (dynamic_cast<QtNode*>(item)
      && dynamic_cast<QtNode*>(item)->isSelected()
    ) {
      ++cnt;
    }
  }
  return cnt;
}

void ribi::cmap::QtConceptMap::DoCommand(Command * const command) noexcept
{
  m_undo.push(command);
  this->UpdateConceptMap();
  qApp->processEvents();
}

bool ribi::cmap::DoubleCheckEdgesAndNodes(
  const QtConceptMap& qtconceptmap,
  const int n_edges_desired,
  const int n_nodes_desired
)
{
  const auto g = qtconceptmap.GetConceptMap();
  const auto n_nodes = static_cast<int>(boost::num_vertices(g));
  const auto n_edges = static_cast<int>(boost::num_edges(g));
  const auto n_qtnodes = CountQtNodes(qtconceptmap.GetScene());
  const auto n_qtedges = CountQtEdges(qtconceptmap.GetScene());
  if (n_nodes != n_qtnodes)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Internal inconsistency, "
      << "n_nodes (" << n_nodes << ") != n_qtedges (" << n_qtedges << ")"
    ;
    throw std::logic_error(msg.str());
  }
  if (n_edges != n_qtedges)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Internal inconsistency, "
      << "n_edges (" << n_edges << ") != n_qtedges (" << n_qtedges << ")"
    ;
    throw std::logic_error(msg.str());
  }
  if (n_nodes != n_nodes_desired) return false;
  if (n_edges != n_edges_desired) return false;
  return true;
}

bool ribi::cmap::DoubleCheckSelectedEdgesAndNodes(
  const QtConceptMap& qtconceptmap,
  const int n_edges_desired,
  const int n_nodes_desired
)
{
  const auto g = qtconceptmap.GetConceptMap();
  const auto n_selected_nodes = count_vertices_with_selectedness(true,g);
  const auto n_selected_edges = count_edges_with_selectedness(true,g);
  const auto n_selected_qtnodes = CountSelectedQtNodes(qtconceptmap.GetScene());
  const auto n_selected_qtedges = CountSelectedQtEdges(qtconceptmap.GetScene());

  if (n_selected_nodes != n_selected_qtnodes)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Internal inconsistency, "
      << "n_selected_nodes (" << n_selected_nodes << ") != n_selected_qtnodes (" 
      << n_selected_qtnodes << ")"
    ;
    throw std::logic_error(msg.str());
  }
  if (n_selected_edges != n_selected_qtedges)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Internal inconsistency, "
      << "n_selected_edges (" << n_selected_edges << ") != n_selected_qtedges (" 
      << n_selected_qtedges << ")"
    ;
    throw std::logic_error(msg.str());
  }
  if (n_selected_nodes != n_nodes_desired) return false;
  if (n_selected_edges != n_edges_desired) return false;
  return true;
}

ribi::cmap::QtEdge * ribi::cmap::FindQtEdge(
  const int edge_id,
  const QGraphicsScene * const scene
) noexcept
{
  for (auto item: scene->items())
  {
    if (QtEdge * qtedge = dynamic_cast<QtEdge*>(item))
    {
      if (qtedge->GetEdge().GetId() == edge_id) { return qtedge; }
    }
  }
  return nullptr;
}

ribi::cmap::QtEdge * ribi::cmap::FindQtEdge(
  const QtNode* const from,
  const QtNode* const to,
  const QGraphicsScene * const scene
) noexcept
{

  assert(from);
  assert(to);
  assert(from != to);
  const std::vector<QtEdge*> edge_concepts = Collect<QtEdge>(scene);
  const auto iter = std::find_if(edge_concepts.begin(),edge_concepts.end(),
    [from,to](const QtEdge* const qtedge)
    {
      return
        (*qtedge->GetFrom() == *from && *qtedge->GetTo() == *to)
     || (*qtedge->GetFrom() == *to && *qtedge->GetTo() == *from);
    }
  );
  if (iter == edge_concepts.end()) return nullptr;
  return * iter;
}

ribi::cmap::QtNode * ribi::cmap::FindQtNode(
  const int node_id, const QGraphicsScene * const scene) noexcept
{
  for (auto item: scene->items())
  {
    if (QtNode * qtnode = dynamic_cast<QtNode*>(item))
    {
      if (qtnode->GetNode().GetId() == node_id) { return qtnode; }
    }
  }
  return nullptr;
}

ribi::cmap::QtNode * ribi::cmap::QtConceptMap::GetCenterNode() noexcept
{
  assert(scene());
  assert(!scene()->items().isEmpty());
  assert(scene()->items()[0]);
  QList<QGraphicsItem *> v = scene()->items();
  const int n_centernodes{
    static_cast<int>(
      std::count_if(v.begin(),v.end(),
        [this](const QGraphicsItem * const item) { return this->IsQtCenterNode(item); }
      )
    )
  };
  assert(n_centernodes == 0 || n_centernodes == 1);
  if (n_centernodes == 0) return nullptr;
  assert(n_centernodes == 1);
  const auto iter = std::find_if(v.begin(),v.end(),
    [this](const QGraphicsItem * const item) { return this->IsQtCenterNode(item); } );
  assert(iter != v.end());
  QtNode * const center_node = dynamic_cast<QtNode*>(*iter);
  assert(center_node);
  assert(IsQtCenterNode(center_node));
  return center_node;
}

ribi::cmap::QtNode* ribi::cmap::QtConceptMap::GetItemBelowCursor(const QPointF& pos) const
{
  #if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
  const QList<QGraphicsItem*> v = this->scene()->items(pos.x(),pos.y(),2.0,2.0,Qt::IntersectsItemShape,Qt::AscendingOrder);
  #else
  const QList<QGraphicsItem*> v = this->scene()->items(pos.x(),pos.y(),2.0,2.0);
  #endif
  std::vector<QtNode*> qtnodes;
  std::for_each(v.begin(),v.end(),
    [&qtnodes](QGraphicsItem* const item)
    {
      if (QtNode * const qtnode = dynamic_cast<QtNode*>(item))
      {
        assert(!dynamic_cast<QtTool*>(item) && "Cannot draw arrow to ToolsItem");
        qtnodes.push_back(qtnode);
      }
    }
  );
  if (!qtnodes.empty())
  {
    return qtnodes[0];
  }
  return nullptr;
}



std::vector<ribi::cmap::QtEdge*> ribi::cmap::GetQtEdges(
  const QtNode* const from,
  const QGraphicsScene * const scene
) noexcept
{
  assert(from);
  const std::vector<QtEdge*> v = GetQtEdges(scene);
  std::vector<QtEdge*> w;
  std::copy_if(v.begin(),v.end(),std::back_inserter(w),
    [from](const QtEdge* const qtedge)
    {
      return *qtedge->GetFrom() == *from || *qtedge->GetTo() == *from;
    }
  );
  return w;
}


std::vector<ribi::cmap::QtEdge *> ribi::cmap::GetQtEdges(
  const QGraphicsScene * const scene
) noexcept
{
  return Collect<QtEdge>(scene);
}

std::vector<ribi::cmap::QtNode *> ribi::cmap::GetQtNodes(
  const QGraphicsScene * const scene
) noexcept
{
  return Collect<QtNode>(scene);
}

QGraphicsScene* ribi::cmap::QtConceptMap::GetScene() const noexcept
{
  return scene();
}

std::vector<const ribi::cmap::QtEdge *> ribi::cmap::QtConceptMap::GetSelectedQtEdges() const noexcept
{
  std::vector<const ribi::cmap::QtEdge *> selected;
  const auto qtedges = GetQtEdges(GetScene());
  std::copy_if(
    std::begin(qtedges),
    std::end(qtedges),
    std::back_inserter(selected),
    [](const QtEdge* const qtedge) { return qtedge->isSelected() || qtedge->GetQtNode()->isSelected(); }
  );
  return selected;
}

std::vector<const ribi::cmap::QtNode *> ribi::cmap::QtConceptMap::GetSelectedQtNodes() const noexcept
{
  std::vector<const ribi::cmap::QtNode *> selected;
  const auto qtnodes = GetQtNodes(GetScene());
  std::copy_if(
    std::begin(qtnodes),
    std::end(qtnodes),
    std::back_inserter(selected),
    [](const QtNode* const qtnode) { return qtnode->isSelected(); }
  );
  return selected;
}

std::string ribi::cmap::QtConceptMap::GetVersion() noexcept
{
  return "2.0";
}

std::vector<std::string> ribi::cmap::QtConceptMap::GetVersionHistory() noexcept
{
  return {
    "2012-xx-xx: version 1.0: initial version"
    "2013-12-03: version 1.1: start of versioning"
    "2015-08-12: version 2.0: merge with QtConceptMap"
  };
}


bool ribi::cmap::QtConceptMap::IsQtCenterNode(const QGraphicsItem* const item)
{
  const QtCenterNode * const qtnode = dynamic_cast<const QtCenterNode*>(item);
  assert(!qtnode || IsCenterNode(qtnode->GetNode()));
  return qtnode;
}

void ribi::cmap::QtConceptMap::keyPressEvent(QKeyEvent *event) noexcept
{
  UpdateConceptMap();

  switch (event->key())
  {
    case Qt::Key_Delete:
    {
      UpdateConceptMap();
      if (GetVerbosity()) { TRACE("Pressing delete"); }
      try
      {

        DoCommand(new CommandDeleteSelected(m_conceptmap,scene(),m_tools));
      }
      catch (std::logic_error& e)
      {
        if (GetVerbosity()) { TRACE(e.what()); }
      }
    }
    return;
    case Qt::Key_Escape:
    {
      if (GetVerbosity()) { TRACE("Pressing Escape"); }
      //Only remove the 'new arrow' if present
      if (m_arrow->isVisible())
      {
        if (GetVerbosity()) { TRACE("Remove the new arrow"); }
        m_arrow->hide();
        assert(!m_arrow->isVisible());
        return;
      }
    }
    break;
    case Qt::Key_Equal:
      if (GetVerbosity()) { TRACE("Pressing Qt::Key_Equal"); }
      this->scale(1.1,1.1);
      break;
    case Qt::Key_Minus:
      if (GetVerbosity()) { TRACE("Pressing Qt::Key_Minus"); }
      this->scale(0.9,0.9);
      break;
    case Qt::Key_E:
      if (event->modifiers() & Qt::ControlModifier)
      {
        if (GetVerbosity()) { TRACE("Pressing CTRL-E"); }
        try { this->DoCommand(new CommandCreateNewEdgeBetweenTwoSelectedNodes(GetConceptMap(),scene(),m_tools)); }
        catch (std::logic_error& ) {}
      }
      return;
    case Qt::Key_N:
      if (event->modifiers() & Qt::ControlModifier)
      {
        if (GetVerbosity()) { TRACE("Pressing CTRL-N"); }
        try { this->DoCommand(new CommandCreateNewNode(m_conceptmap,scene(),m_tools,0.0,0.0)); }
        catch (std::logic_error& ) {}
      }
      return;
    case Qt::Key_Z:
      if (event->modifiers() & Qt::ControlModifier)
      {
        if (event->modifiers() & Qt::ShiftModifier)
        {
          this->m_undo.redo();
        }
        else
        {
          this->m_undo.undo();
        }
      }
      return;
    case Qt::Key_Question:
      if (GetVerbosity()) { TRACE("Pressing Qt::Key_Question"); }
      UpdateConceptMap();
      break;

  }

  QtKeyboardFriendlyGraphicsView::keyPressEvent(event);
  UpdateConceptMap();
}

void ribi::cmap::QtConceptMap::mouseDoubleClickEvent(QMouseEvent *event)
{
  //Create new node at the mouse cursor its position
  try {
    const QPointF pos = mapToScene(event->pos());
    this->DoCommand(
      new CommandCreateNewNode(
        m_conceptmap,
        scene(),
        m_tools,
        pos.x(),
        pos.y()
      )
    );
  }
  catch (std::logic_error& ) {}
  UpdateExamplesItem();
}

void ribi::cmap::QtConceptMap::mouseMoveEvent(QMouseEvent * event)
{
  if (m_arrow->isVisible())
  {
    const QPointF pos = mapToScene(event->pos());
    m_arrow->SetHeadPos(pos.x(),pos.y());

    //Move the item under the arrow
    QtNode* const item_below = GetItemBelowCursor(mapToScene(event->pos()));

    assert(m_highlighter);
    m_highlighter->SetItem(item_below); //item_below is allowed to be nullptr
  }
  else
  {
    assert(m_highlighter);
    m_highlighter->SetItem(nullptr); //item_below is allowed to be nullptr
  }
  QtKeyboardFriendlyGraphicsView::mouseMoveEvent(event);
}

void ribi::cmap::QtConceptMap::mousePressEvent(QMouseEvent *event)
{
  if (GetVerbosity()) { TRACE_FUNC(); }
  UpdateConceptMap();
  assert(m_highlighter);
  if (m_arrow->isVisible())
  {
    if (m_highlighter->GetItem() && m_arrow->GetFrom() != m_highlighter->GetItem())
    {
      //The command needs to find the two selected vertices
      for (auto& i: scene()->selectedItems()) { i->setSelected(false); }
      m_highlighter->GetItem()->SetSelected(true);
      m_arrow->GetFrom()->SetSelected(true);
      try
      {
        const auto command = new CommandCreateNewEdgeBetweenTwoSelectedNodes(
          m_conceptmap,
          scene(),
          m_tools
        );
        DoCommand(command);
        UpdateConceptMap();
      }
      catch (std::logic_error&) { return; }
    }
    m_arrow->hide();
    assert(m_highlighter);
    m_highlighter->SetItem(nullptr);
  }

  QtKeyboardFriendlyGraphicsView::mousePressEvent(event);

  UpdateExamplesItem();
}

void ribi::cmap::QtConceptMap::onFocusItemChanged(
  QGraphicsItem * newFocus, QGraphicsItem */*oldFocus*/, Qt::FocusReason reason
)
{
  if (const QtNode * const qtnode = dynamic_cast<const QtNode*>(newFocus)) {
    m_tools->SetBuddyItem(qtnode);
    onSelectionChanged();
  }
  if (newFocus == m_tools && !m_arrow->isVisible() && m_tools->GetBuddyItem() && reason == Qt::MouseFocusReason) {
    m_arrow->Start(
      m_tools->GetBuddyItem(),m_tools->GetBuddyItem()->GetCenterPos()
    ); //Also sets visibility
  }
}

void ribi::cmap::QtConceptMap::OnEdgeKeyDownPressed(QtEdge* const edge, const int key)
{
  assert(edge);
  if (!edge) throw std::logic_error("ribi::cmap::QtConceptMap::OnEdgeKeyDownPressed: edge must be non-nullptr");
  if (key != Qt::Key_F2) return;
}

void ribi::cmap::QtConceptMap::OnNodeKeyDownPressed(QtNode* const item, const int key)
{
  if (key != Qt::Key_F2) return;
  assert(item);

  {
    QtScopedDisable<QtConceptMap> disable(this);
    QtConceptMapConceptEditDialog d(item->GetNode().GetConcept());
    d.exec();
  }
  this->show();
  this->setFocus();
  this->scene()->setFocusItem(item);
  item->SetSelected(true);
  this->scene()->update();
}


void ribi::cmap::QtConceptMap::onSelectionChanged()
{
  Graph& g = m_conceptmap;

  //Selectness of vertices
  const auto vip = vertices(g);
  std::for_each(vip.first,vip.second,
    [this, &g](const VertexDescriptor vd) {
      const auto vertex_map = get(boost::vertex_custom_type,g);
      const auto is_selected_map = get(boost::vertex_is_selected,g);
      const auto qtnode = FindQtNode(get(vertex_map,vd).GetId(),GetScene());
      //qtnode can be nullptr when onSelectionChanged is called while building up the QtConceptMap
      if (qtnode) { put(is_selected_map,vd,qtnode->isSelected()); }
    }
  );

  //Selectness of edges
  const auto eip = edges(g);
  std::for_each(eip.first,eip.second,
    [this, &g](const EdgeDescriptor ed) {
      const auto edge_map = get(boost::edge_custom_type,g);
      const auto is_selected_map = get(boost::edge_is_selected,g);
      const auto qtedge = FindQtEdge(get(edge_map,ed).GetId(),GetScene());
      if (qtedge) { put(is_selected_map,ed,qtedge->isSelected()); }
    }
  );
  scene()->update();
}

void ribi::cmap::QtConceptMap::OnToolsClicked()
{
  const QPointF cursor_pos_approx(
    m_tools->GetBuddyItem()->GetCenterX(),
    m_tools->GetBuddyItem()->GetCenterY() - 32.0
    - m_tools->GetBuddyItem()->GetRadiusY()
  );
  m_arrow->Start(
    m_tools->GetBuddyItem(),cursor_pos_approx
  );
  m_arrow->update();
  this->scene()->update();
}



void ribi::cmap::QtConceptMap::RepositionItems()
{
  assert(!"Not used");

  {
    //The ray of the upcoming circle of nodes, is the larger of
    //(1) half of the diagonal of the focal question (e.g. for short concepts)
    //(2) calculated from the circumference by adding the nodes' length
    const std::vector<QtNode *> qtnode_concepts_unsorted = Collect<QtNode>(scene());

    if (qtnode_concepts_unsorted.empty()) return;

    const std::vector<QtNode *> qtnode_concepts = Sort(qtnode_concepts_unsorted);
    assert(!qtnode_concepts.empty());
    assert(!qtnode_concepts.empty());
    assert(qtnode_concepts[0]);
    const QtNode * const qtcenter_node
      = dynamic_cast<const QtNode *>(qtnode_concepts[0]);
    assert(qtcenter_node);
    //assert(qtcenter_node->GetOuterRect(). GetOuterX() > -0.5);
    //assert(qtcenter_node->GetOuterX() <  0.5);
    //assert(qtcenter_node->GetOuterY() > -0.5);
    //assert(qtcenter_node->GetOuterY() <  0.5);

    const double r1
      = 0.5 * ribi::cmap::GetDistance(
        qtcenter_node->boundingRect().width(),
        qtcenter_node->boundingRect().height()
      );
    const double r3 = 50.0;
    const double r = std::max(r1,r3);
    assert(r > 10.0);
    const int n_nodes = qtnode_concepts.size();
    for (int i = 1; i!=n_nodes; ++i) //+1 to skip center node
    {
      //Added +0 (instead of -1) to n_nodes, to prevent, in a setup with two concepts and
      //one edge, the edge to overlap the central question
      const double pi = boost::math::constants::pi<double>();
      const double angle
        = 2.0 * pi * boost::numeric_cast<double>(i)
        / boost::numeric_cast<double>(n_nodes - 1);
      const double x =  std::cos(angle) * r;
      const double y = -std::sin(angle) * r;
      QtNode * const qtnode = qtnode_concepts[i];
      qtnode->GetNode().SetPos(x,y);
      //qtnode->setPos(x,y);
      #ifndef NDEBUG
      //const double epsilon = 0.000001;
      #endif
      //assert(std::abs(x - qtnode->GetOuterX()) < epsilon);
      //assert(std::abs(x - qtnode->GetNode().GetX()) < epsilon);
      //assert(std::abs(y - qtnode->GetOuterY()) < epsilon);
      //assert(std::abs(y - qtnode->GetNode().GetY()) < epsilon);

    }
  }

  {
    //Put the edge concepts in the middle of the nodes
    const std::vector<QtEdge *> qtedge_concepts = Collect<QtEdge>(scene());
    std::for_each(qtedge_concepts.begin(), qtedge_concepts.end(),
      [](QtEdge * const qtedge)
      {
        const QPointF p((qtedge->GetFrom()->GetCenterPos() + qtedge->GetTo()->GetCenterPos()) / 2.0);
        const double new_x = p.x();
        const double new_y = p.y();
        qtedge->GetEdge().GetNode().SetX(new_x);
        qtedge->GetEdge().GetNode().SetY(new_y);
      }
    );
  }

  //Put the nodes around the focal question in their improved position
  //If there is no focal node, the non-focal nodes are put around an empty spot
  for (int i=0; i!=10; ++i) //NOTE: maybe replace by while (1)
  {
    bool done = true;
    const std::vector<QtNode *> qtnodes = Sort(Collect<QtNode>(scene()));
    assert(!qtnodes.empty());
    assert(qtnodes[0]);
    //assert(IsCenterNode(qtnodes[0]));
    const std::vector<QtEdge* > qtedges = Collect<QtEdge>(scene());

    //First node
    //const bool is_first_node_center_node {
    //  boost::dynamic_pointer_cast<QtCenterNode>(qtnodes[0])
    //};
    const QtNode * const first_node { qtnodes[0] };
    assert(first_node);

    std::vector<QGraphicsItem*> nodes_and_edges;
    std::copy(qtnodes.begin(),qtnodes.end(),std::back_inserter(nodes_and_edges));
    std::copy(qtedges.begin(),qtedges.end(),std::back_inserter(nodes_and_edges));

    //Move the nodes away from the center
    std::for_each(
      nodes_and_edges.begin() + 1, //+1 to skip the center node at [0]
      nodes_and_edges.end(),
      [first_node,&done](QGraphicsItem* const node_or_edge)
      {
        if (first_node->boundingRect().intersects(
          node_or_edge->boundingRect().translated(-node_or_edge->pos())))
        {
          const double cur_x = node_or_edge->x();
          const double cur_y = node_or_edge->y();
          const double new_x = cur_x + (node_or_edge->x() < first_node->x() ? -1.0 : 1.0);
          const double new_y = cur_y + (node_or_edge->y() < first_node->y() ? -1.0 : 1.0);
          if (QtNode * const qtnode = dynamic_cast<QtNode *>(node_or_edge))
          {
            qtnode->GetNode().SetX(new_x);
            qtnode->GetNode().SetY(new_y);
          }
          else
          {
            QtEdge * const qtedge = dynamic_cast<QtEdge *>(node_or_edge);
            assert(qtedge && "Every item is either a Qt node or Qt edge");
            qtedge->GetEdge().GetNode().SetX(new_x);
            qtedge->GetEdge().GetNode().SetY(new_y);
            //node->setPos(QPointF(new_x,new_y));
          }
          done = false;
        }
      }
    );

    if (done) break;
  }
}

void ribi::cmap::QtConceptMap::SetConceptMap(const ConceptMap& conceptmap)
{
  RemoveConceptMap();
  m_conceptmap = conceptmap;
  assert(GetConceptMap() == conceptmap);

  assert(this->scene());

  //This std::vector keeps the QtNodes in the same order as the nodes in the concept map
  //You cannot rely on Collect<QtConceptMapNodeConcept*>(scene), as this shuffles the order
  //std::vector<QtNode*> qtnodes;

  assert(Collect<QtNode>(scene()).empty());

  //Add the nodes to the scene, if there are any
  const auto vip = vertices(m_conceptmap);
  for(auto i=vip.first; i!=vip.second; ++i)
  {
    const auto pmap = get(boost::vertex_custom_type, m_conceptmap);
    const Node node = get(pmap, *i);
    const bool is_focal_node{i == vip.first};
    QtNode * const qtnode{new QtNode(node)};
    if (is_focal_node) { qtnode->setFlags(0); }
    assert(qtnode);
    scene()->addItem(qtnode);
    assert(FindQtNode(node.GetId(), scene()));
  }
  //Add the Edges
  const auto eip = edges(m_conceptmap);
  for(auto i = eip.first; i != eip.second; ++i)
  {
    const VertexDescriptor vd_from = boost::source(*i, m_conceptmap);
    const VertexDescriptor vd_to = boost::target(*i, m_conceptmap);
    assert(vd_from != vd_to);
    const auto vertex_map = get(boost::vertex_custom_type, m_conceptmap);
    const Node from = get(vertex_map, vd_from);
    const Node to = get(vertex_map, vd_to);
    assert(from.GetId() != to.GetId());
    QtNode * const qtfrom = FindQtNode(from.GetId(), scene());
    QtNode * const qtto = FindQtNode(to.GetId(), scene());
    assert(qtfrom != qtto);
    const auto edge_map = get(boost::edge_custom_type, m_conceptmap);
    const Edge edge = get(edge_map, *i);
    QtEdge * const qtedge{new QtEdge(edge,qtfrom,qtto)};
    if (qtfrom->GetNode().IsCenterNode() || qtto->GetNode().IsCenterNode())
    {
      qtedge->GetQtNode()->setVisible(false);
    }
    scene()->addItem(qtedge);
  }
  assert(GetConceptMap() == conceptmap);
}

void ribi::cmap::QtConceptMap::Undo() noexcept
{
  assert(m_undo.count() > 0);
  m_undo.undo();
}

void ribi::cmap::QtConceptMap::UpdateConceptMap()
{
  UpdateExamplesItem();
  for (const auto item: this->scene()->items()) { item->update(); }
  onSelectionChanged();
}

void ribi::cmap::QtConceptMap::UpdateExamplesItem()
{
  //If nothing is selected, hide the Examples
  m_examples_item->SetBuddyItem(nullptr); //Handles visibility
  assert(GetScene());
  if (GetScene()->selectedItems().count() == 1)
  {
    QtNode * const selected_qtnode = dynamic_cast<QtNode*>(GetScene()->selectedItems().first());
    if (selected_qtnode)
    {
      m_examples_item->SetBuddyItem(selected_qtnode); //Handles visibility
    }
  }
}
