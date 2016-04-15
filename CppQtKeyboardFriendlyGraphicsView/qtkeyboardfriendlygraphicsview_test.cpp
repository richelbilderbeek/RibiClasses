#include "qtkeyboardfriendlygraphicsview_test.h"
#include "qtkeyboardfriendlygraphicsview.h"

#include <QApplication>
#include <QKeyEvent>
#include <QGraphicsRectItem>
#include "container.h"
#include "fileio.h"
#include "testtimer.h"
#include "counter.h"
#include "ribi_system.h"
#include "ribi_time.h"
#include "trace.h"

void ribi::qtkeyboardfriendlygraphicsview_test::all()
{
  using namespace ribi;
  bool verbose{false};
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
  view.show();
  view.setGeometry(0,0,300,300);

  for (int i=0; i!=1000; ++i) { qApp->processEvents(); }

  if (verbose) { TRACE("Space selects one random item"); }
  {
    item1->setSelected(false);
    item2->setSelected(false);
    QVERIFY(view.scene()->selectedItems().empty());
    QTest::keyClick(&view, Qt::Key_Space, Qt::NoModifier, 100);
    QVERIFY(view.scene()->selectedItems().size() == 1);
  }
  if (verbose) { TRACE("Space causes m_signal_update to be emitted"); }
  {
    item1->setSelected(false);
    item2->setSelected(false);
    QVERIFY(view.scene()->selectedItems().empty());

    Counter c{0}; //For receiving the signal
    view.m_signal_update.connect(
      boost::bind(&ribi::Counter::Inc,&c) //Do not forget the &
    );

    QTest::keyClick(&view, Qt::Key_Space, Qt::NoModifier, 100);

    QVERIFY(c.Get() > 0);
    view.m_signal_update.disconnect(
      boost::bind(&ribi::Counter::Inc,&c) //Do not forget the &
    );
  }
  if (verbose) { TRACE("A useless key does nothing"); }
  {
    Counter c{0}; //For receiving the signal
    view.m_signal_update.connect(
      boost::bind(&ribi::Counter::Inc,&c) //Do not forget the &
    );

    QTest::keyClick(&view, Qt::Key_X, Qt::NoModifier, 100);

    QVERIFY(c.Get() == 0);
    view.m_signal_update.disconnect(
      boost::bind(&ribi::Counter::Inc,&c) //Do not forget the &
    );
  }
  if (verbose) { TRACE("When focus/selectedness is transferred, two signals are emitted "); }
  {
    //item1 unselected and unfocused at right
    item1->setSelected(false);
    item1->setPos( 100.0,0.0);
    //item2 selected and focused at left
    item2->setSelected(true);
    item2->setFocus();
    item2->setPos(-100.0,0.0);
    QVERIFY(view.scene()->items().size() == 2);
    QVERIFY(view.scene()->selectedItems().size() == 1);

    Counter c{0}; //For receiving the signal
    view.m_signal_update.connect(
      boost::bind(&ribi::Counter::Inc,&c) //Do not forget the &
    );
    view.SetVerbosity(verbose);
    c.SetVerbosity(verbose);
    QTest::keyClick(&view, Qt::Key_Right, Qt::NoModifier, 100);

    QVERIFY(c.Get() == 2 && "When transferring selectedness, one unselect and one select takes place");
    QTest::keyClick(&view, Qt::Key_Left, Qt::NoModifier, 100);

    QVERIFY(c.Get() == 4 && "When transferring selectedness again, one unselect and one select takes place");
    view.m_signal_update.disconnect(
      boost::bind(&ribi::Counter::Inc,&c) //Do not forget the &
    );
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
    QVERIFY(view.scene()->selectedItems().size() == 1);

    Counter c{0}; //For receiving the signal

    c.SetVerbosity(verbose);
    view.m_signal_update.connect(
      boost::bind(&ribi::Counter::Inc,&c) //Do not forget the &
    );

    view.SetVerbosity(verbose);
    QTest::keyClick(&view, Qt::Key_Right, Qt::ShiftModifier, 100);

    QVERIFY(c.Get() == 1);
    view.m_signal_update.disconnect(
      boost::bind(&ribi::Counter::Inc,&c) //Do not forget the &
    );
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
    QVERIFY(view.scene()->selectedItems().size() == 1);

    Counter c{0}; //For receiving the signal
    view.m_signal_update.connect(
      boost::bind(&ribi::Counter::Inc,&c) //Do not forget the &
    );

    QTest::keyClick(&view, Qt::Key_Left, Qt::NoModifier, 100);

    QVERIFY(view.scene()->selectedItems().size() == 0);
    QVERIFY(c.Get() == 1);
    view.m_signal_update.disconnect(
      boost::bind(&ribi::Counter::Inc,&c) //Do not forget the &
    );
  }
  if (verbose) { TRACE("Pressing space selects one item, when two items were selected"); }
  {
    view.SetVerbosity(false);
    item1->clearFocus();
    item2->clearFocus();
    item1->setSelected(true);
    item2->setSelected(true);
    QVERIFY(view.scene()->selectedItems().size() == 2);
    for (int i=0; i!=100; ++i) //Very often
    {
      QTest::keyClick(&view, Qt::Key_Space, Qt::NoModifier, 100);
      QVERIFY(view.scene()->selectedItems().size() == 1);
    }
  }
  if (verbose) { TRACE("Send random keys to widget"); }
  {
    item1->setPos(150,150);
    item2->setPos(50,50);
    QVERIFY(view.scene());
    QVERIFY(!view.scene()->items().empty());
    for (int i=0; i!=100; ++i)
    {
      switch (std::rand() % 9)
      {
        case 0: QTest::keyClick(&view, Qt::Key_Left, Qt::ControlModifier, 100); break;
        case 1: QTest::keyClick(&view, Qt::Key_Right, Qt::ControlModifier, 100); break;
        case 2: QTest::keyClick(&view, Qt::Key_Space, Qt::NoModifier, 100); break;
        case 3: QTest::keyClick(&view, Qt::Key_Left, Qt::ShiftModifier, 100); break;
        case 4: QTest::keyClick(&view, Qt::Key_Right, Qt::ShiftModifier, 100); break;
        case 5: QTest::keyClick(&view, Qt::Key_Left, Qt::NoModifier, 100); break;
        case 6: QTest::keyClick(&view, Qt::Key_Right, Qt::NoModifier, 100); break;
        case 7: QTest::keyClick(&view, Qt::Key_Space, Qt::NoModifier, 100); break;
        case 8: QTest::keyClick(&view, Qt::Key_X, Qt::NoModifier, 100); break;
      }
    }
  }
  if (verbose) { TRACE("Tapping SHIFT must not remove focus"); }
  {
    //item1 unselected and unfocused at right
    item1->setSelected(false);
    item1->setPos( 100.0,0.0);
    //item2 selected and focused at left
    item2->setSelected(true);
    item2->setFocus();
    item2->setPos(-100.0,0.0);
    QVERIFY(item2->hasFocus());

    view.SetVerbosity(verbose);
    QTest::keyClick(&view, Qt::Key_Shift, Qt::NoModifier, 100);

    assert(item2->hasFocus());
  }
}
