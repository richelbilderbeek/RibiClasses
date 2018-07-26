#ifndef QTROUNDEDRECTITEM_H
#define QTROUNDEDRECTITEM_H

#include <iosfwd>
#include <QGraphicsRectItem>
#include <QPen>

namespace ribi {

///Rounded rectangle item
/*

     _______
  A /       \           ______
   /  _____  \         / _____ \
  |  |B    |  |       | |B    | |
  |  |  C  |  |       | |  C  | |
  |  |_____|  |       | |_____| |
   \         /         \_______/
    \_______/

A: Outer rectangle, includes thickest pen width
B: Inner rectangle, excluded thickest pen width
C: The position if the QtRoundedRectItem
If the rectangle is displayed with a thinner pen, A and B remain at the same relative position

QGraphicsRectItem::rect must be of the form (-0.5*width,-0.5*height,0.5*width,0.5*height)
so that (0.0,0.0) is its origin

Comparison of Qt and this class:

   rect():                 GetOuterRect()

        |                        |
        |                        |
     ---+---                  ---+---
    /   |   \                /   |   \
 --+----O----+--          --+----C----+--
    \   |   /                \   |   /
     ---+---                  ---+---
        |                        |
        |                        |


   where rect().center    where GetOuterRect().center
     == QPointF(0.0,0.0)   == GetPos()

   pos():
     (1.2,3.4)


*/
class QtRoundedRectItem : public QGraphicsRectItem
{
  //No need to inherit from Q_OBJECT

  public:
  explicit QtRoundedRectItem(QGraphicsItem *parent = 0);

  virtual ~QtRoundedRectItem() noexcept;

  ///Get the pen by which the contour is drawn
  const QPen& GetContourPen() const noexcept { return m_contour_pen; }

  ///Get the pen by which focus is indicated
  const QPen& GetFocusPen() const noexcept { return m_focus_pen; }

  double GetInnerHeight() const noexcept;

  ///Gets the rectangle within/excluding the pen, use GetRawRect the get the raw QRectF
  QRectF GetInnerRect() const noexcept;
  double GetInnerWidth() const noexcept;

  double GetOuterHeight() const noexcept { return QGraphicsRectItem::rect().height(); }
  ///Gets the rectangle with/including the pen, use GetRawRect the get the raw QRectF
  QRectF GetOuterRect() const noexcept;
  double GetOuterWidth() const noexcept { return QGraphicsRectItem::rect().width(); }


  ///Get the rounded rect corner x radius
  double GetRadiusX() const noexcept{ return m_radius_x; }

  ///Get the rounded rect corner y radius
  double GetRadiusY() const noexcept{ return m_radius_y; }

  void SetCenterPos(const double x,const double y) noexcept { SetCenterX(x); SetCenterY(y); }
  void SetCenterPos(const QPointF& pos) noexcept { SetCenterPos(pos.x(),pos.y()); }
  void SetCenterX(const double x) noexcept;
  void SetCenterY(const double y) noexcept;

  void SetContourPen(const QPen& pen) noexcept; ///Set the pen by which the contours are normally drawn, default value: QPen(Qt::DashLine)
  void SetFocusPen(const QPen& pen) noexcept; ///Set the pen by which focus is indicated, default value: QPen(Qt::DashLine)

  void SetInnerHeight(const double width) noexcept;
  void SetInnerWidth(const double width) noexcept;

  void SetOuterHeight(const double width) noexcept;
  void SetOuterWidth(const double width) noexcept;

  void SetRadiusX(const double radius_x) noexcept;
  void SetRadiusY(const double radius_y) noexcept;

  ///Define a usertype for this QGraphicsItem, must be unique
  enum { Type = UserType + 12 };
  int type() const override
  {
    return Type;
  }

protected:

  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) noexcept override;

  private:
  ///The pen by which the contour is drawn
  QPen m_contour_pen;

  ///The pen by which focus is indicated
  QPen m_focus_pen;

  ///The rounded rect corner x radius
  double m_radius_x;

  ///The rounded rect corner y radius
  double m_radius_y;


  ///To make it private, use GetRect instead
  QRectF rect() = delete; //#244
  ///To make it private, use SetRoundedRect instead
  void setRect(const double,const double,const double,const double) = delete;
  void setRect(const QRectF&) = delete;

};

///Get the current pen, depends on the focus and selectedness of the item
const QPen& GetCurrentPen(const QtRoundedRectItem& r) noexcept;

///Obtain the version of this class
std::string GetQtRoundedRectItemVersion() noexcept;

///Obtain the version history of this class
std::vector<std::string> GetQtRoundedRectItemVersionHistory() noexcept;

///Does this object have focus?
bool HasFocus(const QtRoundedRectItem& r) noexcept;

///Is this object selected?
bool IsSelected(const QtRoundedRectItem& r) noexcept;

std::ostream& operator<<(std::ostream& os,const QtRoundedRectItem& item) noexcept;

bool operator==(const QtRoundedRectItem& lhs, const QtRoundedRectItem& rhs) noexcept;
bool operator!=(const QtRoundedRectItem& lhs, const QtRoundedRectItem& rhs) noexcept;

} //~namespace ribi

#endif // QTROUNDEDRECTITEM_H
