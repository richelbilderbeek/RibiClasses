//---------------------------------------------------------------------------
/*
fuzzy_equal_to, fuzzy equal_to predicate
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
//From http://www.richelbilderbeek.nl/CppFuzzy_equal_to.htm
//---------------------------------------------------------------------------
#include "fuzzy_equal_to.h"
#include <boost/test/unit_test.hpp>
#include <boost/math/constants/constants.hpp>

BOOST_AUTO_TEST_CASE(ribi_fuzzy_equal_to_test)
{
  //Set a high tolerance
  const double quarter_of_pi{boost::math::constants::half_pi<double>() / 2.0};
  const ribi::fuzzy_equal_to f(0.0000001);
  BOOST_CHECK(std::cos(quarter_of_pi) != std::sin(quarter_of_pi));
  BOOST_CHECK(f(std::cos(quarter_of_pi),std::sin(quarter_of_pi)));
  BOOST_CHECK(f(    1.0,   1.0 - std::cos(quarter_of_pi) + std::sin(quarter_of_pi)));
  BOOST_CHECK(f(   10.0,  10.0 - std::cos(quarter_of_pi) + std::sin(quarter_of_pi)));
  BOOST_CHECK(f(  100.0, 100.0 - std::cos(quarter_of_pi) + std::sin(quarter_of_pi)));
  BOOST_CHECK(f( 1000.0,1000.0 - std::cos(quarter_of_pi) + std::sin(quarter_of_pi)));
}

