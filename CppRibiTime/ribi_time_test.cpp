#include <boost/test/unit_test.hpp>
#include "ribi_time.h"

BOOST_AUTO_TEST_CASE(ribi_time_test)
{
  const ribi::Time t;
  {
    t.Wait(0.0);
  }
  BOOST_CHECK(t.GetTodayIso8601Boost() == t.GetTodayIso8601Stl());
  BOOST_CHECK(!t.GetTodayIso8601Stl().empty());
  BOOST_CHECK(t.GetTodayIso8601AsInt() > 0);
}
