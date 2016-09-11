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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "qtkeyboardfriendlygraphicsview.h"

#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QGraphicsSimpleTextItem>

#include "container.h"
#include "trace.h"

#pragma GCC diagnostic pop

ribi::QtKeyboardFriendlyGraphicsView::QtKeyboardFriendlyGraphicsView(QWidget* parent)
  : QGraphicsView(new QGraphicsScene,parent),
    m_verbose{false}
{
  assert(scene());
}

QGraphicsItem* ribi::GetClosest(
  const QGraphicsItem* const focus_item,
  const std::vector<QGraphicsItem *>& items) noexcept
{
  if (items.empty()) return nullptr;
  assert(std::count(items.begin(),items.end(),focus_item) == 0);

  QGraphicsItem* best = nullptr;
  double best_distance = std::numeric_limits<double>::max();
  for (QGraphicsItem* const item: items)
  {
    if (!(item->flags() & QGraphicsItem::ItemIsFocusable)) continue;
    if (!(item->isVisible())) continue;

    assert(item != focus_item);
    const double distance = GetDistance(focus_item->pos(),item->pos());
    if (distance < best_distance)
    {
      best_distance = distance;
      best = item;
    }
  }

  assert(best != focus_item);
  return best; //best can be nullptr
}

double ribi::GetDistance(const QPointF& a, const QPointF& b)
{
  const double dx = a.x() - b.x();
  const double dy = a.y() - b.y();
  return std::sqrt((dx * dx) + (dy * dy));
}

QGraphicsItem * ribi::GetClosestNonselectedItem(
  const QtKeyboardFriendlyGraphicsView& q,
  const QGraphicsItem* const focus_item,
  const int key
)
{
  QGraphicsItem * closest_nonselected_item = nullptr;
  switch (key)
  {
    case Qt::Key_Up:
      closest_nonselected_item
        = GetClosestNonselectedItem(q, focus_item, Direction::above);
      break;
    case Qt::Key_Right:
      closest_nonselected_item
        = GetClosestNonselectedItem(q, focus_item, Direction::right);
      break;
    case Qt::Key_Down:
      closest_nonselected_item
        = GetClosestNonselectedItem(q, focus_item, Direction::below);
      break;
    case Qt::Key_Left:
      closest_nonselected_item
        = GetClosestNonselectedItem(q, focus_item, Direction::left);
      break;
  }
  if (!closest_nonselected_item) {
    if (q.GetVerbosity()) {
      TRACE("Did not find a nonselected item");
    }
  }
  else {
    assert(closest_nonselected_item);
    if (q.GetVerbosity()) {
      std::stringstream s;
      s << "Found a nonselected item with tooltip " << closest_nonselected_item->toolTip().toStdString();
      TRACE(s.str());
    }
  }
  return closest_nonselected_item;
}

