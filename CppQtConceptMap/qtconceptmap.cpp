//---------------------------------------------------------------------------
/*
QtConceptMap, Qt classes for display and interaction with ConceptMap
Copyright (C) 2013-2015 The Brainweaver Team

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

#include <QGraphicsScene>
#include <QKeyEvent>

#include "fuzzy_equal_to.h"
#include "qtconceptmapcollect.h"

#include "container.h"
#include "conceptmapconceptfactory.h"
#include "conceptmapconcept.h"
#include "conceptmapfactory.h"
#include "conceptmap.h"
#include "conceptmapedgefactory.h"
#include "conceptmapedge.h"
#include "conceptmaphelper.h"
#include "conceptmapnodefactory.h"
#include "conceptmapnode.h"
#include "qtconceptmapcommanddeleteedge.h"
#include "qtconceptmapcommanddeletenode.h"
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
    m_signal_conceptmapitem_requests_edit{},
    m_signal_request_rate_concept{},
    m_arrow(nullptr),
    m_conceptmap{},
    m_examples_item(new QtExamplesItem),
    m_highlighter{new QtItemHighlighter},
    m_tools{new QtTool}
{
  #ifndef NDEBUG
  Test();
  this->SetVerbosity(true);
  #endif

  this->setScene(new QGraphicsScene(this));

  assert(!m_examples_item->scene());
  scene()->addItem(m_examples_item); //Add the examples so it has a parent

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
}

ribi::cmap::QtConceptMap::~QtConceptMap()
{
  delete m_examples_item;
  m_examples_item = nullptr;

  delete m_tools;
  m_tools = nullptr;
  if (m_highlighter) m_highlighter->SetItem(nullptr); //Do this before destroying items
  m_arrow = nullptr;

}


ribi::cmap::QtEdge * ribi::cmap::QtConceptMap::AddEdge(
  const Edge& /* edge */
)
{
  #ifdef NOT_NOW_20151230

  if (GetVerbosity()) { TRACE("Start of 'QtConceptMap::AddEdge(const Edge edge)'"); }

  //Already added
  if (GetQtEdge(edge))
  {
    if (GetVerbosity()) { TRACE("QtEdge already present"); }
    return GetQtEdge(edge);
  }

  QtNode * const qtfrom = GetQtNode(*edge.GetFrom());
  assert(qtfrom);
  QtNode * const qtto   = GetQtNode(*edge.GetTo());
  assert(qtto);
  assert(qtfrom != qtto);
  QtEdge * const qtedge = new QtEdge(
    edge,
    qtfrom,
    qtto
  );

  //Edges connected to the center node do not show their concepts
  if (IsQtCenterNode(qtfrom) || IsQtCenterNode(qtto))
  {
    //assert(qtconcept == qtedge.GetDisplayStrategy());
    //qtconcept->setVisible(false);
  }

  if (GetVerbosity()) { TRACE("Adding QtEdge (and its QtQuadBezierArrow and QtNode) to QtConceptMap's QScene"); }
  assert(!qtedge->scene());
  this->scene()->addItem(qtedge); //QtEdge adds the QtQuadBezierArrow and QtNode to the QScene when QtEdge::paint is called

  qtfrom->SetSelected(false);
  qtto->SetSelected(false);
  qtedge->GetQtNode()->SetSelected(true);

  if (GetScene()->focusItem() && GetScene()->focusItem() != qtedge->GetQtNode().get())
  {
    GetScene()->focusItem()->clearFocus();
    qtedge->GetQtNode()->setFocus();
  }

  //this->GetConceptMap().SetSelected( ConceptMap::Edges( { qtedge->GetEdge() } ) );
  //assert(qtedge->GetQtNode()->isSelected());
  return qtedge;
  #endif // NOT_NOW_20151230
  return nullptr;
}


