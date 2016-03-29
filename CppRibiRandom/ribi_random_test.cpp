#include <boost/test/unit_test.hpp>
#include "ribi_random.h"

BOOST_AUTO_TEST_CASE(ribi_random_get_bool)
{
  ribi::Random r(42);
  while (r.GetBool() == true) {}
  while (r.GetBool() == false) {}
}

BOOST_AUTO_TEST_CASE(ribi_random_get_char)
{
  ribi::Random r(42);
  const char c = r.GetChar();
}

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

BOOST_AUTO_TEST_CASE(ribi_random_get_int)
{
  ribi::Random r(42);
  while (r.GetInt(0, 1) == 0) {}
  while (r.GetInt(0, 1) == 1) {}
}

BOOST_AUTO_TEST_CASE(ribi_random_get_normal)
{
  ribi::Random r(42);
  double sum{0.0};
  for (int i=0; i!=100; ++i) { sum += r.GetNormal(0.0,1.0); }
  BOOST_CHECK_LT(sum,  10.0);
  BOOST_CHECK_GT(sum, -10.0);
}

BOOST_AUTO_TEST_CASE(ribi_random_get_string)
{
  ribi::Random r(42);
  const auto s = r.GetString(99);
  //The frequence of the character at the first spot, should be less than 10%
  BOOST_CHECK(std::count(std::begin(s),std::end(s),s[0]) < 10);
}

BOOST_AUTO_TEST_CASE(ribi_random_get_version)
{
  ribi::Random r(42);
  BOOST_CHECK(!r.GetVersion().empty());
  BOOST_CHECK(!r.GetVersionHistory().empty());
}
