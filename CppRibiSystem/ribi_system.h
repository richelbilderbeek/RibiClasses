//---------------------------------------------------------------------------
/*
System, class to interact with the system
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
//From http://www.richelbilderbeek.nl/CppSystem.htm
//---------------------------------------------------------------------------
#ifndef RIBI_SYSTEM_H
#define RIBI_SYSTEM_H

#include <string>
#include <vector>
#include "fileiofwd.h"
#include "filecopymode.h"

namespace ribi {

struct System
{
  System();

  std::string GetHome() const noexcept;
  std::string GetPath() const noexcept;
  std::string GetWhoami() const noexcept;

  private:
  #ifndef NDEBUG
  void Test() noexcept;
  #endif
};

} //~namespace ribi

#endif // RIBI_SYSTEM_H
