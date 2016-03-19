//---------------------------------------------------------------------------
/*
Counter, an incrementing counter
Copyright (C) 2011-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppCounter.htm
//---------------------------------------------------------------------------
#include "counter.h"
#include <boost/test/unit_test.hpp>
#include "container.h"
#include "trace.h"

BOOST_AUTO_TEST_CASE(counter_test)
{
  using ribi::Counter;
  const bool verbose{false};
  if (verbose) { TRACE("Default-construction must have value zero"); }
  {
    const Counter c;
    BOOST_CHECK(c.Get() == 0);
  }
  if (verbose) { TRACE("Construction with value must return it"); }
  {
    const Counter c(42);
    BOOST_CHECK(c.Get() == 42);
  }
  if (verbose) { TRACE("Increment must increment"); }
  {
    Counter c;
    const int old_value = c.Get();
    c.Inc();
    BOOST_CHECK(c.Get() == old_value + 1);
  }
  if (verbose) { TRACE("operator++ must increment"); }
  {
    Counter c;
    const int old_value = c.Get();
    ++c;
    BOOST_CHECK(c.Get() == old_value + 1);
  }
  if (verbose) { TRACE("operator=="); }
  {
    Counter c;
    Counter d;
    BOOST_CHECK(c == d);
  }
  if (verbose) { TRACE("operator!="); }
  {
    Counter c;
    Counter d;
    ++c;
    BOOST_CHECK(c != d);
  }
}
