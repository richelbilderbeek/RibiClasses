//---------------------------------------------------------------------------
/*
TestTimer, class that measures time a test takes
Copyright (C) 2014-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppTestTimer.htm
//---------------------------------------------------------------------------
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
