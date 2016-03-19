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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "container.h"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include "fuzzy_equal_to.h"
#include "testtimer.h"
#include "trace.h"
#pragma GCC diagnostic pop

template <class Container>
std::string ContainerToStrImpl(const Container& c, const std::string& seperator)
{
  std::stringstream s;
  std::copy(c.begin(),c.end(),
    std::ostream_iterator<typename Container::value_type>(s,seperator.c_str()));
  return s.str();
}

ribi::Container::Container()
{
}

bool ribi::Container::AllAboutEqual(
  const std::vector<double>& v,
  const double tolerance) const noexcept
{
  assert(!v.empty());
  fuzzy_equal_to f(tolerance);
  const double first_value{v[0]};

  return std::count_if(
    std::begin(v),
    std::end(v),
    [f,first_value](const double x) { return f(first_value,x); }
  )
    == boost::numeric_cast<int>(v.size())
  ;
}

std::string ribi::Container::Concatenate(const std::vector<std::string>& v, const std::string& seperator) const noexcept
{
  std::stringstream s;
  for (const auto& t: v) { s << t << seperator; }
  std::string str = s.str();
  //Remove seperator
  if (!str.empty())
  {
    str.resize(str.size() - seperator.size());
  }
  return str;
}

std::string ribi::Container::ContainerToStr(
  const std::vector<std::string>& s,
  const std::string& seperator
) const noexcept
{
  return ContainerToStrImpl(s,seperator);
}


std::string ribi::Container::GetVersion() const noexcept
{
  return "1.2";
}

std::vector<std::string> ribi::Container::GetVersionHistory() const noexcept
{
  return {
    "2014-xx-xx: Version 1.0: initial version",
    "2014-06-14: Version 1.1: added SeperateString"
    "2014-07-30: Version 1.2: added Concatenate"
  };
}

std::string ribi::Container::RemoveWhitespace(std::string s) const noexcept
{
  boost::algorithm::erase_all(s," ");
  boost::algorithm::erase_all(s,"\t");
  boost::algorithm::erase_all(s,"\n");
  return s;
}

std::vector<std::string> ribi::Container::SeperateString(
  const std::string& input,
  const char seperator) const noexcept
{
  std::vector<std::string> v;
  boost::algorithm::split(v,input,
    std::bind2nd(std::equal_to<char>(),seperator),
    boost::algorithm::token_compress_on);
  return v;
}

std::string ribi::Container::ToUpper(std::string s) const
{
  std::transform(
    std::begin(s), std::end(s), std::begin(s),
    std::ptr_fun<int,int>(std::toupper)
  );
  return s;
}
