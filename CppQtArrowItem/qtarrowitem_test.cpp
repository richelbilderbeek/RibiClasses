#include "qtarrowitem_test.h"
#include "qtarrowitem.h"

void ribi::qtarrowitem_test::can_default_construct() const noexcept
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