QGraphicsItem * ribi::GetClosestNonselectedItem(
  const QtKeyboardFriendlyGraphicsView& q,
  const QGraphicsItem* const focus_item,
  const Direction direction
)
{
  const auto my_function_loose_above  = [](const double /* dx */, const double dy) { return dy < 0.0; };
  const auto my_function_strict_above = [](const double dx, const double dy) { return dy < 0.0 && std::abs(dx) < std::abs(dy); };

  const auto my_function_loose_below  = [](const double /* dx */, const double dy) { return dy > 0.0; };
  const auto my_function_strict_below = [](const double dx, const double dy) { return dx > 0.0 && std::abs(dx) < std::abs(dy); };

  const auto my_function_loose_left  = [](const double dx, const double /* dy */) { return dx < 0.0; };
  const auto my_function_strict_left = [](const double dx, const double dy) { return dx < 0.0 && std::abs(dy) < std::abs(dx); };

  const auto my_function_loose_right  = [](const double dx, const double /* dy */) { return dx > 0.0; };
  const auto my_function_strict_right = [](const double dx, const double dy) { return dx > 0.0 && std::abs(dy) < std::abs(dx); };

  using Function = std::function<bool(const double, const double)>;
  Function my_function_loose  = my_function_loose_above;
  Function my_function_strict = my_function_strict_above;

  switch (direction)
  {
    case Direction::above:
      my_function_loose = my_function_loose_above;
      my_function_strict = my_function_strict_above;
      break;
    case Direction::below:
      my_function_loose = my_function_loose_below;
      my_function_strict = my_function_strict_below;
      break;
    case Direction::left:
      my_function_loose = my_function_loose_left;
      my_function_strict = my_function_strict_left;
      break;
    case Direction::right:
      my_function_loose = my_function_loose_right;
      my_function_strict = my_function_strict_right;
      break;
  }

  std::vector<QGraphicsItem *> v;
  const QList<QGraphicsItem *> all_items = q.items();
  std::vector<QGraphicsItem *> nonselected_items;
  std::copy_if(all_items.begin(),all_items.end(),std::back_inserter(nonselected_items),
    [](const QGraphicsItem * const item) { return !item->isSelected(); }
  );
  //Remove the focus item
  nonselected_items.erase(
    std::remove(
      std::begin(nonselected_items),
      std::end(nonselected_items),
      focus_item),
    std::end(nonselected_items)
  );
  assert(std::count(std::begin(nonselected_items),std::end(nonselected_items),focus_item) == 0);
  if (q.GetVerbosity()) {
    std::stringstream s;
    s << "Finding the best item between " << nonselected_items.size() << " non-selected items";
    TRACE(s.str());
  }

  //Look for strict items
  for(QGraphicsItem* const item: nonselected_items)
  {
    const double dx = item->pos().x() - focus_item->pos().x();
    const double dy = item->pos().y() - focus_item->pos().y();
    if (my_function_strict(dx,dy))
    {
      assert(item != focus_item);
      if (!item->isSelected()) { v.push_back(item); }
    }
  }
  if (q.GetVerbosity()) {
    std::stringstream s;
    s << "Using a strict comparison " << v.size() << " non-selected items were found";
    TRACE(s.str());
  }
  //If nothing found, look more loosely
  if (v.empty())
  {

    for(QGraphicsItem* const item: nonselected_items)
    {
      const double dx = item->pos().x() - focus_item->pos().x();
      const double dy = item->pos().y() - focus_item->pos().y();
      if (my_function_loose(dx,dy))
      {
        assert(item != focus_item);
        if (!item->isSelected()) { v.push_back(item); }
      }
    }
    if (q.GetVerbosity())
    {
      std::stringstream s;
      s << "Using a loose comparison " << v.size() << " non-selected items were found";
      TRACE(s.str());
    }

  }
  if (v.empty()) return nullptr;

  if (q.GetVerbosity()) {
    std::stringstream s;
    s << "Selecting the closest out of " << v.size() << " items";
    TRACE(s.str());
  }

  assert(Container().AllUnique(v));
  QGraphicsItem * const closest_item = GetClosest(focus_item,v);
  if (!closest_item) return nullptr;
  assert(closest_item != focus_item);
  assert(!closest_item->isSelected());
  return closest_item;
}

QGraphicsScene& ribi::QtKeyboardFriendlyGraphicsView::GetScene() noexcept
{
  assert(scene());
  return *scene();
}

const QGraphicsScene& ribi::QtKeyboardFriendlyGraphicsView::GetScene() const noexcept
{
  assert(scene());
  return *scene();
}

std::string ribi::GetQtKeyboardFriendlyGraphicsViewVersion() noexcept
{
  return "1.4";
}

std::vector<std::string> ribi::GetQtKeyboardFriendlyGraphicsViewVersionHistory() noexcept
{
  return {
    "2012-12-13: version 1.0: initial version",
    "2012-12-31: version 1.1: improved moving focus",
    "2015-08-24: version 1.2: move item with CTRL, add selected with SHIFT, can move multiple items",
    "2015-09-18: version 1.3: added verbosity",
    "2015-08-16: version 1.4: keyPressEvent may throw"
  };
}

void ribi::QtKeyboardFriendlyGraphicsView::keyPressEvent(QKeyEvent *event)
{
  if (event->modifiers() & Qt::ControlModifier) {
    if (this->GetVerbosity()) { std::clog << "Key event using CTRL" << std::endl; }
    KeyPressEventCtrl(*this, event);
  }
  else if (event->modifiers() & Qt::ShiftModifier) {
    if (this->GetVerbosity()) { std::clog << "Key event using SHIFT" << std::endl; }
    KeyPressEventShift(*this, event);
  }
  else {
    if (this->GetVerbosity()) { std::clog << "Key event without CTRL nor SHIFT" << std::endl; }
    KeyPressEventNoModifiers(*this, event);
  }

}

