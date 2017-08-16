#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtquadbezierarrowitem.h"

#include <cassert>
#include <cmath>

#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry.hpp>
#include <boost/math/constants/constants.hpp>

#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QGraphicsView>
#include <QApplication>
#include <QTimer>

#include "geometry.h"
#include "grabber.h"
// 
#include "qtroundededitrectitem.h"
#include "qtroundedrectitem.h"
#pragma GCC diagnostic pop

const double ribi::QtQuadBezierArrowItem::m_click_easy_width = 10.0;

ribi::QtQuadBezierArrowItem::QtQuadBezierArrowItem(
  QGraphicsItem* const from,
  const bool tail,
  QGraphicsItem* const mid,
  const bool head,
  QGraphicsItem* const to,
  QGraphicsItem* parent
) noexcept
  : QGraphicsItem(parent),
    m_focus_pen{QPen(Qt::DashLine)},
    m_from{from},
    m_head{head},
    m_mid{mid},
    m_pen{QPen(QColor(0,0,0))},
    m_show_bounding_rect{false},
    m_tail{tail},
    m_to{to}
    // m_verbose{false}
{
  assert(from);
  assert(to);
  assert((mid || !mid) && "No mid results in a straight arrow");
  assert(from != to);
  assert(from != mid);
  assert(mid != to);
  this->setFlags(
      QGraphicsItem::ItemIsFocusable
    | QGraphicsItem::ItemIsSelectable
  );

  assert(!(flags() & QGraphicsItem::ItemIsMovable) );
  assert( (flags() & QGraphicsItem::ItemIsSelectable) );

  this->setAcceptHoverEvents(true);

  //Put this arrow item under the source and target node
  this->setZValue(std::min(from->zValue(),to->zValue()) - 1.0);
  //Put the center item above the arrow
  if (mid)
  {
    mid->setZValue((from->zValue() + to->zValue()) / 2.0);
  }
}

QRectF ribi::QtQuadBezierArrowItem::boundingRect() const
{
  assert(this->scene());
  return shape().boundingRect();
}

QPointF ribi::QtQuadBezierArrowItem::GetBeyond() const noexcept
{
  const QPointF p_center = GetCenter();
  const double dx_mid_center = GetMidItem() ? (GetMidItem()->pos().x() - p_center.x()) : 0.0;
  const double dy_mid_center = GetMidItem() ? (GetMidItem()->pos().y() - p_center.y()) : 0.0;
  const QPointF p_beyond(p_center.x() + dx_mid_center + dx_mid_center, p_center.y() + dy_mid_center + dy_mid_center);

  //const double dx_mid_center = GetMidItem() ? (GetMidItem()->pos().x() - center.x()) : 0.0;
  //const double dy_mid_center = GetMidItem() ? (GetMidItem()->pos().y() - center.y()) : 0.0;
  //const QPointF beyond(center.x() + dx_mid_center + dx_mid_center, center.y() + dy_mid_center + dy_mid_center);
  return p_beyond;
}

QPointF ribi::QtQuadBezierArrowItem::GetCenter() const noexcept
{
  //DON'T: will result in recursion
  //const QPointF center((GetTail() + GetHead()) / 2.0);
  const QPointF center( (GetToItem()->pos() + GetFromItem()->pos()) / 2.0);
  return center;
}

const QGraphicsItem* ribi::QtQuadBezierArrowItem::GetFromItem() const noexcept
{
  assert(m_from);
  return m_from;
}

