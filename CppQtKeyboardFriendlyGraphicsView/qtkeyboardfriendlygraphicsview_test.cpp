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

void ribi::QtKeyboardFriendlyGraphicsViewTest::cleanup()
{
  delete m_view;
  delete m_item1;
  delete m_item2;
}

void ribi::QtKeyboardFriendlyGraphicsViewTest::init()
{
  m_view = new QtKeyboardFriendlyGraphicsView;
  m_item1 = new QGraphicsRectItem;
  m_item2 = new QGraphicsRectItem;
  m_item1->setToolTip("Item1");
  m_item2->setToolTip("Item2");
  m_item1->setFlag(QGraphicsItem::ItemIsFocusable);
  m_item2->setFlag(QGraphicsItem::ItemIsFocusable);
  m_item1->setFlag(QGraphicsItem::ItemIsSelectable);
  m_item2->setFlag(QGraphicsItem::ItemIsSelectable);
  m_view->scene()->addItem(m_item1);
  m_view->scene()->addItem(m_item2);
  m_item1->setRect(-100,0,50,50);
  m_item2->setRect( 100,0,50,50);
  m_view->showFullScreen();
}

void ribi::QtKeyboardFriendlyGraphicsViewTest::SelectNothing()
{
  // When selectedness is transferred to nothing,
  // one signal is emitted: one by the unselected item

  //item1 unselected and unfocused at right
  m_item1->setSelected(false);
  m_item1->setPos( 100.0,0.0);
  //item1 selected and focused at left
  m_item2->setSelected(true);
  m_item2->setFocus();
  m_item2->setPos(-100.0,0.0);
  QVERIFY(m_view->scene()->selectedItems().size() == 1);
  QTest::keyClick(m_view, Qt::Key_Left, Qt::NoModifier, 100);
  QVERIFY(m_view->scene()->selectedItems().size() == 0);
}

void ribi::QtKeyboardFriendlyGraphicsViewTest::SpaceSelectsOneRandomItemWhenTwoAreSelected()
{
  // Pressing space selects one item, when two items were selected
  m_view->SetVerbosity(false);
  m_item1->clearFocus();
  m_item2->clearFocus();
  m_item1->setSelected(true);
  m_item2->setSelected(true);
  QVERIFY(m_view->scene()->selectedItems().size() == 2);
  for (int i=0; i!=100; ++i) //Very often
  {
    QTest::keyClick(m_view, Qt::Key_Space, Qt::NoModifier, 10);
    QVERIFY(m_view->scene()->selectedItems().size() == 1);
    m_view->show();
  }
}

void ribi::QtKeyboardFriendlyGraphicsViewTest::SendRandomKeys()
{
  return; //TODO
  m_item1->setPos(150,150);
  m_item2->setPos(50,50);
  QVERIFY(m_view->scene());
  QVERIFY(!m_view->scene()->items().empty());
  for (int i=0; i!=100; ++i)
  {
    switch (std::rand() % 9)
    {
      case 0: QTest::keyClick(m_view, Qt::Key_Left, Qt::ControlModifier, 100); break;
      case 1: QTest::keyClick(m_view, Qt::Key_Right, Qt::ControlModifier, 100); break;
      case 2: QTest::keyClick(m_view, Qt::Key_Space, Qt::NoModifier, 100); break;
      case 3: QTest::keyClick(m_view, Qt::Key_Left, Qt::ShiftModifier, 100); break;
      case 4: QTest::keyClick(m_view, Qt::Key_Right, Qt::ShiftModifier, 100); break;
      case 5: QTest::keyClick(m_view, Qt::Key_Left, Qt::NoModifier, 100); break;
      case 6: QTest::keyClick(m_view, Qt::Key_Right, Qt::NoModifier, 100); break;
      case 7: QTest::keyClick(m_view, Qt::Key_Space, Qt::NoModifier, 100); break;
      case 8: QTest::keyClick(m_view, Qt::Key_X, Qt::NoModifier, 100); break;
    }
    m_view->update();
  }
}

void ribi::QtKeyboardFriendlyGraphicsViewTest::SpaceSelectsOneRandomItemWhenZeroAreSelected()
{
  m_item1->setSelected(false);
  m_item2->setSelected(false);
  QVERIFY(m_view->scene()->selectedItems().empty());
  QTest::keyClick(m_view, Qt::Key_Space, Qt::NoModifier, 100);
  QVERIFY(m_view->scene()->selectedItems().size() == 1);
}

void ribi::QtKeyboardFriendlyGraphicsViewTest::TappingShiftMustKeepFocus()
{
  return; // TODO
  // Tapping SHIFT must not remove focus
  //item1 unselected and unfocused at right
  m_view->showFullScreen();
  m_item1->setSelected(false);
  //item2 selected and focused at left
  m_item2->setSelected(true);
  m_view->clearFocus();
  m_item2->setFocus();
  m_view->showFullScreen();
  qApp->processEvents();
  QVERIFY(m_item2->hasFocus());
  //m_view->SetVerbosity(verbose);
  QTest::keyClick(m_view, Qt::Key_Shift, Qt::NoModifier, 100);
  qApp->processEvents();
  QVERIFY(m_item2->hasFocus());
}
