//---------------------------------------------------------------------------
/*
System, class to interact with the system
Copyright (C) 2015-2016 Richel Bilderbeek

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
#include "ribi_system.h"





#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>

#include "fileio.h"




ribi::System::System()
{

}

std::string ribi::System::GetHome() const noexcept {
  return std::getenv("HOME");
}

std::string ribi::System::GetPath() const noexcept {
  return std::getenv("PATH");
}

std::string ribi::System::GetWhoami() const noexcept
{
  const auto tempfilename = ribi::FileIo().GetTempFileName();
  std::stringstream cmd;
  cmd << "whoami > " << tempfilename;
  const int error_code = std::system(cmd.str().c_str());
  if (error_code)
  {
    std::cerr << "Command '" << cmd.str() << "' had non-OK exit code " << error_code << std::endl;
  }
  assert(FileIo().IsRegularFile(tempfilename));
  std::ifstream file(tempfilename);
  std::string user;
  file >> user;
  FileIo().DeleteFile(tempfilename);
  return user;
}