QPointF ribi::QtQuadBezierArrowItem::GetHead() const noexcept
{
  typedef boost::geometry::model::d2::point_xy<double> Point;
  typedef boost::geometry::model::linestring<Point> Line;
  typedef boost::geometry::model::box<Point> Rect;

  const QPointF beyond = GetBeyond();

  const Line line_head = Geometry().CreateLine(
    std::vector<Point>(
      {
        Point(beyond.x(),beyond.y()),
        Point(m_to->pos().x(),m_to->pos().y()),
      }
    )
  );
  const QRectF qr_to = m_to->boundingRect().translated(m_to->pos());

  const Rect r_to(
    Point(qr_to.topLeft().x()    ,qr_to.topLeft().y()    ),
    Point(qr_to.bottomRight().x(),qr_to.bottomRight().y())
    );

  std::vector<Point> p_head_end = Geometry().GetLineRectIntersections(line_head,r_to);

  if (p_head_end.size() == 1)
  {
    return QPointF(p_head_end[0].x(),p_head_end[0].y());
  }
  if (p_head_end.empty())
  {
    p_head_end.push_back(Point(m_to->pos().x(),m_to->pos().y()));
    //Yes,it happens, when the line does not leave the rectangle
    //this happens when the two node rectanges overlap
    assert(!p_head_end.empty());
    assert(p_head_end.size() == 1); ///BUG? claudio does not think this is a bug:
                                    ///one element is added two lines above
    return QPointF(p_head_end[0].x(),p_head_end[0].y());
  }
  else
  {
    assert(p_head_end.size() == 2);
    //Choose point closest to beyond
    const double d1 = Geometry().GetDistance(beyond.x(),beyond.y(),p_head_end[0].x(),p_head_end[0].y());
    const double d2 = Geometry().GetDistance(beyond.x(),beyond.y(),p_head_end[1].x(),p_head_end[1].y());
    if (d1 <= d2)
    {
      return QPointF(p_head_end[0].x(),p_head_end[0].y());
    }
    else
    {
      return QPointF(p_head_end[1].x(),p_head_end[1].y());
    }
  }
}

const QGraphicsItem* ribi::QtQuadBezierArrowItem::GetMidItem() const noexcept
{
  assert(m_mid);
  return m_mid;
}

QGraphicsItem* ribi::QtQuadBezierArrowItem::GetMidItem() noexcept
{
  assert(m_mid);
  return m_mid;
}

QPointF ribi::QtQuadBezierArrowItem::GetTail() const noexcept
{

  typedef boost::geometry::model::d2::point_xy<double> Point;
  typedef boost::geometry::model::linestring<Point> Line;
  typedef boost::geometry::model::box<Point> Rect;

  const QPointF beyond = GetBeyond();

  const Line line_tail = Geometry().CreateLine(
    std::vector<Point>(
      {
        Point(m_from->pos().x(),m_from->pos().y()),
        Point(beyond.x(),beyond.y()),
      }
    )
  );

  const QRectF qr_from = m_from->boundingRect().translated(m_from->pos());

  const Rect r_from(
    Point(qr_from.topLeft().x()    ,qr_from.topLeft().y()    ),
    Point(qr_from.bottomRight().x(),qr_from.bottomRight().y())
    );

  std::vector<Point> p_tail_end = Geometry().GetLineRectIntersections(line_tail,r_from);

  if (p_tail_end.size() == 1)
  {
    return QPointF(p_tail_end[0].x(),p_tail_end[0].y());
  }
  if (p_tail_end.empty())
  {
    p_tail_end.push_back(Point(m_from->pos().x(),m_from->pos().y()));
    //Yes,it happens, when the line does not leave the rectangle
    //this happens when the two node rectanges overlap
    return m_from->pos();
  }
  else
  {
    assert(p_tail_end.size() == 2);
    //Choose point closest to beyond
    const double d1 = Geometry().GetDistance(beyond.x(),beyond.y(),p_tail_end[0].x(),p_tail_end[0].y());
    const double d2 = Geometry().GetDistance(beyond.x(),beyond.y(),p_tail_end[1].x(),p_tail_end[1].y());
    if (d1 <= d2)
    {
      return QPointF(p_tail_end[0].x(),p_tail_end[0].y());
    }
    else
    {
      return QPointF(p_tail_end[1].x(),p_tail_end[1].y());
    }
  }
}

const QGraphicsItem* ribi::QtQuadBezierArrowItem::GetToItem() const noexcept
{
  assert(m_to);
  return m_to;
}

std::string ribi::QtQuadBezierArrowItem::GetVersion() noexcept
{
  return "1.5";
}

std::vector<std::string> ribi::QtQuadBezierArrowItem::GetVersionHistory() noexcept
{
  return {
    "2012-12-07: version 1.0: initial version",
    "2012-12-13: version 1.1: respond to focus",
    "2012-12-29: version 1.2: fixed bug in GetHead and GetTail that occurs when GetLineRectIntersections returns two points",
    "2013-01-01: version 1.3: added QGraphicsItem getters",
    "2013-07-10: version 1.4: setting arrow heads emits a notification signal",
    "2014-03-18: version 1.5: allow arrow to be straight"
  };
}