void ribi::KeyPressEventCtrl(
  QtKeyboardFriendlyGraphicsView& q,
  QKeyEvent *event
) noexcept
{
  //CTRL: Move items
  assert(event->modifiers() & Qt::ControlModifier);

  //Do special movements
  if (event->key() == Qt::Key_Space)
  {
    if (q.GetVerbosity()) { std::clog << "Pressing CTRL-Space" << std::endl; }
    SetRandomSelectedness(q);
    return;
  }


  if (q.GetVerbosity()) { std::clog << "CTRL pressed: try to move items" << std::endl; }
  double delta_x{0.0};
  double delta_y{0.0};
  switch (event->key())
  {
    case Qt::Key_Up:
      if (q.GetVerbosity()) { std::clog << "Moving selected item (s) up" << std::endl; }
      delta_y = -10.0;
      break;
    case Qt::Key_Right:
      if (q.GetVerbosity()) { std::clog << "Moving selected item (s) right" << std::endl; }
      delta_x =  10.0;
      break;
    case Qt::Key_Down:
      if (q.GetVerbosity()) { std::clog << "Moving selected item (s) down" << std::endl; }
      delta_y =  10.0;
      break;
    case Qt::Key_Left:
      if (q.GetVerbosity()) { std::clog << "Moving selected item (s) left" << std::endl; }
      delta_x = -10.0;
      break;
  }
  for (const auto item: q.GetScene().selectedItems())
  {
    assert(item);
    if (!(item->flags() & QGraphicsItem::ItemIsMovable)) { continue; }
    item->setPos(item->pos() + QPointF(delta_x,delta_y));
  }
  q.GetScene().update();
}

void ribi::KeyPressEventNoModifiers(
  QtKeyboardFriendlyGraphicsView& q,
  QKeyEvent *event
) noexcept
{
  assert(!(event->modifiers() & Qt::ShiftModifier));
  assert(!(event->modifiers() & Qt::ControlModifier));

  //Do special movements
  if (event->key() == Qt::Key_Space)
  {
    SetRandomFocus(q); //If you want to select a random item, use CTRL-space
    return;
  }

  QGraphicsItem* const current_focus_item = q.GetScene().focusItem(); //Can be nullptr
  if (!current_focus_item) {
    if (q.GetVerbosity()) { std::clog << "Cannot tranfer selectedness when there is no focus" << std::endl; }
    return;
  }

  const std::set<int> keys = { Qt::Key_Up, Qt::Key_Right, Qt::Key_Left, Qt::Key_Down };
  if (keys.count(event->key()) == 0) {
    if (q.GetVerbosity()) { std::clog << "Do only movements here" << std::endl; }
    return;
  }

  assert(current_focus_item);
  QGraphicsItem * const new_selected_item = GetClosestNonselectedItem(q, current_focus_item,event->key());
  assert(new_selected_item != current_focus_item);

  //Unselect currently selected
  const auto current_selected_items = q.GetScene().selectedItems();
  if (q.GetVerbosity()) { std::clog << "Unselecting " << current_selected_items.size() << " items " << std::endl; }
  for (const auto item: current_selected_items)
  {
    if (q.GetVerbosity()) { std::clog << "Unselect: " << item->toolTip().toStdString() << std::endl; }
    assert(item->isSelected());
    item->setSelected(false);
  }
  assert(!current_focus_item->isSelected() && "Focus item must have lost focus now");
  //Select newly selected
  if (new_selected_item)
  {
    if (q.GetVerbosity()) { std::clog << "Select: " << new_selected_item->toolTip().toStdString() << std::endl; }
    assert(!new_selected_item->isSelected());
    new_selected_item->setSelected(true);
  }
  //Transfer focus
  current_focus_item->clearFocus();
  if (new_selected_item) {
    assert(new_selected_item->isSelected());
    new_selected_item->setFocus();
  }
}

