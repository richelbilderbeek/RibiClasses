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
#ifndef QTCONCEPTMAPEDGEITEM_H
#define QTCONCEPTMAPEDGEITEM_H

//#include <boost/shared_ptr.hpp>
#include <QGraphicsItem>
#include "qtconceptmapfwd.h"
//#include "conceptmapnode.h"
#include "qtconceptmapqtnode.h"
#include "conceptmapedge.h"

namespace ribi {

struct QtRoundedRectItem;

namespace cmap {

struct Edge;
struct Node;

///The QtEdge is a QGraphicsItem that
///draws a curve underneath itself, between head and tail arrowhead
///It parents an arrow and a QtNode.
struct QtEdge : public QGraphicsItem
{
  using Base = QGraphicsItem;
  using Arrow = QtQuadBezierArrowItem *;
  using ReadOnlyArrow = const QtQuadBezierArrowItem *;
  using ReadOnlyNodePtr = boost::shared_ptr<const Node>;
  using QtNodePtr =  QtNode *;
  using ReadOnlyQtNodePtr = const QtNode *;

  using From = QtNodePtr;
  using ReadOnlyFrom = ReadOnlyQtNodePtr;
  using To = QtNodePtr;
  using ReadOnlyTo = ReadOnlyQtNodePtr;

  QtEdge(
    const Edge& edge,
    QtNode * const from,
    QtNode * const to
  );
  QtEdge(const QtEdge&) = delete;
  QtEdge& operator=(const QtEdge&) = delete;
  ~QtEdge() noexcept;

  QRectF boundingRect() const override final;

  void DisableAll() noexcept;
  void EnableAll() noexcept;

  ReadOnlyArrow GetArrow() const noexcept { return m_arrow; }
  const Arrow& GetArrow() noexcept { return m_arrow; }

  const Edge& GetEdge() const noexcept { return m_edge; }
        Edge& GetEdge()       noexcept { return m_edge; }

  ///The node item the arrow originates from
  ReadOnlyFrom GetFrom() const noexcept { return m_from; }
  From GetFrom() noexcept { return m_from; }

  ///The node item the arrow targets
  ReadOnlyTo GetTo() const noexcept { return m_to; }
  To GetTo() noexcept { return m_to; }

        QtNode * GetQtNode()       noexcept { return m_qtnode; }
  const QtNode * GetQtNode() const noexcept { return m_qtnode; }

  static std::string GetVersion() noexcept;
  static std::vector<std::string> GetVersionHistory() noexcept;

  bool HasHeadArrow() noexcept;
  bool HasTailArrow() noexcept;

  bool isSelected() const;

  void SetEdge(const Edge& edge) noexcept;


  void SetHasHeadArrow(const bool has_head_arrow) noexcept;
  void SetHasTailArrow(const bool has_tail_arrow) noexcept;

  void SetSelected(bool selected);

  void SetShowBoundingRect(const bool show_bounding_rect) const noexcept { m_show_bounding_rect = show_bounding_rect; }


  std::string ToStr() const noexcept;

  void keyPressEvent(QKeyEvent *event) noexcept override final;

protected:
  void focusInEvent(QFocusEvent *event) noexcept override final;
  void focusOutEvent(QFocusEvent *event) noexcept override final;
  void mousePressEvent(QGraphicsSceneMouseEvent *event) noexcept override final;
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) noexcept override final;
  QPainterPath shape() const noexcept override final;

private:
  ///The arrow used for drawing, deleted by this class
  Arrow m_arrow;

  ///The edge
  Edge m_edge;

  ///The node item the arrow originates from
  QtNode * const m_from;

  ///The QtNode around Edge its Node
  QtNode * const m_qtnode;

  ///Show the bounding rectangle
  mutable bool m_show_bounding_rect;

  ///The node item the arrow targets
  QtNode * const m_to;

  ///Called whenever the edge changes
  void OnEdgeChanged(const Edge &edge) noexcept;
  void OnConceptChanged(Node * const node) noexcept;
  void OnNodeChanged(const Edge& edge) noexcept;
  void OnNodePosChanged(QtRoundedRectItem * const node) noexcept;
  void OnTextChanged(QtRoundedEditRectItem* item) noexcept;

  void OnMustUpdateScene();
  void OnRequestSceneUpdate();

  void setSelected(bool selected) = delete;
};

std::ostream& operator<<(std::ostream& os, const QtEdge& qtedge) noexcept;

bool operator==(const QtEdge& lhs, const QtEdge& rhs) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPEDGEITEM_H
