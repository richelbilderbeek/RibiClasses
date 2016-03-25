#include <boost/test/unit_test.hpp>
#include "ribi_random.h"

BOOST_AUTO_TEST_CASE(ribi_random_get_fraction)
{
  ribi::Random r(42);
  for (int i=0; i!=100; ++i)
  {
    const double f = r.GetFraction();
    BOOST_CHECK(f >= 0.0);
    BOOST_CHECK(f  < 1.0);
  }
}

BOOST_AUTO_TEST_CASE(ribi_random_get_string)
{
  ribi::Random r(42);
  const auto s = r.GetString(99);
  //The frequence of the character at the first spot, should be less than 10%
  BOOST_CHECK(std::count(std::begin(s),std::end(s),s[0]) < 10);
}