void ribi::KeyPressEventShift(
  QtKeyboardFriendlyGraphicsView& q,
  QKeyEvent *event
) noexcept
{
  const std::set<int> keys_accepted = { Qt::Key_Up, Qt::Key_Right, Qt::Key_Down, Qt::Key_Left };
  if (keys_accepted.count(event->key()) == 0)
  {
    if (q.GetVerbosity())
    {
      std::clog << "SHIFT pressed with unaccepted key" << std::endl;
    }
    return;
  }

  //Add selectedness to items
  assert(event->modifiers() & Qt::ShiftModifier);
  //Can be nullptr
  QGraphicsItem* const current_focus_item = q.GetScene().focusItem();
  if (!current_focus_item)
  {
    if (q.GetVerbosity()) { std::clog << "Cannot add items without a focus" << std::endl; }
    return;
  }

  QGraphicsItem * const new_added_selected_item
    = GetClosestNonselectedItem(q, current_focus_item,event->key())
  ;
  assert(!new_added_selected_item || new_added_selected_item != current_focus_item);

  //Add selectedness
  if (new_added_selected_item)
  {
    if (q.GetVerbosity()) { std::clog << "Add select: " << new_added_selected_item->toolTip().toStdString() << std::endl; }
    assert(new_added_selected_item);
    assert(!new_added_selected_item->isSelected());
    new_added_selected_item->setSelected(true);
  }

  //Transfer focus
  assert(current_focus_item);
  current_focus_item->clearFocus();
  if (new_added_selected_item) { new_added_selected_item->setFocus(); }
  q.GetScene().update();
}

void ribi::SetRandomFocus(
  QtKeyboardFriendlyGraphicsView& q
)
{
  if (QGraphicsItem* const item = q.GetScene().focusItem())
  {
    assert(item);
    if (q.GetVerbosity()) { std::clog << "Removing current focus" << std::endl; }
    //Really lose focus
    item->setEnabled(false);
    //assert(item->isSelected()); //Not true
    item->setSelected(false); // #239
    item->clearFocus();
    item->setEnabled(true);
  }
  else
  {
    if (q.GetVerbosity()) { std::clog << "No focused item to remove focus of" << std::endl; }
  }

  if (q.GetVerbosity()) { std::clog << "Remove selectedness of all " << q.GetScene().selectedItems().size() << " selected items" << std::endl; }
  for (auto item: q.GetScene().selectedItems())
  {
    assert(item->isSelected());
    item->setSelected(false);
  }


  //Let a random item receive focus
  const QList<QGraphicsItem *> all_items = q.items();
  QList<QGraphicsItem *> items;
  std::copy_if(all_items.begin(),all_items.end(),std::back_inserter(items),
    [](const QGraphicsItem* const item)
    {
      return (item->flags() & QGraphicsItem::ItemIsFocusable)
        && item->isVisible();
    }
  );
  if (q.GetVerbosity()) { std::clog << "Obtained " << all_items.size() << " focusable items" << std::endl; }

  if (!items.empty())
  {
    static std::mt19937 rng_engine{0};
    std::uniform_int_distribution<int> distribution(0, static_cast<int>(items.size()) - 1);
    const int i{distribution(rng_engine)};
    assert(i >= 0);
    assert(i < items.size());
    if (q.GetVerbosity()) { std::clog << "Giving the " << i << "th item focus" << std::endl; }
    auto& new_focus_item = items[i];
    assert(!new_focus_item->isSelected());
    new_focus_item->setSelected(true);
    new_focus_item->setFocus();
  }
  else
  {
    if (q.GetVerbosity()) { std::clog << "No focusable items" << std::endl; }
  }
}


void ribi::SetRandomSelectedness(
  QtKeyboardFriendlyGraphicsView& q
)
{
  for (auto item: q.GetScene().items())
  {
    if (item->isSelected()) {
      item->setSelected(false);
    }
  }
  assert(q.GetScene().selectedItems().size() == 0);

  if (q.GetScene().focusItem()) {
    q.GetScene().focusItem()->clearFocus();
  }

  //Choose a random item visible item to receive selectedness
  const auto all_items = q.GetScene().items();
  QList<QGraphicsItem *> items;
  std::copy_if(all_items.begin(),all_items.end(),std::back_inserter(items),
    [](const QGraphicsItem* const item)
    {
      return (item->flags() & QGraphicsItem::ItemIsSelectable)
        && item->isVisible();
    }
  );
  assert(q.GetScene().selectedItems().size() == 0);
  if (!items.empty())
  {
    static std::mt19937 rng_engine{0};
    std::uniform_int_distribution<int> distribution(0, static_cast<int>(items.size()) - 1);
    const int i{distribution(rng_engine)};
    assert(i >= 0);
    assert(i < items.size());
    auto& new_focus_item = items[i];
    assert(!new_focus_item->isSelected());
    new_focus_item->setSelected(true);
    assert(q.GetScene().selectedItems().size() == 1);
  }
}
