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

#include "testtimer.h"
#include "trace.h"

#pragma GCC diagnostic pop

ribi::Time::Time()
{
  #ifndef NDEBUG
  Test();
  #endif
}

std::string ribi::Time::GetVersion() noexcept
{
  return "1.0";
}

std::vector<std::string> ribi::Time::GetVersionHistory() noexcept
{
  return {
    "2015-10-04: Version 1.0: initial version",
  };
}

void ribi::Time::Wait(const double n_secs) noexcept
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
  {
    Time();
  }
}
#endif
