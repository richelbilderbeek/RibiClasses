//---------------------------------------------------------------------------
/*
RibiRegex, class for working with regular expressions
Copyright (C) 2014-2016 Richel Bilderbeek

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
//From http://www.richelbilderbeek.nl/CppRibiRegex.htm
//---------------------------------------------------------------------------
#include "ribi_regex.h"
#include <boost/test/unit_test.hpp>
#include "trace.h"

BOOST_AUTO_TEST_CASE(ribi_regex_test)
{
  ribi::Regex r;
  const bool verbose{false};

  if (verbose) { TRACE("GetRegexMatches, plain, on Dutch postal codes"); }
  {
    const std::string s = "In the Netherlands, 1234 AB and 2345 BC are valid zip codes";
    std::vector<std::string> expected;
    expected.push_back("1234 AB");
    expected.push_back("2345 BC");
    {
      const std::string t = "(\\d{4} [A-Z]{2})";
      BOOST_CHECK(r.GetRegexMatches(s,(t.c_str())) == expected);
    }
  }
  if (verbose) { TRACE("GetRegexMatches, plain, XML"); }
  {
    const std::string s = "<concept><name>Concept with examples</name><example>Example 1</example><example>Example 2</example><example>Example 3</example></concept>";
    const std::vector<std::string> expected {
      "<example>Example 1</example>",
      "<example>Example 2</example>",
      "<example>Example 3</example>"
    };
    const std::string regex_str = "(<example>.*?</example>)";
    BOOST_CHECK(r.GetRegexMatches(s,regex_str) == expected);
  }
  if (verbose) { TRACE("GetRegexMatches on GetRegexDutchPostalCode"); }
  {
    const std::string s = "Both '1234 AB' and '9999 ZZ' are valid Dutch zip codes";
    const std::string t = r.GetRegexDutchPostalCode();
    const auto v = r.GetRegexMatches(s,t);
    //const auto w = r.GetRegexMatchesQRegExp(s,t);
    const auto x = r.GetRegexMatchesXpressive(s,t);
    BOOST_CHECK(v.size() == 2);
    //BOOST_CHECK(v == w);
    BOOST_CHECK(v == x);
  }
  if (verbose) { TRACE("GetRegexMatches on GetRegexShapes"); }
  {
    const std::string s = "POLYGON((0 0,0 1,1 0)),LINESTRING(0 0,0 1,1 0)";
    const std::string t = r.GetRegexShapes();
    const auto v = r.GetRegexMatches(s,t);
    BOOST_CHECK(v.size() == 2);
    BOOST_CHECK(v[0] == "POLYGON((0 0,0 1,1 0))");
    BOOST_CHECK(v[1] == "LINESTRING(0 0,0 1,1 0)");
    //const auto w = r.GetRegexMatchesQRegExp(s,t);
    const auto x = r.GetRegexMatchesXpressive(s,t);
    //BOOST_CHECK(v == w);
    BOOST_CHECK(v == x);
  }
  if (verbose) { TRACE("GetRegexDutchFloat"); }
  {
    BOOST_CHECK( r.GetRegexMatches("1.23",r.GetRegexDutchFloat()).empty());
    BOOST_CHECK(!r.GetRegexMatches("1,23",r.GetRegexDutchFloat()).empty());
  }
}
