#include "stopwatch.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_ribi_test_stopwatch)
{
  ribi::Stopwatch s;
  BOOST_CHECK(s.GetElapsedSecs() >= 0.0);
  BOOST_CHECK(!s.GetVersion().empty());
  BOOST_CHECK(!s.GetVersionHistory().empty());
}
