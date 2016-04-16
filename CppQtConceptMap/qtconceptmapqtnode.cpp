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
#include "qtconceptmapqtnode.h"

#include <cassert>
#include <climits>

//#include <boost/lambda/lambda.hpp>

#include <QCursor>
#include <QKeyEvent>
#include <QPainter>
#include <QPen>

#include "conceptmapconcept.h"
#include "conceptmapexamples.h"
#include "conceptmapnodefactory.h"
#include "conceptmapnode.h"
#include "container.h"
#include "qtconceptmapbrushfactory.h"
#include "qtconceptmapeditstrategy.h"
#include "qtconceptmapqtnodefactory.h"
#include "qtconceptmapratestrategy.h"
#include "qtitemdisplaystrategy.h"
#include "trace.h"

ribi::cmap::QtNode::QtNode(const Node& node)
  : QtRoundedEditRectItem(),
    //m_signal_base_changed{},
    //m_signal_key_down_pressed{},
    //m_signal_node_changed{},
    m_node{node},
    m_show_bounding_rect{false}
{
  //Allow mouse tracking
  this->setAcceptHoverEvents(true);

  this->SetPadding(Base::Padding(1.0,6.0,1.0,2.0));

  this->setAcceptHoverEvents(true);
  this->setFlags(
      QGraphicsItem::ItemIsFocusable
    | QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
  );

  SetNode(node);

  //m_signal_pos_changed.connect(
  //  boost::bind(&ribi::cmap::QtNode::OnPosChanged,this,boost::lambda::_1)
  //);

  //this->m_signal_text_changed.connect(
  //  boost::bind(&ribi::cmap::QtNode::OnTextChanged,this,boost::lambda::_1)
  //);
}

ribi::cmap::QtNode::~QtNode() noexcept
{

}

void ribi::cmap::QtNode::DisableAll()
{
  this->setEnabled(false);
  this->setVisible(false);
}

void ribi::cmap::QtNode::EnableAll()
{
  this->setEnabled(true);
  this->setVisible(true);
}

void ribi::cmap::QtNode::focusInEvent(QFocusEvent* e) noexcept
{
  QtRoundedEditRectItem::focusInEvent(e);
  //this->m_signal_selected_changed(this);
  assert(hasFocus());
}

void ribi::cmap::QtNode::focusOutEvent(QFocusEvent* e) noexcept
{
  QtRoundedEditRectItem::focusOutEvent(e);
  //this->m_signal_selected_changed(this);
  assert(!hasFocus());
}

void ribi::cmap::QtNode::hoverMoveEvent(QGraphicsSceneHoverEvent*) noexcept
{
  this->setCursor(QCursor(Qt::PointingHandCursor));
  //m_concept_item->hoverMoveEvent(e);
  //Won't cause a hover, because the concept item
  //is not visible??
}


void ribi::cmap::QtNode::keyPressEvent(QKeyEvent *event) noexcept
{
  //m_signal_key_down_pressed(this,event->key());
  Base::keyPressEvent(event);
}


void ribi::cmap::QtNode::OnConceptChanged(Node * const node) noexcept
{
  //Node changed, sync QtRoundedRectItem
  assert(node);
  assert(*node == m_node);
  const std::string new_str = node->GetConcept().GetName();
  const std::vector<std::string> new_text{new_str};
  assert(new_text.size() == 1);
  this->SetText(new_text);
  assert(GetText() == new_text);
}

void ribi::cmap::QtNode::OnPosChanged(const QtRoundedRectItem * const item) noexcept
{
  //QtRoundedRectItem changed, sync Node
  assert(item);
  const auto new_pos = item->GetCenterPos();
  m_node.SetPos(new_pos.x(),new_pos.y());
}

void ribi::cmap::QtNode::OnTextChanged(const QtRoundedRectItem * const
#ifndef NDEBUG
  item
#endif
) noexcept
{
  //QtRoundedRectItem changed, sync Node
  assert(item);
  assert(item == this);
  const std::vector<std::string> new_text{GetText()};
  assert((new_text.size() == 1 || new_text.size() != 1)
    && "new_text can be of any number of lines, as QtRoundedEditRect supports this"
  );
  const std::string s{Container().Concatenate(new_text)};
  assert(std::count(std::begin(s),std::end(s),'\n') ==  0 && "Keep it single-line");
  m_node.GetConcept().SetName(s);
}


void ribi::cmap::QtNode::OnXchanged(Node * const node) noexcept
{
  const bool verbose{false};
  if (verbose) { TRACE("Slot ribi::cmap::QtNode::OnXchanged"); }

  //Node changed, sync QtRoundedRectItem
  assert(node);
  SetCenterX(node->GetX());
}

void ribi::cmap::QtNode::OnYchanged(Node * const node) noexcept
{
  //Node changed, sync QtRoundedRectItem
  assert(node);
  SetCenterY(node->GetY());
}


//void ribi::cmap::QtNode::OnRequestsSceneUpdate()
//{
  //this->m_signal_request_scene_update();
//}

void ribi::cmap::QtNode::paint(
  QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget
) noexcept
{
  Base::paint(painter,item,widget);

  if (!GetNode().GetConcept().GetExamples().Get().empty())
  {
    //painter->setBrush(m_display_strategy->GetIndicatorBrush());
    //painter->setPen(m_display_strategy->GetIndicatorPen());
    painter->setBrush(Qt::transparent);
    painter->setPen(Qt::black);
    //Draw indicator that a concept has examples in it
    painter->drawRect(
      GetInnerRect().right() - 5.0,
      GetInnerRect().top() + 3.0,
      3.0,
      3.0
      );
  }

  //Check if item can move (as the center node cannot)
  #define BRAINWEAVER_MOVE_ITEMS_ON_COLLISION
  #ifdef BRAINWEAVER_MOVE_ITEMS_ON_COLLISION
  if (this->flags() & QGraphicsItem::ItemIsMovable)
  {
    //Item can move, check for collision
    const QList<QGraphicsItem*> others = collidingItems();
    std::for_each(others.begin(),others.end(),
      [this](const QGraphicsItem* const other_item)
      {
        assert(other_item);
        if (const QtNode* const other_node = dynamic_cast<const QtNode*>(other_item))
        {
          const double dx = x() - other_node->x() > 0.0 ? 1.0 : -1.0;
          const double dy = y() - other_node->y() > 0.0 ? 1.0 : -1.0;
          //assert(this->flags() & QGraphicsItem::ItemIsMovable); //Not true for center node
          this->SetCenterPos( this->x()  + dx, this->y()  + dy);
        }
      }
    );
  }
  #endif
  if (m_show_bounding_rect)
  {
    const QPen prev_pen = painter->pen();
    const QBrush prev_brush = painter->brush();
    painter->setPen(QPen(QColor(0,0,96)));
    painter->setBrush(QBrush(QColor(0,0,255,64)));
    painter->drawRect(this->boundingRect().adjusted(1.0,1.0,-1.0,-1.0));
    painter->setPen(prev_pen);
    painter->setBrush(prev_brush);
  }
}

void ribi::cmap::QtNode::SetNode(const Node& node) noexcept
{
  m_node = node;
  this->SetCenterPos(m_node.GetX(), m_node.GetY());
  this->SetText( { node.GetConcept().GetName() } );
}

std::string ribi::cmap::QtNode::ToStr() const noexcept
{
  std::stringstream s;
  s << (*this);
  return s.str();
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const QtNode& qtnode) noexcept
{
  os
    << qtnode.GetNode()
  ;
  return os;
}
