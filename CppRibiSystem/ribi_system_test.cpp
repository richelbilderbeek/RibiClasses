#include "ribi_system.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(ribi_system_test)
{
  ribi::System s;
  BOOST_CHECK(!s.GetHome().empty());
  BOOST_CHECK(!s.GetPath().empty());
  BOOST_CHECK(!s.GetWhoami().empty());
}
