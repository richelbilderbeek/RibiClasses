//---------------------------------------------------------------------------
/*
Container, class with container class helper functions
Copyright (C) 2013-2016 Richel Bilderbeek

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
//From http://www.richelbilderbeek.nl/CppContainer.htm
//---------------------------------------------------------------------------
#include <boost/test/unit_test.hpp>
#include "container.h"
#include "trace.h"

BOOST_AUTO_TEST_CASE(container_test)
{
  const bool verbose{false};
  const ribi::Container c;
  if (verbose) { TRACE("Concatenate: empty vector with empty seperator must result in an empty string"); }
  {
    const std::vector<std::string> v{};
    const std::string s{c.Concatenate(v,"")};
    BOOST_CHECK(s.empty());
  }
  if (verbose) { TRACE("Concatenate: empty vector with longer seperator must result in an empty string"); }
  {
    const std::vector<std::string> v{};
    const std::string s{c.Concatenate(v,"[wont be used]")};
    BOOST_CHECK(s.empty());
  }
  if (verbose) { TRACE("Concatenate: vector with one string and empty seperator must result in that string"); }
  {
    const std::string s{"any string"};
    const std::vector<std::string> v{s};
    const std::string t{c.Concatenate(v,"")};
    BOOST_CHECK(s == t);
  }
  if (verbose) { TRACE("Concatenate: vector with one string and longer seperator must result in that string"); }
  {
    const std::string s{"any string again"};
    const std::vector<std::string> v{s};
    const std::string t{c.Concatenate(v,"[wont be used]")};
    BOOST_CHECK(s == t);
  }
  if (verbose) { TRACE("Concatenate: vector with two string and empty seperator must result in the summed string"); }
  {
    const std::string s{"any string"};
    const std::string t{"goes on"};
    const std::string expected{s+t};
    const std::vector<std::string> v{s,t};
    const std::string u{c.Concatenate(v,"")};
    BOOST_CHECK(u == expected);
  }
  if (verbose) { TRACE("Concatenate: vector with two string and longer seperator must result in the summed string"); }
  {
    const std::string s{"any string"};
    const std::string t{"goes on"};
    const std::string seperator{" "};
    const std::string expected{s+seperator+t};
    const std::vector<std::string> v{s,t};
    const std::string u{c.Concatenate(v,seperator)};
    BOOST_CHECK(u == expected);
  }
  //SeperateString
  {
    { //Single input, seperator of type char
      const auto v = c.SeperateString("a",',');
      BOOST_CHECK(v.size() == 1);
      BOOST_CHECK(v[0]=="a");
    }
    { //Two inputs, seperator of type char
      const auto v = c.SeperateString("a,b",',');
      BOOST_CHECK(v.size() == 2);
      BOOST_CHECK(v[0]=="a");
      BOOST_CHECK(v[1]=="b");
    }
    {
      //Five inputs, seperator of type char
      const auto v = c.SeperateString("a,bb,ccc,dddd,eeeee",',');
      BOOST_CHECK(v.size() == 5);
      BOOST_CHECK(v[0]=="a");
      BOOST_CHECK(v[1]=="bb");
      BOOST_CHECK(v[2]=="ccc");
      BOOST_CHECK(v[3]=="dddd");
      BOOST_CHECK(v[4]=="eeeee");
    }
    { //Three inputs, of which one empty, seperator of type char
      const auto v = c.SeperateString("a, ,ccc",',');
      BOOST_CHECK(v.size() == 3);
      BOOST_CHECK(v[0]=="a");
      BOOST_CHECK(v[1]==" ");
      BOOST_CHECK(v[2]=="ccc");
    }
  }
  //AllAboutEqual
  {
    std::vector<double> v = { 0.9, 1.0, 1.1 };
    BOOST_CHECK(c.AllAboutEqual(v,1.0));
    BOOST_CHECK(!c.AllAboutEqual(v,0.01));
  }
  //AllUnique
  {
    BOOST_CHECK( c.AllUnique( std::vector<int>( {} ) ));
    BOOST_CHECK( c.AllUnique( std::vector<int>( {1} ) ));
    BOOST_CHECK( c.AllUnique( std::vector<int>( {1,2} ) ));
    BOOST_CHECK( c.AllUnique( std::vector<int>( {1,2,3} ) ));
    BOOST_CHECK(!c.AllUnique( std::vector<int>( {1,2,1} ) ));
  }
  //HasNoOverlap
  {
    BOOST_CHECK( c.HasNoOverlap( std::vector<int>( {} ),std::vector<int>( {} )));
    BOOST_CHECK( c.HasNoOverlap( std::vector<int>( {1} ),std::vector<int>( {} )));
    BOOST_CHECK( c.HasNoOverlap( std::vector<int>( {1} ),std::vector<int>( {2} )));
    BOOST_CHECK(!c.HasNoOverlap( std::vector<int>( {1} ),std::vector<int>( {1} )));
    BOOST_CHECK( c.HasNoOverlap( std::vector<int>( {1,2} ),std::vector<int>( {3} )));
    BOOST_CHECK(!c.HasNoOverlap( std::vector<int>( {1,2,3} ),std::vector<int>( {1,2,3} )));
    BOOST_CHECK( c.HasNoOverlap( std::vector<int>( {1,2,3} ),std::vector<int>( {4,5,6} )));
  }
  //ToUpper
  {
    BOOST_CHECK(c.ToUpper("abc") == "ABC");
    BOOST_CHECK(c.ToUpper("ABC") == "ABC");
  }
  //RemoveWhitespace
  {
    BOOST_CHECK(c.RemoveWhitespace("abc") == "abc");
    BOOST_CHECK(c.RemoveWhitespace("a\tb \nc") == "abc");
  }
  //Sort
  {
    std::vector<int> v = {3,1,2};
    const std::vector<int> w = {1,2,3};
    c.Sort(v);
    BOOST_CHECK(v == w);
  }
  //Count
  {
    const auto v = {3,1,2};
    BOOST_CHECK(c.Count(v,1) == 1);
    BOOST_CHECK(c.Count(v,4) == 0);
  }
  //CountIf
  {
    const auto v = {3,1,2};
    BOOST_CHECK(c.CountIf(v, [](const int i) { return i > 0; }) == 3);
    BOOST_CHECK(c.CountIf(v, [](const int i) { return i > 1; }) == 2);
    BOOST_CHECK(c.CountIf(v, [](const int i) { return i > 2; }) == 1);
    BOOST_CHECK(c.CountIf(v, [](const int i) { return i > 3; }) == 0);
  }
  //CountIf
  {
    const auto v = {3,1,2};
    BOOST_CHECK(c.FindIf(v, [](const int i) { return i > 0; }) != std::end(v));
    BOOST_CHECK(c.FindIf(v, [](const int i) { return i > 1; }) != std::end(v));
    BOOST_CHECK(c.FindIf(v, [](const int i) { return i > 2; }) != std::end(v));
    BOOST_CHECK(c.FindIf(v, [](const int i) { return i > 3; }) == std::end(v));
  }
}
