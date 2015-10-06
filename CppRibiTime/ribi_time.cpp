//---------------------------------------------------------------------------
/*
RibiRegex, class for working with regular expressions
Copyright (C) 2014-2015 Richel Bilderbeek

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
#include "ribi_time.h"

#include <cassert>
#include <chrono>

#include <boost/date_time/gregorian/gregorian.hpp>

#include "testtimer.h"
#include "trace.h"

#pragma GCC diagnostic pop

ribi::Time::Time()
{
  #ifndef NDEBUG
  Test();
  #endif
}

int ribi::Time::GetTodayIso8601AsInt() const noexcept
{
  std::string s{GetTodayIso8601()};
  const auto new_end = std::remove(
    std::begin(s),
    std::end(s),
    '-'
  );
  s.erase(new_end,std::end(s));
  const int d{std::stoi(s)};
  return d;
}

std::string ribi::Time::GetTodayIso8601Boost() const noexcept
{
  const boost::gregorian::date today
    = boost::gregorian::day_clock::local_day();
  const std::string s
    = boost::gregorian::to_iso_extended_string(today);
  assert(s.size()==10);
  assert(s[4]=='-');
  assert(s[7]=='-');
  return s;
}

std::string ribi::Time::GetTodayIso8601Stl() const noexcept
{
  const std::time_t t = std::time(0);
  const std::tm * const now = std::localtime( &t );
  std::string year;
  {
    std::stringstream s;
    s << (now->tm_year + 1900);
    year = s.str();
  }
  assert(year.size() == 4);
  std::string month;
  {
    std::stringstream s;
    s << (now->tm_mon + 1);
    month = s.str();
    if (month.size() == 1) month = std::string("0") + month;
  }
  assert(month.size() == 2);
  std::string day;
  {
    std::stringstream s;
    s << (now->tm_mday);
    day = s.str();
    assert(day.size() == 1 || day.size() == 2);
    if (day.size() == 1)
    {
      day = "0" + day;
      assert(day.size() == 2);
    }
  }
  assert(day.size() == 2);
  return year + "-" + month + "-" + day;
}

std::string ribi::Time::GetVersion() noexcept
{
  return "1.1";
}

std::vector<std::string> ribi::Time::GetVersionHistory() noexcept
{
  return {
    "2015-10-04: Version 1.0: initial version with Wait",
    "2015-10-06: Version 1.1: added GetToday functions",
  };
}

void ribi::Time::Wait(const double n_secs) const noexcept
{
  const std::chrono::system_clock::time_point t = std::chrono::system_clock::now();
  while (1)
  {
    const std::chrono::system_clock::duration d = std::chrono::system_clock::now() - t;
    const long long int n_msecs_passed{std::chrono::duration_cast<std::chrono::milliseconds>(d).count()};
    const double secs_passed{static_cast<double>(n_msecs_passed) / 1000.0};
    if (secs_passed > n_secs) return;
  }
}

#ifndef NDEBUG
void ribi::Time::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  const TestTimer test_timer(__func__,__FILE__,1.0);
  //const bool verbose{false};
  const Time t;
  {
    t.Wait(0.0);
  }
  assert(t.GetTodayIso8601Boost() == t.GetTodayIso8601Stl());
  assert(!t.GetTodayIso8601Stl().empty());
  assert(t.GetTodayIso8601AsInt() > 0);
}
#endif
