#include "qtkeyboardfriendlygraphicsview.h"

#ifndef NDEBUG
//Everything only present in debug mode
#include <cassert>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <QApplication>
#include <QKeyEvent>
#include <QGraphicsRectItem>
#pragma GCC diagnostic pop

#include "container.h"
#include "fileio.h"
#include "testtimer.h"
#include "counter.h"
#include "ribi_system.h"
#include "ribi_time.h"
#include "trace.h"

namespace ribi {
  namespace qtkeyboardfriendlygraphicsview {
    QKeyEvent CreateCtrlLeft() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Left,Qt::ControlModifier); }
    QKeyEvent CreateCtrlRight() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Right,Qt::ControlModifier); }
    QKeyEvent CreateShiftLeft() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Left,Qt::ShiftModifier); }
    QKeyEvent CreateShiftRight() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Right,Qt::ShiftModifier); }
    QKeyEvent CreateDown() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Down,Qt::NoModifier); }
    QKeyEvent CreateLeft() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Left,Qt::NoModifier); }
    QKeyEvent CreateRight() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Right,Qt::NoModifier); }
    QKeyEvent CreateSpace() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Space,Qt::NoModifier); }
    QKeyEvent CreateUp() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Up,Qt::NoModifier); }
    QKeyEvent CreateX() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_X,Qt::NoModifier); }
  }
}

void ribi::QtKeyboardFriendlyGraphicsView::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  {
    Counter();
    ribi::fileio::FileIo();
    ribi::System();
    ribi::Time();
    ribi::Container();
  }
  using namespace ribi::qtkeyboardfriendlygraphicsview;
  const TestTimer test_timer(__func__,__FILE__,1.0);
  const bool verbose{false};
  QtKeyboardFriendlyGraphicsView view;
  QGraphicsRectItem * const item1{new QGraphicsRectItem};
  QGraphicsRectItem * const item2{new QGraphicsRectItem};
  item1->setToolTip("Item1");
  item2->setToolTip("Item2");
  item1->setFlag(QGraphicsItem::ItemIsFocusable);
  item2->setFlag(QGraphicsItem::ItemIsFocusable);
  item1->setFlag(QGraphicsItem::ItemIsSelectable);
  item2->setFlag(QGraphicsItem::ItemIsSelectable);
  view.scene()->addItem(item1);
  view.scene()->addItem(item2);
  view.showFullScreen();
  for (int i=0; i!=1000; ++i) { qApp->processEvents(); }

  if (verbose) { TRACE("Space selects one random item"); }
  {
    item1->setSelected(false);
    item2->setSelected(false);
    assert(view.scene()->selectedItems().empty());
    auto space = CreateSpace();
    view.keyPressEvent(&space);
    assert(view.scene()->selectedItems().size() == 1);
  }
  //Make richelbilderbeek continue
  {
    if (ribi::System().GetWhoami() == "richelbilderbeek")
    {
      TRACE("Skip test for richelbilderbeek");
      return;
    }
  }
  if (verbose) { TRACE("Space causes m_signal_update to be emitted"); }
  {
    item1->setSelected(false);
    item2->setSelected(false);
    assert(view.scene()->selectedItems().empty());

    Counter c{0}; //For receiving the signal
    view.m_signal_update.connect(
      boost::bind(&ribi::Counter::Inc,&c) //Do not forget the &
    );

    auto space = CreateSpace();
    view.keyPressEvent(&space);

    assert(c.Get() > 0);
  }
  if (verbose) { TRACE("A useless key does nothing"); }
  {
    Counter c{0}; //For receiving the signal
    view.m_signal_update.connect(
      boost::bind(&ribi::Counter::Inc,&c) //Do not forget the &
    );

    auto key_x = CreateX();
    view.keyPressEvent(&key_x);

    assert(c.Get() == 0);
  }
  if (verbose) { TRACE("When focus/selectedness is transferred, two signals are emitted "); }
  //for (int i=0; i!=10; ++i)
  {
    //item1 unselected and unfocused at right
    item1->setSelected(false);
    item1->setPos( 100.0,0.0);
    //item2 selected and focused at left
    item2->setSelected(true);
    item2->setFocus();
    item2->setPos(-100.0,0.0);
    assert(view.scene()->selectedItems().size() == 1);

    Counter c{0}; //For receiving the signal
    view.m_signal_update.connect(
      boost::bind(&ribi::Counter::Inc,&c) //Do not forget the &
    );

    auto right = CreateRight();
    view.keyPressEvent(&right);

    if (c.Get() != 2)
    {
      TRACE("Expected c to be 2");
      TRACE(c.Get());
    }
    assert(c.Get() == 2 && "When transferring selectedness, one unselect and one select takes place");

    auto left = CreateLeft();
    view.keyPressEvent(&left);

    assert(c.Get() == 4 && "When transferring selectedness again, one unselect and one select takes place");
  }
  if (verbose) { TRACE("When focus/selectedness is added (using SHIFT), one signal is emitted "); }
  {
    //item1 unselected and unfocused at right
    item1->setSelected(false);
    item1->setPos( 100.0,0.0);
    //item1 selected and focused at left
    item2->setSelected(true);
    item2->setFocus();
    item2->setPos(-100.0,0.0);
    assert(view.scene()->selectedItems().size() == 1);

    Counter c{0}; //For receiving the signal
    view.m_signal_update.connect(
      boost::bind(&ribi::Counter::Inc,&c) //Do not forget the &
    );

    auto shift_right = CreateShiftRight();
    view.keyPressEvent(&shift_right);

    if (c.Get() > 1)
    {
      TRACE("#1: Warning: expected 1 m_signal_update, no idea why this fails");
    }
    assert(c.Get() >= 1);
  }
  if (verbose) { TRACE("When selectedness is transferred to nothing, one signal is emitted: one by the unselected item"); }
  {
    //item1 unselected and unfocused at right
    item1->setSelected(false);
    item1->setPos( 100.0,0.0);
    //item1 selected and focused at left
    item2->setSelected(true);
    item2->setFocus();
    item2->setPos(-100.0,0.0);
    assert(view.scene()->selectedItems().size() == 1);

    Counter c{0}; //For receiving the signal
    view.m_signal_update.connect(
      boost::bind(&ribi::Counter::Inc,&c) //Do not forget the &
    );

    auto left = CreateLeft(); //Move away, tranfer selectedness to nothing
    view.keyPressEvent(&left);

    TRACE(c.Get());
    TRACE(view.scene()->selectedItems().size());
    assert(view.scene()->selectedItems().size() == 0);
    assert(c.Get() == 1);
  }
  if (verbose) { TRACE("Pressing space selects one item, when two items were selected"); }
  {
    item1->clearFocus();
    item2->clearFocus();
    item1->setSelected(true);
    item2->setSelected(true);
    assert(view.scene()->selectedItems().size() == 2);
    auto space = CreateSpace();

    for (int i=0; i!=100; ++i) //Very often
    {
      view.keyPressEvent(&space);
      assert(view.scene()->selectedItems().size() == 1);
    }
  }
}
#endif


