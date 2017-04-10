#include "stopwatch.h"

ribi::Stopwatch::Stopwatch() noexcept
  : m_time_start(std::chrono::system_clock::now())
{

}

double ribi::Stopwatch::GetElapsedSecs() const noexcept
{
  const std::chrono::system_clock::duration d = std::chrono::system_clock::now() - m_time_start;
  const long long int n_msecs_passed{std::chrono::duration_cast<std::chrono::milliseconds>(d).count()};
  const double secs_passed{static_cast<double>(n_msecs_passed) / 1000.0};
  return secs_passed;
}

std::string ribi::Stopwatch::GetVersion() const noexcept
{
  return "2.0";
}

std::vector<std::string> ribi::Stopwatch::GetVersionHistory() const noexcept
{
  return {
    "2010-10-08: version 1.0: initial version",
    "2011-05-30: version 1.1: added version history",
    "2015-08-28: version 2.0: give time in floating point seconds, use C++11 chrono facilities"
  };
}
