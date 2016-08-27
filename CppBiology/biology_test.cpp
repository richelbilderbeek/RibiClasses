#include <boost/test/unit_test.hpp>
#include "biology.h"

BOOST_AUTO_TEST_CASE(biology_test)
{
  ribi::Biology b;
  const ribi::Biology::PopSize pop_size{1.0};
  const double r{2.0};
  BOOST_CHECK(b.LogisticGrowth(pop_size,0.5 * pop_size,r) > 0.0);
  BOOST_CHECK(b.LogisticGrowth(pop_size,2.0 * pop_size,r) < 0.0);
}
