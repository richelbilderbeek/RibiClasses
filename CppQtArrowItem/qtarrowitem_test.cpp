#include "qtarrowitem_test.h"

#include <cassert>
#include "qtarrowitem.h"
#include "qtkeyboardfriendlygraphicsview.h"

void ribi::QtArrowItemTest::can_default_construct() const noexcept
{
  try
  {
    ribi::QtArrowItem a(
      0.0, //const double x1,
      0.0, //const double y1,
      false, //const bool tail,
      100.0, //const double x2,
      200.0, //const double y2,
      true, //const bool head,
      nullptr //QGraphicsItem* parent = 0
    );

    QVERIFY(!a.scene());
  }
  catch (...)
  {
    QVERIFY(!"Should not get here");
  }
}

void ribi::QtArrowItemTest::put_in_view() const noexcept
{
  ribi::QtKeyboardFriendlyGraphicsView v;
  v.show();
  assert(v.scene());
  QtArrowItem * const a{
    new QtArrowItem(
      0.0, //const double x1,
      0.0, //const double y1,
      false, //const bool tail,
      100.0, //const double x2,
      200.0, //const double y2,
      true, //const bool head,
      nullptr //QGraphicsItem* parent = 0
    )
  };
  assert(a);
  v.scene()->addItem(a);
  qApp->processEvents();
}
