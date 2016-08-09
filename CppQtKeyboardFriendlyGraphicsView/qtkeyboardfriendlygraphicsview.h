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

#include <random>
#include <QGraphicsView>

struct QGraphicsItem;

namespace ribi {

///The widget holding the items
struct QtKeyboardFriendlyGraphicsView : public QGraphicsView
{
  QtKeyboardFriendlyGraphicsView(QWidget* parent = 0);

  ///QtKeyboardFriendlyGraphicsView creates its own QGraphicsScene
  QtKeyboardFriendlyGraphicsView(QGraphicsScene* scene, QWidget* parent) = delete;

  virtual ~QtKeyboardFriendlyGraphicsView() noexcept {}

  bool GetVerbosity() const noexcept { return m_verbose; }

  ///Obtain the version of this class
  static std::string GetVersion() noexcept;

  ///Obtain the version history of this class
  static std::vector<std::string> GetVersionHistory() noexcept;

  ///Respond to a key press
  virtual void keyPressEvent(QKeyEvent *event) noexcept;

  void SetVerbosity(const bool verbosity) noexcept { m_verbose = verbosity; }

  private:

  ///The RNG engine. When a user presses space, a random item is chosen
  std::mt19937 m_rng_engine;
  bool m_verbose;

  ///Obtain the closest item in the collection
  ///Returns nullptr if there is no focusable item in the items
  QGraphicsItem* GetClosest(
    const QGraphicsItem* const focus_item,
    const std::vector<QGraphicsItem *>& items
  ) const noexcept;

  ///Calculate the Euclidian distance between two points
  static double GetDistance(const QPointF& a, const QPointF& b);

  enum class Direction { above, below, left, right };
  QGraphicsItem * GetClosestNonselectedItem(
    const QGraphicsItem* const focus_item,
    int key
  ) const;

  QGraphicsItem * GetClosestNonselectedItem(
    const QGraphicsItem* const focus_item,
    const Direction direction
  ) const;

  void KeyPressEventCtrl(QKeyEvent *event) noexcept;
  void KeyPressEventNoModifiers(QKeyEvent *event) noexcept;
  void KeyPressEventShift(QKeyEvent *event) noexcept;

  ///Give focus to a random item
  void SetRandomFocus();

  ///Give selectedness to one random item
  void SetRandomSelectedness();
};

} //~namespace ribi

#endif // QTKEYBOARDFRIENDLYGRAPHICSVIEW_H
