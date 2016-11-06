#ifndef QTARROWITEM_H
#define QTARROWITEM_H

#include <string>
#include <vector>
#include <QGraphicsLineItem>
#include <QPen>

namespace ribi {

///An arrow QGraphicsItem
//From http://www.richelbilderbeek.nl/CppQtArrowItem.htm
struct QtArrowItem : public QGraphicsLineItem
{
  typedef QtArrowItem This;

  QtArrowItem(
    const double x1,
    const double y1,
    const bool tail,
    const double x2,
    const double y2,
    const bool head,
    QGraphicsItem* parent = 0
  ) noexcept;

  virtual ~QtArrowItem() noexcept {}

  ///Obtain the version of this class
  static std::string GetVersion() noexcept;

  ///Obtain the version history of this class
  static std::vector<std::string> GetVersionHistory() noexcept;

  ///Is there an arrow at the 'to' point (x2,y2)?
  bool HasHead() const noexcept { return m_head; }

  ///Is there an arrow at the 'from' point (x1,y1)?
  bool HasTail() const noexcept { return m_tail; }

  ///Respond to key presses
  void keyPressEvent(QKeyEvent *event);

  ///Set the distance in pixels the user is allowed to be distance from the
  ///arrow head/tail to change it
  void SetArrowHeadClickingDistance(const double manhattan_distance) noexcept;

  ///Set the pen used to show focus
  void SetFocusPen(const QPen& pen) noexcept { m_focus_pen = pen; }

  ///Set if the arrow has a point at the head
  void SetHasHead(const bool has_head) noexcept { m_head = has_head; }

  ///Set if the arrow has a point at the tail
  void SetHasTail(const bool has_tail) noexcept { m_tail = has_tail; }

  ///Set the position of the head
  void SetHeadPos(const double x, const double y) noexcept;

  ///Set the regular pen used to draw the arrow
  void SetPen(const QPen& pen) noexcept { m_pen = pen; }

  ///Set the position of the tail
  void SetTailPos(const double x, const double y) noexcept;

protected:

  QRectF boundingRect() const;
  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
  virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem *, QWidget *);
  QPainterPath shape() const;

private:

  ///Set the distance in pixels the user is allowed to be distance from the
  ///arrow head/tail to change it
  double m_arrow_head_clicking_distance;

  ///The extra width given to the line for easier clicking
  static const double m_click_easy_width;

  ///The arrow used for indicating focus
  QPen m_focus_pen;

  ///Show arrow at head
  bool m_head;

  ///The regular pen
  QPen m_pen;

  ///Show arrow at tail
  bool m_tail;
};

} //~namespace ribi

#endif // QTARROWITEM_H
