#include "qtkeyboardfriendlygraphicsview_test.h"
#include "qtkeyboardfriendlygraphicsview.h"

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

  if (verbose) { TRACE("Space selects one random item"); }
  {
    item1->setSelected(false);
    item2->setSelected(false);
    QVERIFY(view.scene()->selectedItems().empty());
    QTest::keyClick(&view, Qt::Key_Space, Qt::NoModifier, 100);
    QVERIFY(view.scene()->selectedItems().size() == 1);
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
    QTest::keyClick(&view, Qt::Key_Left, Qt::NoModifier, 100);
    QVERIFY(view.scene()->selectedItems().size() == 0);
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
  if (1 == 2) //TODO
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
      view.update();
    }
  }
  if (verbose) { TRACE("Tapping SHIFT must not remove focus"); }
  if (1==2) //No idea how to do this non-randomly for now
  {
    //item1 unselected and unfocused at right
    item1->setSelected(false);
    item1->setPos( 100.0,0.0);
    //item2 selected and focused at left
    item2->setSelected(true);
    item2->setFocus();
    item2->setPos(-100.0,0.0);
    qApp->processEvents();
    QVERIFY(item2->hasFocus());
    view.SetVerbosity(verbose);
    QTest::keyClick(&view, Qt::Key_Shift, Qt::NoModifier, 100);
    qApp->processEvents();
    QVERIFY(item2->hasFocus());
  }
}
