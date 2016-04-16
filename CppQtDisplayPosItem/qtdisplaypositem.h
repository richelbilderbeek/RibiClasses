//---------------------------------------------------------------------------
/*
QtDisplayPosItem, QGraphicsItem that displays its position
Copyright (C) 2012-2016 Richel Bilderbeek

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
//From http://www.richelbilderbeek.nl/CppQtDisplayPosItem.htm
//---------------------------------------------------------------------------
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

protected:
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

} //~namespace ribi

#endif // QTDISPLAYPOSITEM_H
