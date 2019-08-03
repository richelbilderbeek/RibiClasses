#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"

#include "qtroundedrectitem.h"

#include <cassert>
#include <cmath>
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QKeyEvent>
// 
#pragma GCC diagnostic pop

ribi::QtRoundedRectItem::QtRoundedRectItem(QGraphicsItem *parent)
 : QGraphicsRectItem(parent),
   m_contour_pen(QPen(QColor(0,0,0),0.0)),
   m_focus_pen(QPen(QColor(0,0,0),0.0,Qt::DashLine)),
   m_radius_x(4.0),
   m_radius_y(4.0)
{
  this->setFlags(
      QGraphicsItem::ItemIsFocusable
    | QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
    );
  this->SetContourPen(QPen(QColor(0,0,0),2.0));
  assert(this->GetContourPen().isSolid());
  assert(this->GetContourPen().style() != Qt::DashLine);
  this->SetFocusPen(QPen(QColor(0,0,0),3.0,Qt::DashLine));
  assert(this->GetFocusPen().style() == Qt::DashLine);
  //assert(!this->GetFocusPen().isSolid()); //Fails?
  const double height = 32.0;
  const double width = 64.0;
  this->SetInnerWidth(width);
  this->SetInnerHeight(height);
  this->setBrush(Qt::white); //Makes the rectangle opaque; otherwise it would be transparent
}

ribi::QtRoundedRectItem::~QtRoundedRectItem() noexcept
{
  //OK
}

const QPen& ribi::GetCurrentPen(const QtRoundedRectItem& r) noexcept
{
  return (IsSelected(r) || HasFocus(r)) ? r.GetFocusPen() : r.GetContourPen();
}

double ribi::QtRoundedRectItem::GetInnerHeight() const noexcept
{
  const double pen_width = GetFocusPen().widthF() > GetContourPen().widthF()
                         ? GetFocusPen().widthF()
                         : GetContourPen().widthF();
  return GetOuterHeight() - (2.0 * pen_width);
}

QRectF ribi::QtRoundedRectItem::GetInnerRect() const noexcept
{
  return QRectF(
    pos().x(),
    pos().y(),
    GetInnerWidth(),
    GetInnerHeight()
  );
}

double ribi::QtRoundedRectItem::GetInnerWidth() const noexcept
{
  const double pen_width = GetFocusPen().widthF() > GetContourPen().widthF()
                         ? GetFocusPen().widthF()
                         : GetContourPen().widthF();
  return GetOuterWidth() - (2.0 * pen_width);
}


QRectF ribi::QtRoundedRectItem::GetOuterRect() const noexcept
{
  const QRectF r{QGraphicsRectItem::rect()};
  return r.translated(pos().x(),pos().y());
}

std::string ribi::GetQtRoundedRectItemVersion() noexcept
{
  return "1.11";
}

std::vector<std::string> ribi::GetQtRoundedRectItemVersionHistory() noexcept
{
  return {
    "2012-12-13: version 1.0: initial version",
    "2012-12-19: version 1.1: added use of pen, brush and focus-indicating pen",
    "2012-12-22: version 1.2: correctly uses the focus and regular pen, added contour pen",
    "2014-06-14: version 1.3: removed superfluous signal",
    "2014-06-14: version 1.4: fixed issue #219",
    "2014-06-16: version 1.5: disallow setRect and setPos (use SetRoundedRect and SetPos instead), cooperation with QtRoundedRectItemDialog",
    "2014-06-22: version 1.6: allow setting the inner and outer rectangle",
    "2014-08-07: version 1.7: renamed IncludingPen member functions to Outer",
    "2014-08-08: version 1.8: removed using with rectangles from interface, as it led to incorrectness and confusion",
    "2014-08-09: version 1.9: increased use of TDD, fixed bug",
    "2015-02-08: version 1.10: gaining/losing focus for different pen widths work correctly",
    "2015-08-21: version 1.11: rectangles are opaque and white by default"
  };
}

bool ribi::HasFocus(const QtRoundedRectItem& r) noexcept
{
  return r.hasFocus();
}

bool ribi::IsSelected(const QtRoundedRectItem& r) noexcept
{
  return r.isSelected();
}

void ribi::QtRoundedRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  QGraphicsRectItem::mouseMoveEvent(event);
  this->update();
  this->scene()->update();
}

