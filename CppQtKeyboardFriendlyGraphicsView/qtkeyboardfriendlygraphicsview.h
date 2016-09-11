//---------------------------------------------------------------------------
/*
QtKeyboardFriendlyGraphicsView, an keyboard friendly QGraphicsView
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
//From http://www.richelbilderbeek.nl/CppQtKeyboardFriendlyGraphicsView.htm
//---------------------------------------------------------------------------
#ifndef QTKEYBOARDFRIENDLYGRAPHICSVIEW_H
#define QTKEYBOARDFRIENDLYGRAPHICSVIEW_H

#include <vector>
#include <QGraphicsView>

struct QGraphicsItem;

namespace ribi {

///The widget holding the items
struct QtKeyboardFriendlyGraphicsView : public QGraphicsView
{
  QtKeyboardFriendlyGraphicsView(QWidget* parent = 0);

  virtual ~QtKeyboardFriendlyGraphicsView() noexcept {}

  ///Obtain the QGraphicsScene, which is always present
  const QGraphicsScene& GetScene() const noexcept;
        QGraphicsScene& GetScene() noexcept;

  bool GetVerbosity() const noexcept { return m_verbose; }

  ///Respond to a key press
  virtual void keyPressEvent(QKeyEvent *event) override;

  void SetVerbosity(const bool verbosity) noexcept { m_verbose = verbosity; }

  private:
  bool m_verbose;
};

///Obtain the closest item in the collection
///Returns nullptr if there is no focusable item in the items
QGraphicsItem* GetClosest(
  const QGraphicsItem* const focus_item,
  const std::vector<QGraphicsItem *>& items
) noexcept;

///Calculate the Euclidian distance between two points
double GetDistance(const QPointF& a, const QPointF& b);

enum class Direction { above, below, left, right };

QGraphicsItem * GetClosestNonselectedItem(
  const QtKeyboardFriendlyGraphicsView& q,
  const QGraphicsItem* const focus_item,
  int key
);

QGraphicsItem * GetClosestNonselectedItem(
  const QtKeyboardFriendlyGraphicsView& q,
  const QGraphicsItem* const focus_item,
  const Direction direction
);

std::string GetQtKeyboardFriendlyGraphicsViewVersion() noexcept;
std::vector<std::string> GetQtKeyboardFriendlyGraphicsViewVersionHistory() noexcept;

void KeyPressEventCtrl(
  QtKeyboardFriendlyGraphicsView& q,
  QKeyEvent *event
) noexcept;

void KeyPressEventNoModifiers(
  QtKeyboardFriendlyGraphicsView& q,
  QKeyEvent *event
) noexcept;

void KeyPressEventShift(
  QtKeyboardFriendlyGraphicsView& q,
  QKeyEvent *event
) noexcept;

///Give focus to a random item
void SetRandomFocus(
  QtKeyboardFriendlyGraphicsView& q
);

///Give selectedness to one random item
void SetRandomSelectedness(
  QtKeyboardFriendlyGraphicsView& q
);

} //~namespace ribi

#endif // QTKEYBOARDFRIENDLYGRAPHICSVIEW_H
