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
#ifndef QTCONCEPTMAPNODEITEM_H
#define QTCONCEPTMAPNODEITEM_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/shared_ptr.hpp>
#include "qtroundededitrectitem.h"
#include "conceptmapnode.h"
#include "conceptmapfwd.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///QtNode displays a Node as a QtConceptMapElement
struct QtNode : public QtRoundedEditRectItem
{
  typedef QtRoundedEditRectItem Base;
  virtual ~QtNode() noexcept;

  ///Node cannot be const as it contains a Concept that the user might want to edit
  ///concept_item contains the Stategy to display (and respond to the concept)
  explicit QtNode(const Node& node);

  virtual QRectF boundingRect() const { return Base::boundingRect(); }

  void DisableAll();
  void EnableAll();

  const Node& GetNode() const noexcept { return m_node; }
        Node& GetNode()       noexcept { return m_node; }

  void SetNode(const Node& node) noexcept;

  std::string ToStr() const noexcept;

  ///m_signal_request_rate_node is emitted due to a m_signal_request_rate_node
  ///of the Node its QtRateConceptItem
  //boost::signals2::signal<void (QtNode *)> m_signal_display_changed;
  //mutable boost::signals2::signal<void (QtNode *)> m_signal_base_changed;

  ///The QtNode is selected or unselected
  //mutable boost::signals2::signal<void (QtNode *,const int key)> m_signal_key_down_pressed;
  //mutable boost::signals2::signal<void (QtNode *)> m_signal_node_changed;

  virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem *, QWidget *) noexcept final;

protected:

  void focusInEvent(QFocusEvent *event) noexcept final override;
  void focusOutEvent(QFocusEvent *event) noexcept final override;
  void keyPressEvent(QKeyEvent *event) noexcept final;
  void hoverMoveEvent(QGraphicsSceneHoverEvent *event) noexcept final;

private:

  ///The node being edited, or displayed and not changed, or rated
  Node m_node;

  bool m_show_bounding_rect;

  void OnPosChanged(const QtRoundedRectItem * const item) noexcept;
  void OnTextChanged(const QtRoundedRectItem * const item) noexcept;
  void OnXchanged(Node * const node) noexcept;
  void OnYchanged(Node * const node) noexcept;
  void OnConceptChanged(Node * const node) noexcept;
};

std::ostream& operator<<(std::ostream& os, const QtNode& qtnode) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPNODEITEM_H
