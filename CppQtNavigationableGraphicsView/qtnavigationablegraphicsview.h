//---------------------------------------------------------------------------
/*
QtNavigationableGraphicsView, navigationable QGraphicsView
Copyright (C) 2014-2015 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppQtNavigationableGraphicsView.htm
//---------------------------------------------------------------------------
#ifndef QTNAVIGATIONABLEGRAPHICSVIEW_H
#define QTNAVIGATIONABLEGRAPHICSVIEW_H





#include <QGraphicsView>


struct QWheelEvent;

namespace ribi {

class QtNavigationableGraphicsView : public QGraphicsView
{
  Q_OBJECT

  public:
  QtNavigationableGraphicsView(QWidget *parent = 0);
  QtNavigationableGraphicsView(const QtNavigationableGraphicsView&) = delete;
  QtNavigationableGraphicsView& operator=(const QtNavigationableGraphicsView&) = delete;

  void wheelEvent(QWheelEvent *event) override;
};

} //~namespace ribi

#endif // QTNAVIGATIONABLEGRAPHICSVIEW_H