void ribi::QtQuadBezierArrowItem::hoverEnterEvent(QGraphicsSceneHoverEvent *) noexcept
{
  this->setCursor(QCursor(Qt::PointingHandCursor));
}

void ribi::QtQuadBezierArrowItem::hoverMoveEvent(QGraphicsSceneHoverEvent *) noexcept
{
  this->setCursor(QCursor(Qt::PointingHandCursor));
}

void ribi::QtQuadBezierArrowItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *) noexcept
{
  this->setCursor(QCursor(Qt::PointingHandCursor));
}

void ribi::QtQuadBezierArrowItem::keyPressEvent(QKeyEvent *event) noexcept
{
  switch (event->key())
  {
    case Qt::Key_F1:
    case Qt::Key_1:
    case Qt::Key_T:
    case Qt::Key_Minus:
      SetHasTail(!HasTail());
      return;
    case Qt::Key_F2:
    case Qt::Key_2:
    case Qt::Key_H:
    case Qt::Key_Plus:
      SetHasHead(!HasHead());
      return;
    default:
      break;
  }
  QGraphicsItem::keyPressEvent(event);
}

void ribi::QtQuadBezierArrowItem::mousePressEvent(QGraphicsSceneMouseEvent *event) noexcept
{
  if (event->modifiers() & Qt::ShiftModifier)
  {
    if ((event->pos() - this->m_from->pos()).manhattanLength() < 10.0)
    {
      SetHasTail(!HasTail());
    }
    else if ((event->pos() - this->m_to->pos()).manhattanLength() < 10.0)
    {
      SetHasHead(!HasHead());
    }
  }
  QGraphicsItem::mousePressEvent(event);
}


