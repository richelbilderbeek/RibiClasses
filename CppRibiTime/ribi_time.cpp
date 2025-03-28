



#include "ribi_time.h"

#include <cassert>
#include <chrono>

#include <boost/date_time/gregorian/gregorian.hpp>






ribi::Time::Time()
{

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