void ribi::QtRoundedRectItem::paint(
  QPainter *painter,
  const QStyleOptionGraphicsItem *,
  QWidget *
)
{
  painter->setBrush(brush());

  const QPen thickest_pen{
    GetContourPen().widthF() > GetFocusPen().widthF()
    ? GetContourPen()
    : GetFocusPen()
  };

  if (this->isSelected() || this->hasFocus())
  {
    painter->setPen(m_focus_pen);
    assert(painter->pen() == m_focus_pen);
  }
  else
  {
    painter->setPen(m_contour_pen);
    assert(painter->pen() == m_contour_pen);
  }
  const double w{GetOuterWidth() - (2 * thickest_pen.widthF()) + GetCurrentPen(*this).widthF()};
  const double h{GetOuterHeight() - (2 * thickest_pen.widthF()) + GetCurrentPen(*this).widthF()};

  painter->drawRoundedRect(
    QRectF(
      -0.5 * w,
      -0.5 * h,
       1.0 * w, //Width
       1.0 * h //Height
    ),
    m_radius_x,m_radius_y
  );
}

void ribi::QtRoundedRectItem::SetCenterX(const double x) noexcept
{
  const auto current_pos = this->pos();
  QGraphicsRectItem::setPos(
    x,
    current_pos.y()
  );
  this->update();
}

void ribi::QtRoundedRectItem::SetCenterY(const double y) noexcept
{
  const auto current_pos = this->pos();
  QGraphicsRectItem::setPos(
    current_pos.x(),
    y
  );
  this->update();
}

void ribi::QtRoundedRectItem::SetContourPen(const QPen& pen) noexcept
{
  m_contour_pen = pen;
  //this->update();
  assert(std::abs(pen.widthF() - GetContourPen().widthF()) < 0.01);
}

void ribi::QtRoundedRectItem::SetFocusPen(const QPen& pen) noexcept
{
  m_focus_pen = pen;
  //this->update();
  assert(std::abs(pen.widthF() - GetFocusPen().widthF()) < 0.01);
}

void ribi::QtRoundedRectItem::SetInnerHeight(const double height) noexcept
{
  const double pen_width = GetFocusPen().widthF() > GetContourPen().widthF()
                         ? GetFocusPen().widthF()
                         : GetContourPen().widthF();
  SetOuterHeight(
    height + (2.0 * pen_width)
  );
}

void ribi::QtRoundedRectItem::SetInnerWidth(const double width) noexcept
{
  const double pen_width = GetFocusPen().widthF() > GetContourPen().widthF()
                         ? GetFocusPen().widthF()
                         : GetContourPen().widthF();
  SetOuterWidth(
    width + (2.0 * pen_width)
  );
}


void ribi::QtRoundedRectItem::SetOuterHeight(const double height) noexcept
{
  if (height != GetOuterHeight())
  {
    const auto w = GetOuterWidth();
    const auto h = height;
    QGraphicsRectItem::setRect(
      QRectF(
        QPointF(-0.5 * w,-0.5 * h),
        QPointF( 0.5 * w, 0.5 * h)
      )
    );
    this->update();
  }
}

void ribi::QtRoundedRectItem::SetOuterWidth(const double width) noexcept
{
  if (width != GetOuterWidth())
  {
    const auto w = width;
    const auto h = GetOuterHeight();
    QGraphicsRectItem::setRect(
      QRectF(
        QPointF(-0.5 * w,-0.5 * h),
        QPointF( 0.5 * w, 0.5 * h)
      )
    );
    this->update();
  }
}

void ribi::QtRoundedRectItem::SetRadiusX(const double radius_x) noexcept
{
  m_radius_x = radius_x;
  this->update();
  assert(radius_x == GetRadiusX());
}

void ribi::QtRoundedRectItem::SetRadiusY(const double radius_y) noexcept
{
  m_radius_y = radius_y;
  assert(radius_y == GetRadiusY());
}

bool ribi::operator==(const QtRoundedRectItem& lhs, const QtRoundedRectItem& rhs) noexcept
{
  return
       lhs.GetContourPen() == rhs.GetContourPen()
    && lhs.GetFocusPen() == rhs.GetFocusPen()
    && lhs.GetRadiusX() == rhs.GetRadiusX()
    && lhs.GetRadiusY() == rhs.GetRadiusY()
    && lhs.GetInnerWidth() == rhs.GetInnerWidth()
    && lhs.GetInnerHeight() == rhs.GetInnerHeight()
  ;
}

bool ribi::operator!=(const QtRoundedRectItem& lhs, const QtRoundedRectItem& rhs) noexcept
{
  return !(lhs == rhs);
}
