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
#include "ribi_system.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>

#include "fileio.h"
#include "testtimer.h"
#include "trace.h"
#pragma GCC diagnostic pop

ribi::System::System()
{
  #ifndef NDEBUG
  Test();
  #endif
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

#ifndef NDEBUG
void ribi::System::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  {
    FileIo();
  }
  const TestTimer test_timer(__func__,__FILE__,1.0);
  {
    assert(!System().GetWhoami().empty());
  }
}
#endif
