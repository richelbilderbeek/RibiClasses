#include "qtarrowitem.h"

#include <cassert>
#include <cmath>

#include <boost/math/constants/constants.hpp>

#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QPainter>

#include "geometry.h"

const double ribi::QtArrowItem::m_click_easy_width = 10.0;

ribi::QtArrowItem::QtArrowItem(
  const double x1,
  const double y1,
  const bool tail,
  const double x2,
  const double y2,
  const bool head,
  QGraphicsItem* parent
) noexcept
  : QGraphicsLineItem(x1,y1,x2,y2,parent),
    m_arrow_head_clicking_distance(20.0),
    m_focus_pen(QPen(Qt::DashLine)),
    m_head(head),
    m_pen(QPen(QColor(0,0,0))),
    m_tail(tail)
{
  this->setFlags(
      QGraphicsItem::ItemIsFocusable
    | QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
  );

  //Allow mouse tracking
  this->setAcceptHoverEvents(true);

  assert(this->line().p1() == QPointF(x1,y1));
  assert(this->line().p2() == QPointF(x2,y2));
}

ribi::QtArrowItem::~QtArrowItem() noexcept
{

}

QRectF ribi::QtArrowItem::boundingRect() const
{
  return shape().boundingRect();
}

std::string ribi::QtArrowItem::GetVersion() noexcept
{
  return "1.2";
}

std::vector<std::string> ribi::QtArrowItem::GetVersionHistory() noexcept
{
  return {
    "2012-11-18: version 1.0: initial version",
    "2012-11-20: version 1.1: mouse cursor changes its shape when moving over this item",
    "2012-12-19: version 1.2: allow changing pens"
  };
}

void ribi::QtArrowItem::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
  this->setCursor(QCursor(Qt::PointingHandCursor));
}

void ribi::QtArrowItem::keyPressEvent(QKeyEvent *event)
{
  switch (event->key())
  {
    case Qt::Key_F1:
    case Qt::Key_1:
    case Qt::Key_T:
    case Qt::Key_Minus:
      m_tail = !m_tail;
      this->update();
      break;
    case Qt::Key_F2:
    case Qt::Key_2:
    case Qt::Key_H:
    case Qt::Key_Plus:
      m_head = !m_head;
      this->update();
      break;
    default:
      break;
  }
  QGraphicsLineItem::keyPressEvent(event);
}

void ribi::QtArrowItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  QGraphicsLineItem::mouseMoveEvent(event);
}

void ribi::QtArrowItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  if (event->modifiers() & Qt::ShiftModifier)
  {
    if ((event->pos() - this->line().p1()).manhattanLength() < m_arrow_head_clicking_distance)
    {
      m_tail = !m_tail;
      this->update();
    }
    else if ((event->pos() - this->line().p2()).manhattanLength() < m_arrow_head_clicking_distance)
    {
      m_head = !m_head;
      this->update();
    }
  }
}

void ribi::QtArrowItem::paint(QPainter* painter, const QStyleOptionGraphicsItem *, QWidget *)
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
  painter->drawLine(this->line());

  const double pi{boost::math::constants::pi<double>()};

  //The angle from tail to head
  double angle{Geometry().GetAngleClockScreen(line().dx(),line().dy())};
  const double sz{10.0}; //pixels
  if (m_tail)
  {
    const QPointF p0{this->line().p1()};
    const QPointF p1{
      p0 + QPointF(
         std::sin(angle + pi + (pi * 0.1)) * sz,
        -std::cos(angle + pi + (pi * 0.1)) * sz)};
    const QPointF p2
      = p0 + QPointF(
         std::sin(angle + pi - (pi * 0.1)) * sz,
        -std::cos(angle + pi - (pi * 0.1)) * sz);
    painter->drawPolygon(QPolygonF() << p0 << p1 << p2);
  }
  if (m_head)
  {
    const QPointF p0 = this->line().p2();

    const QPointF p1{
      p0 + QPointF(
         std::sin(angle + pi + (pi * 0.1)) * sz,
        -std::cos(angle + pi + (pi * 0.1)) * sz
      )
    };
    const QPointF p2{
      p0 + QPointF(
         std::sin(angle + pi - (pi * 0.1)) * sz,
        -std::cos(angle + pi - (pi * 0.1)) * sz
      )
    };

    painter->drawPolygon(QPolygonF() << p0 << p1 << p2);
  }
}

void ribi::QtArrowItem::SetArrowHeadClickingDistance(const double manhattan_distance) noexcept
{
  m_arrow_head_clicking_distance = manhattan_distance;
}

void ribi::QtArrowItem::SetHeadPos(const double x, const double y) noexcept
{
  if (line().x2() != x || line().y2() != y)
  {
    QLineF line = this->line();
    line.setP2(QPointF(x,y));
    this->setLine(line);
    this->update();
  }
}

void ribi::QtArrowItem::SetTailPos(const double x, const double y) noexcept
{
  if (line().x1() != x || line().y1() != y)
  {
    QLineF line = this->line();
    line.setP1(QPointF(x,y));
    this->setLine(line);
    this->update();
  }
}

QPainterPath ribi::QtArrowItem::shape() const
{
  assert(this->scene());
  //Thanks to norobro for posting this code at
  //http://www.qtcentre.org/threads/49201-Increase-margin-for-detecting-tooltip-events-of-QGraphicsLineItem
  QPainterPath path;
  QPainterPathStroker stroker;
  path.moveTo(line().p1());
  path.lineTo(line().p2());
  stroker.setWidth(m_click_easy_width);
  return stroker.createStroke(path);
}