void ribi::cmap::QtConceptMap::CleanMe()
{
  //Prepare cleaning the scene
  assert(GetQtExamplesItem());
  SetExamplesItem(nullptr);

  assert(m_tools);
  this->m_tools = nullptr;
  assert(!m_tools);
  if (m_highlighter) m_highlighter->SetItem(nullptr); //Do this before destroying items

  this->m_arrow = nullptr;

  assert(!m_tools);

  //Clear the scene, invalidates all scene items copies
  assert(this->scene());
  this->scene()->clear();

  //Put stuff back in
  assert(!m_tools);

  //Add the invisible examples item
  {
    assert(!GetQtExamplesItem());
    QtExamplesItem * const examples_item = new QtExamplesItem;
    assert(examples_item);
    assert(!m_tools);
    SetExamplesItem(examples_item);
    assert(!m_tools);
    //Signal #4
    examples_item->m_signal_request_scene_update.connect(
      boost::bind(
        &ribi::cmap::QtConceptMap::OnRequestSceneUpdate,this));
    examples_item->setVisible(false);
    assert(!examples_item->scene());
    this->scene()->addItem(examples_item);
  }

  //Add the tools item
  {
    assert(!m_tools);
    m_tools = new QtTool;
    //Signal #5
    m_tools->m_signal_clicked.connect(
      boost::bind(
        &ribi::cmap::QtConceptMap::OnToolsClicked,
        this));
    assert(!m_tools->scene());
    this->scene()->addItem(m_tools);
  }
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

void ribi::cmap::QtConceptMap::DeleteNode(const Node& node)
{
  //Already deleted
  if (!GetQtNode(node)) { return; }

  assert(GetQtNode(node)->scene() == this->GetScene());

  //Delete the QtNode
  DeleteQtNode(GetQtNode(node));

  for (const auto new_selected_nodes: this->GetConceptMap().GetSelectedNodes())
  {
    assert(GetQtNode(new_selected_nodes));
    GetQtNode(new_selected_nodes)->SetSelected(true);
  }

  if (this->GetQtNodes().empty())
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

int ribi::cmap::CountQtEdges(const QGraphicsScene& scene) noexcept
{
  int cnt{0};
  for (auto item: scene.items()) {
    if (dynamic_cast<QtEdge*>(item)) {
      ++cnt;
    }
  }
  return cnt;
}

int ribi::cmap::CountQtNodes(const QGraphicsScene& scene) noexcept
{
  int cnt{0};
  for (auto item: scene.items()) {
    if (dynamic_cast<QtNode*>(item)) {
      ++cnt;
    }
  }
  return cnt;
}

void ribi::cmap::QtConceptMap::DoCommand(Command * const command) noexcept
{
  m_undo.push(command);
}

ribi::cmap::QtNode * ribi::cmap::FindQtNode(
  const Node& node, const QGraphicsScene& scene) noexcept
{
  for (auto item: scene.items())
  {
    if (QtNode * qtnode = dynamic_cast<QtNode*>(item))
    {
      if (qtnode->GetNode() == node) { return qtnode; }
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

const ribi::cmap::QtEdge * ribi::cmap::QtConceptMap::GetQtEdge(
  const Edge& edge) const noexcept
{
  const auto v(GetQtEdges(*GetScene()));
  for (const auto e:v)
  {
    if (e->GetEdge() == edge) return e;
  }
  return nullptr;
}

ribi::cmap::QtEdge * ribi::cmap::QtConceptMap::GetQtEdge(
  const Edge& edge) noexcept
{
  //Calls the const version of this member function
  //To avoid duplication in const and non-const member functions [1]
  //[1] Scott Meyers. Effective C++ (3rd edition). ISBN: 0-321-33487-6.
  //    Item 3, paragraph 'Avoid duplication in const and non-const member functions'
  QtEdge * const qtedge(
    const_cast<QtEdge *>(
      dynamic_cast<const QtConceptMap*>(this)->GetQtEdge(edge)
    )
  );
  return qtedge;
}

ribi::cmap::QtEdge * ribi::cmap::QtConceptMap::GetQtEdge(
  const QtEdge* const edge) noexcept
{
  //Calls the const version of this member function
  //To avoid duplication in const and non-const member functions [1]
  //[1] Scott Meyers. Effective C++ (3rd edition). ISBN: 0-321-33487-6.
  //    Item 3, paragraph 'Avoid duplication in const and non-const member functions'
  QtEdge * const qtedge(
    const_cast<QtEdge *>(
      dynamic_cast<const QtConceptMap*>(this)->GetQtEdgeConst(edge)
    )
  );
  return qtedge;
}

const ribi::cmap::QtEdge * ribi::cmap::QtConceptMap::GetQtEdgeConst(
  const QtEdge* const edge) const noexcept
{
  //return FindQtEdgeConst(edge->GetFrom().get(),edge->GetTo().get());
  return GetQtEdgeConst(edge->GetFrom(),edge->GetTo());
}

const ribi::cmap::QtEdge * ribi::cmap::QtConceptMap::GetQtEdgeConst(
  const QtNode* const from,
  const QtNode* const to) const noexcept
{

  assert(from);
  assert(to);
  assert(from != to);
  const std::vector<QtEdge*> edge_concepts = Collect<QtEdge>(scene());
  const auto iter = std::find_if(edge_concepts.begin(),edge_concepts.end(),
    [from,to](const QtEdge* const edge)
    {
      return
        (*edge->GetFrom() == *from && *edge->GetTo() == *to)
     || (*edge->GetFrom() == *to && *edge->GetTo() == *from);
    }
  );
  if (iter == edge_concepts.end()) return nullptr;
  return * iter;
}

std::vector<ribi::cmap::QtEdge*> ribi::cmap::GetQtEdges(
  const QtNode* const from,
  const QGraphicsScene& scene
) noexcept
{
  assert(from);
  const std::vector<QtEdge*> v = GetQtEdges(scene);
  std::vector<QtEdge*> w;
  std::copy_if(v.begin(),v.end(),std::back_inserter(w),
    [from](const QtEdge* const edge)
    {
      return *edge->GetFrom() == *from || *edge->GetTo() == *from;
    }
  );
  return w;
}


std::vector<ribi::cmap::QtEdge *> ribi::cmap::GetQtEdges(
  const QGraphicsScene& scene
) noexcept
{
  return Collect<QtEdge>(&scene);
}

std::vector<ribi::cmap::QtNode *> ribi::cmap::GetQtNodes(
  const QGraphicsScene& scene
) noexcept
{
  return Collect<QtNode>(&scene);
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
  switch (event->key())
  {
    case Qt::Key_Space:
    {
      if (GetVerbosity()) { TRACE("Pressing space"); }
      try
      {
        DoCommand(new CommandSelectRandomNode(m_conceptmap,scene(),m_tools));
      }
      catch (std::logic_error& e)
      {
        if (GetVerbosity()) { TRACE(e.what()); }
      }
    }
    break;
    case Qt::Key_Delete:
    {
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
      if (m_arrow)
      {
        if (GetVerbosity()) { TRACE("Remove the new arrow"); }
        this->scene()->removeItem(m_arrow);
        m_arrow = nullptr;
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
        try { this->DoCommand(new CommandCreateNewEdgeBetweenTwoSelectedNodes(GetConceptMap())); }
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
          TRACE("Try REDO");
          this->m_undo.redo();
          //if (GetConceptMap().CanRedo()) { GetConceptMap().Redo(); }
        }
        else
        {
          TRACE("Try UNDO");
          this->m_undo.undo();
          //if (GetConceptMap().CanUndo()) { GetConceptMap().Undo(); }
        }
      }
      return;
    case Qt::Key_Question:
      if (GetVerbosity()) { TRACE("Pressing Qt::Key_Question"); }
      UpdateSelection();
      break;

  }

  QtKeyboardFriendlyGraphicsView::keyPressEvent(event);
  UpdateSelection();
}

void ribi::cmap::QtConceptMap::OnItemRequestsUpdate(const QGraphicsItem* const item)
{
  m_tools->SetBuddyItem(dynamic_cast<const QtNode*>(item));
  GetQtExamplesItem()->SetBuddyItem(item);
  scene()->update();
}

void ribi::cmap::QtConceptMap::OnItemSelectedChanged(
  QGraphicsItem* const /* item */
)
{
#ifdef NOT_NOW_20151230
  if (GetVerbosity()) { std::clog << "A QGraphicsItem has its selectedness changed" << std::endl; }
  //What changes its selection
  if (QtNode * const qtnode = dynamic_cast<QtNode*>(item))
  {
    assert(!dynamic_cast<QtEdge*>(item));
    assert(!GetConceptMap().GetEdgeHaving(qtnode->GetNode()));
    //Does the QtNode gain or lose selection?
    if (qtnode->isSelected())
    {
      //QtNode gains selection
      if (GetVerbosity()) { std::clog << "A QtNode got selected" << std::endl; }
      if (m_conceptmap.IsSelected(qtnode->GetNode()))
      {
        if (GetVerbosity()) { std::clog << "Warning: QtNode its node already was selected" << std::endl; }
      }
      m_conceptmap.AddSelected( ConceptMap::Nodes( { qtnode->GetNode() } ) );
    }
    else
    {
      //QtNode loses selection
      if (m_conceptmap.IsSelected(qtnode->GetNode()))
      {
        if (GetVerbosity()) { std::clog << "A QtNode got unselected" << std::endl; }
        m_conceptmap.RemoveSelected( ConceptMap::Nodes( { qtnode->GetNode() } ) );
      }
      else
      {
        if (GetVerbosity())
        {
          std::clog << "Warning: a QtNode got unselected, the Node already was unselected" << std::endl;
        }
      }
    }
  }
  else if (QtEdge * const qtedge = dynamic_cast<QtEdge*>(item))
  {
    //Does the QtEdge gain or lose selection?
    if (qtedge->isSelected())
    {
      if (GetVerbosity()) { std::clog << "A QtEdge got selected" << std::endl; }
      if (m_conceptmap.IsSelected(qtedge->GetEdge()))
      {
        std::clog << "Warning: a QtEdge got selected, the Edge already was selected" << std::endl;
      }
      m_conceptmap.AddSelected( ConceptMap::Edges( { qtedge->GetEdge() } ) );
    }
    else
    {
      if (GetVerbosity()) { std::clog << "A QtEdge got unselected" << std::endl; }
      if (!m_conceptmap.IsSelected(qtedge->GetEdge()))
      {
        std::clog << "Warning: a QtEdge got unselected, the Edge already was unselected" << std::endl;
      }
      m_conceptmap.RemoveSelected( ConceptMap::Edges( { qtedge->GetEdge() } ) );
    }
  }
  else
  {
    if (GetVerbosity())
    {
      std::clog << "Warning: something unknown changed its selectionness" << std::endl;
    }
  }
#endif
}

void ribi::cmap::QtConceptMap::OnRequestSceneUpdate()
{
  scene()->update();
}


void ribi::cmap::QtConceptMap::RepositionItems()
{

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
  CleanMe();
  m_conceptmap = conceptmap;

  assert(this->scene());

  //This std::vector keeps the QtNodes in the same order as the nodes in the concept map
  //You cannot rely on Collect<QtConceptMapNodeConcept*>(scene), as this shuffles the order
  std::vector<QtNode*> qtnodes;

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
    //AddNode(node,is_focal_node); //See AddNode command
  }
  //Add the Edges
  const auto eip = edges(m_conceptmap);
  for(auto i=eip.first; i!=eip.second; ++i)
  {
    const auto pmap = get(boost::edge_custom_type, m_conceptmap);
    const Edge edge = get(pmap, *i);
    this->AddEdge(edge);
  }
  assert(GetConceptMap() == conceptmap);
}

void ribi::cmap::QtConceptMap::SetExamplesItem(QtExamplesItem * const item)
{
  assert((item || !item) && "Can be both");
  m_examples_item = item;
}

#ifdef NOT_NOW_20151230
#ifndef NDEBUG
void ribi::cmap::QtConceptMap::TestMe(const ConceptMap map) const
{
  {
    std::set<Node> w;
    const std::vector<Node> v = map.GetNodes();
    std::transform(v.begin(),v.end(),std::inserter(w,w.begin()),
      [](const Node& node)
      {
        return node;
      }
    );
    assert(v.size() == w.size() && "All nodes must be unique");
  }
  {
    std::set<Edge> w;
    const std::vector<Edge> v = map.GetEdges();
    std::transform(v.begin(),v.end(),std::inserter(w,w.begin()),
      [](const Edge& ptr)
      {
        return ptr; //TODO: replace by std::copy
      }
    );
    assert(v.size() == w.size() && "All edges must be unique");
  }
  {
    std::set<QGraphicsItem*> v;
    const std::vector<QtNode*> node_concepts = Collect<QtNode>(scene());
    std::copy(node_concepts.begin(),node_concepts.end(),std::inserter(v,v.begin()));
    const std::vector<QtEdge*> edge_concepts = Collect<QtEdge>(scene());
    std::copy(edge_concepts.begin(),edge_concepts.end(),std::inserter(v,v.begin()));
    assert(v.size() == (node_concepts.size() + edge_concepts.size()) && "All Qt nodes must be unique");
  }
  {
    ///Note that the ConceptMap read out again differs from the original,
    ///because the Nodes are placed
    if (!cmap::HasSameContent(GetConceptMap(),map))
    {
      //OH OH, AN ERROR! HELP ME OUT AND GIMME LOTS OF DEBUG INFO!
      {
        const std::vector<std::string> v
          = xml::XmlToPretty(ToXml(map));
        std::clog << "original map:\n";
        std::clog << "\n";
        std::copy(v.begin(),v.end(),std::ostream_iterator<std::string>(std::clog,"\n"));
        std::clog << "\n";
      }
      {
        const std::vector<std::string> v
          = xml::XmlToPretty(cmap::ToXml(GetConceptMap()));
        std::clog << "GetConceptMap():\n";
        std::clog << "\n";
        std::copy(v.begin(),v.end(),std::ostream_iterator<std::string>(std::clog,"\n"));
        std::clog << "\n";
      }

      TRACE(cmap::ToXml(GetConceptMap()));
      TRACE(cmap::ToXml(map));
    }
  }
  assert(cmap::HasSameContent(GetConceptMap(),map)
    && "The concept map supplied must be homomorphous to the one created in the widget");

}
#endif
#endif // NOT_NOW_20151230

void ribi::cmap::QtConceptMap::mouseDoubleClickEvent(QMouseEvent *event)
{
  if (GetVerbosity()) { TRACE_FUNC(); }
  try
  {
    this->DoCommand(
      new CommandCreateNewNode(
        m_conceptmap,
        scene(),
        m_tools,
        mapToScene(event->pos()).x(),
        mapToScene(event->pos()).y()
      )
    );
  }
  catch (std::logic_error& )
  {

  }
}

void ribi::cmap::QtConceptMap::mouseMoveEvent(QMouseEvent * event)
{
  //if (GetVerbosity()) { TRACE_FUNC(); }

  if (m_arrow)
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
  assert(m_highlighter);
  if (m_arrow) //&& m_highlighter->GetItem())
  {
    if (m_highlighter->GetItem() && m_arrow->GetFrom() != m_highlighter->GetItem())
    {
      //assert(!dynamic_cast<QtTool*>(m_highlighter->GetItem()) && "Cannot select a ToolsItem");
      //AddEdge( m_arrow->GetFrom(),m_highlighter->GetItem());
      try
      {
        const auto command = new CommandCreateNewEdgeBetweenTwoSelectedNodes(GetConceptMap());
        DoCommand(command);
        UpdateSelection();
      }
      catch (std::logic_error&) { return; }
    }
    this->scene()->removeItem(m_arrow);
    m_arrow = nullptr;
    assert(m_highlighter);
    m_highlighter->SetItem(nullptr);
  }

  QtKeyboardFriendlyGraphicsView::mousePressEvent(event);

  //If nothing is selected, hide the Examples
  if (!GetScene()->focusItem() && !this->GetScene()->selectedItems().count())
  {
    //Let any node (in this case the central node) emit an update for the Examples
    //to hide.
    if (GetCenterNode()) {
      GetCenterNode()->m_signal_node_changed(GetCenterNode());
    }
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
  item->m_signal_node_changed(item);
  this->scene()->update();
  this->OnItemRequestsUpdate(item);
}

void ribi::cmap::QtConceptMap::OnToolsClicked()
{
  const QPointF cursor_pos_approx(
    m_tools->GetBuddyItem()->GetCenterX(),
    m_tools->GetBuddyItem()->GetCenterY() - 32.0
    - m_tools->GetBuddyItem()->GetRadiusY()
  );
  m_arrow = new QtNewArrow(
    m_tools->GetBuddyItem(),cursor_pos_approx
  );
  assert(!m_arrow->scene());
  this->scene()->addItem(m_arrow);
  m_arrow->update();
  this->scene()->update();
}

void ribi::cmap::QtConceptMap::UpdateSelection()
{
  /*
  for (const auto item: this->scene()->items()) { item->update(); }

  //The QtKeyboardFriendlyGraphicsView may change the selected items
  ConceptMap::ConstEdgesAndNodes edges_and_nodes;
  for (const auto item: this->scene()->selectedItems())
  {
    if (QtEdge* qtedge = dynamic_cast<QtEdge*>(item))
    {
      assert(this->GetConceptMap().HasEdge(qtedge->GetEdge()));
      edges_and_nodes.first.push_back(qtedge->GetEdge()); continue;
    }
    if (QtNode* qtnode = dynamic_cast<QtNode*>(item))
    {
      if(this->GetConceptMap().HasNode(qtnode->GetNode()))
      {
        edges_and_nodes.second.push_back(qtnode->GetNode());
        continue;
      }
      else
      {
        const auto edge = GetConceptMap().GetEdgeHaving(qtnode->GetNode());
        assert(edge);
        edges_and_nodes.first.push_back(edge);
        continue;
      }
    }
  }
  this->m_conceptmap.SetSelected(edges_and_nodes);

  scene()->update();
  */
}
