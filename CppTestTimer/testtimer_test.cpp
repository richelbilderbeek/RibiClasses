#include "testtimer.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_ribi_test_timer_simple_behavior)
{
  ribi::TestTimer t(__func__, __FILE__, 0.0); //Will trigger the message
  BOOST_CHECK(!t.GetVersion().empty());
  BOOST_CHECK(!t.GetVersionHistory().empty());
  const auto prev_cnt = t.GetMaxCnt();
  t.SetMaxCnt(42);
  BOOST_CHECK_EQUAL(t.GetMaxCnt(), 42);
  t.SetMaxCnt(prev_cnt);
  BOOST_CHECK_EQUAL(t.GetMaxCnt(), prev_cnt);
}

BOOST_AUTO_TEST_CASE(test_ribi_test_timer_set_negative_max_cnt)
{
  BOOST_CHECK_THROW(
    ribi::TestTimer::SetMaxCnt(-123),
    std::invalid_argument
  );
}