void ribi::QtQuadBezierArrowItem::paint(QPainter* painter, const QStyleOptionGraphicsItem *, QWidget *) noexcept
{
  painter->setRenderHint(QPainter::Antialiasing);

  if (this->isSelected() || this->hasFocus())
  {
    painter->setPen(m_focus_pen);
  }
  else
  {
    painter->setPen(m_pen);
  }

  //Line must go _through_ mid pos, instead of using it as a virtual hinge point
  //Solution:
  // - define point 'between' as the middle between from and to
  // - point 'center' is the center point of the item there
  // - define point 'beyond' as the mirror point of 'center', using mid_pos as a mirror

  const QPointF p_end_head{GetHead()};
  const QPointF p_end_tail{GetTail()};
  const QPointF p_beyond{GetBeyond()};
  //const QPointF p_center{GetCenter()};
  QPainterPath curve;
  curve.moveTo(p_end_tail);

  if (GetMidItem() && GetMidItem()->isVisible())
  {
    //Curve through midpoint
    painter->drawEllipse(GetMidItem()->pos(),1,1);
    curve.quadTo(p_beyond,p_end_head);
  }
  else
  {
    //Straight line
    curve.lineTo(p_end_head);
    #ifdef REALLY_THINK_IT_IS_A_GOOD_IDEA_TO_MOVE_CHILDREN_20170815
    //Keep middle item between tail and head
    if (GetMidItem()) {
      GetMidItem()->setX((GetHead().x() + GetTail().x()) / 2.0);
      GetMidItem()->setY((GetHead().y() + GetTail().y()) / 2.0);
    }
    #endif // REALLY_THINK_IT_IS_A_GOOD_IDEA_TO_MOVE_CHILDREN_20170815
  }
  painter->drawPath(curve);


  {
    const double sz = 10.0; //pixels
    if (m_tail)
    {
      const double pi{boost::math::constants::pi<double>()};
      const double dx{p_end_tail.x() - p_beyond.x()};
      const double dy{(p_end_tail.y() - p_beyond.y())};
      double angle1{0.6*pi - Geometry().GetAngleClockScreen(-dx,-dy)};
      double angle2{0.4*pi - Geometry().GetAngleClockScreen(-dx,-dy)};

      const QPointF p0(p_end_tail.x(),p_end_tail.y());
      const QPointF p1
        = p0 + QPointF(
           std::cos(angle1) * sz,
           -std::sin(angle1) * sz);
      const QPointF p2
        = p0 + QPointF(
           std::cos(angle2) * sz,
           -std::sin(angle2) * sz);

      painter->drawPolygon(QPolygonF() << p0 << p1 << p2);
    }
    if (m_head)
    {
      const double pi{boost::math::constants::pi<double>()};
      const double dx{p_end_head.x() - p_beyond.x()};
      const double dy{(p_end_head.y() - p_beyond.y())};
      double angle1{0.6*pi - Geometry().GetAngleClockScreen(-dx,-dy)};
      double angle2{0.4*pi - Geometry().GetAngleClockScreen(-dx,-dy)};

      const QPointF p0(p_end_head.x(),p_end_head.y());
      const QPointF p1
        = p0 + QPointF( 
           std::cos(angle1) * sz,
           -std::sin(angle1) * sz);
      const QPointF p2
        = p0 + QPointF(
           std::cos(angle2) * sz,
           -std::sin(angle2) * sz);

      painter->drawPolygon(QPolygonF() << p0 << p1 << p2);
    }
  }

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

void ribi::QtQuadBezierArrowItem::SetFocusPen(const QPen& pen) noexcept
{
  if (m_focus_pen != pen)
  {
    m_focus_pen = pen;
    this->update();
  }
}

void ribi::QtQuadBezierArrowItem::SetFromPos(const QPointF& pos) noexcept
{
  assert(m_from);
  if (m_from->pos() != pos)
  {
    m_from->setPos(pos);
    this->update();
  }
}

void ribi::QtQuadBezierArrowItem::SetHasHead(const bool has_head) noexcept
{

  if (m_head != has_head)
  {
    m_head = has_head;
    this->update();
  }
}

void ribi::QtQuadBezierArrowItem::SetHasTail(const bool has_tail) noexcept
{
  if (m_tail != has_tail)
  {
    m_tail = has_tail;
    this->update();
  }
}


void ribi::QtQuadBezierArrowItem::SetMidPos(const QPointF& pos) noexcept
{
  assert(m_mid);
  if (m_mid->pos() != pos)
  {
    m_mid->setPos(pos);
    this->update();
  }
}


void ribi::QtQuadBezierArrowItem::SetPen(const QPen& pen) noexcept
{
  if (m_pen != pen)
  {
    m_pen = pen;
    this->update();
  }
}


void ribi::QtQuadBezierArrowItem::SetToPos(const QPointF& pos) noexcept
{
  assert(m_to);
  if (m_to->pos() != pos)
  {
    m_to->setPos(pos);
    this->update();
  }
}

QPainterPath ribi::QtQuadBezierArrowItem::shape() const noexcept
{
  assert(this->scene());
  const QPointF p_end_tail = GetHead();
  const QPointF p_end_head = GetTail();

  const QPointF p_center((p_end_tail + p_end_head) / 2.0);
  const double dx_mid_center = GetMidItem() ? (GetMidItem()->pos().x() - p_center.x()) : 0.0;
  const double dy_mid_center = GetMidItem() ? (GetMidItem()->pos().y() - p_center.y()) : 0.0;
  const QPointF p_beyond(p_center.x() + dx_mid_center + dx_mid_center, p_center.y() + dy_mid_center + dy_mid_center);

  QPainterPath curve;
  curve.moveTo(p_end_tail);
  curve.quadTo(p_beyond,p_end_head);

  QPainterPathStroker stroker;
  stroker.setWidth(m_click_easy_width);
  return stroker.createStroke(curve);
}

std::ostream& ribi::operator<<(std::ostream& os, const QtQuadBezierArrowItem& arrow) noexcept
{
  os
    << '(' << arrow.m_from->x() << ',' << arrow.m_from->y() << ')'
    << (arrow.m_tail ? '<' : '-') << "-- "
    << '(' << arrow.m_mid->x() << ',' << arrow.m_mid->y() << ')'
    << " --" << (arrow.m_head ? '>' : '-') << ' '
    << '(' << arrow.m_to->x() << ',' << arrow.m_to->y() << ')'
  ;
  return os;
}

bool ribi::operator==(const QtQuadBezierArrowItem& lhs, const QtQuadBezierArrowItem& rhs) noexcept
{
  return
       lhs.GetFocusPen() == rhs.GetFocusPen()
    && lhs.GetFromItem() == rhs.GetFromItem()
    && lhs.HasHead() == rhs.HasHead()
    && lhs.GetMidItem() == rhs.GetMidItem()
    && lhs.GetPen() == rhs.GetPen()
    && lhs.HasTail() == rhs.HasTail()
    && lhs.GetToItem() == rhs.GetToItem()
  ;
}
