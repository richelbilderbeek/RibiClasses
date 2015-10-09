//---------------------------------------------------------------------------
/*
Counter, an incrementing counter
Copyright (C) 2011-2015 Richel Bilderbeek

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

#include <sstream>

#include "trace.h"

ribi::Counter::Counter(const int initial_count) noexcept
  : m_count(initial_count), m_verbosity{false}
{
  #ifndef NDEBUG
  Test();
  #endif
}

std::string ribi::Counter::GetVersion() noexcept
{
  return "1.3";
}

std::vector<std::string> ribi::Counter::GetVersionHistory() noexcept
{
  return {
    "2011-08-20: Version 1.0: initial version",
    "2014-08-01: Version 1.1: added tests",
    "2014-04-18: Version 1.2: added operator++, operator== and operator!=",
    "2014-04-18: Version 1.3: added verbosity"
  };
}

void ribi::Counter::Inc() noexcept
{
  ++m_count;

  if (m_verbosity)
  {
    std::stringstream s;
    s << "Incremented counter to " << m_count;
    TRACE(s.str());
  }
}

bool ribi::operator==(const ribi::Counter& lhs, const ribi::Counter& rhs) noexcept
{
  return lhs.Get() == rhs.Get();
}

bool ribi::operator!=(const ribi::Counter& lhs, const ribi::Counter& rhs) noexcept
{
  return !(lhs == rhs);
}
