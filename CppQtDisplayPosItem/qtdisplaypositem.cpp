



#include "qtdisplaypositem.h"

#include <cassert>
#include <sstream>



ribi::QtDisplayPosItem::QtDisplayPosItem(QGraphicsItem *parent)
 : QGraphicsSimpleTextItem(parent)
{
  this->setFlags(
      QGraphicsItem::ItemIsFocusable
    | QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
  );
  this->update();
}

std::string ribi::QtDisplayPosItem::GetVersion() noexcept
{
  return "1.1";
}

std::vector<std::string> ribi::QtDisplayPosItem::GetVersionHistory() noexcept
{
  return {
    "2012-12-19: version 1.0: initial version",
    "2013-07-25: version 1.1: transition to Qt5",
    "2016-04-16: version 1.2: removed Boost.Signals2",
  };
}




void ribi::QtDisplayPosItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  QGraphicsSimpleTextItem::mouseMoveEvent(event);
}


void ribi::QtDisplayPosItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  std::stringstream s;
  s << "(" << static_cast<int>(this->pos().x()) << "," << static_cast<int>(this->pos().y()) << ")";
  this->setText(s.str().c_str());
  QGraphicsSimpleTextItem::paint(painter,option,widget);
}
