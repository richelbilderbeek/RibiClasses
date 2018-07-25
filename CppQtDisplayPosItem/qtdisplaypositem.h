#ifndef QTDISPLAYPOSITEM_H
#define QTDISPLAYPOSITEM_H

#include <string>
#include <vector>

#include <QGraphicsSimpleTextItem>

namespace ribi {

///A text item that displays its coordinats
struct QtDisplayPosItem : public QGraphicsSimpleTextItem
{
  QtDisplayPosItem(QGraphicsItem *parent = 0);

  ///Obtain the version of this class
  static std::string GetVersion() noexcept;

  ///Obtain the version history of this class
  static std::vector<std::string> GetVersionHistory() noexcept;

  ///Define a usertype for this QGraphicsItem, must be unique
  enum { Type = UserType + 8 };
  int type() const override
  {
    return Type;
  }

protected:
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

} //~namespace ribi

#endif // QTDISPLAYPOSITEM_H
