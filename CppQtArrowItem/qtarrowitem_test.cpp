#include "qtarrowitem.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(qtarrowitem_test)
{
  BOOST_CHECK_NO_THROW(
    ribi::QtArrowItem(
      0.0, //const double x1,
      0.0, //const double y1,
      false, //const bool tail,
      100.0, //const double x2,
      200.0, //const double y2,
      true, //const bool head,
      nullptr //QGraphicsItem* parent = 0
    )
  );
}
