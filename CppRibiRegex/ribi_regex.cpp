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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "ribi_regex.h"

#include <cassert>
#include <sstream>
#include <stdexcept>
#include <boost/xpressive/xpressive.hpp>

#include <QRegExp>


// #include "trace.h"

#pragma GCC diagnostic pop

ribi::Regex::Regex()
{
}

std::vector<std::string>
  ribi::Regex::GetRegexMatchesQRegExp(
  const std::string& s,
  const std::string& regex_str
) const noexcept
{
  QRegExp r(regex_str.c_str());
  r.setMinimal(true); //QRegExp must be non-greedy
  std::vector<std::string> v;
  int pos = 0;
  while ((pos = r.indexIn(s.c_str(), pos)) != -1)
  {
    const QString q = r.cap(1);
    if (q.isEmpty()) break;
    v.push_back(q.toStdString());
    pos += r.matchedLength();
  }
  return v;
}

std::vector<std::string>
  ribi::Regex::GetRegexMatchesXpressive(
  const std::string& s,
  const std::string& regex_str
) const
{
  if (!IsValidXpressive(regex_str))
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "invalid regex '" << regex_str
      << "' supplied"
    ;
    throw std::invalid_argument(msg.str());
  }

  const boost::xpressive::sregex r
    = boost::xpressive::sregex::compile(regex_str)
  ;

  std::vector<std::string> v;
  boost::xpressive::sregex_iterator cur(
    std::begin(s),
    std::end(s),
    r
  );
  boost::xpressive::sregex_iterator end;
  for( ; cur != end; ++cur )
  {
    const boost::xpressive::smatch& what = *cur;
    v.push_back(what[0]);
  }
  return v;
}

std::string ribi::Regex::GetRegexDutchFloat() const noexcept
{
  return R"((\d*,\d*))";
}

std::string ribi::Regex::GetRegexDutchPostalCode() const noexcept
{
  return R"((\d{4})( )([A-Z]{2}))";
}

std::string ribi::Regex::GetRegexPolygon() const noexcept
{
  return
    "(POLYGON\\(\\(.*?\\)\\))"
  ;
}

std::string ribi::Regex::GetRegexShapes() const noexcept
{
  return R"((POLYGON\(\(.*?\)\))|(LINESTRING\(.*?\)))";
}

std::string ribi::Regex::GetVersion() noexcept
{
  return "1.2";
}

std::vector<std::string> ribi::Regex::GetVersionHistory() noexcept
{
  return {
    "2014-06-19: Version 1.0: initial version",
    "2014-01-02: Version 1.1: added GetRegexDutchFloat",
    "2016-03-19: Version 1.2: use of Boost.Test"
  };
}

bool ribi::Regex::IsValidXpressive(const std::string& regex_str) const noexcept
{
  try
  {
    boost::xpressive::sregex::compile(regex_str);
  }
  catch (boost::xpressive::regex_error&)
  {
    return false;
  }
  return true;
}
