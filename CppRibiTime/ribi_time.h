//---------------------------------------------------------------------------
/*
RibiTime, class for working with time
Copyright (C) 2015-2015 Richel Bilderbeek

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
//From http://www.richelbilderbeek.nl/CppRibiTime.htm
//---------------------------------------------------------------------------
#ifndef RIBI_TIME_H
#define RIBI_TIME_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <string>
#include <vector>
#pragma GCC diagnostic pop

namespace ribi {

///Regex functions
struct Time
{
  Time();

  //Returns date in YYYY-MM-DD format
  std::string GetTodayIso8601() const noexcept { return GetTodayIso8601Stl(); }

  //Returns date in YYYYMMDD format
  int GetTodayIso8601AsInt() const noexcept;

  static std::string GetVersion() noexcept;
  static std::vector<std::string> GetVersionHistory() noexcept;

  void Wait(const double n_secs) const noexcept;

  private:

  std::string GetTodayIso8601Boost() const noexcept;
  std::string GetTodayIso8601Stl() const noexcept;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif


};

} //~namespace ribi

#endif // RIBI_TIME_H
