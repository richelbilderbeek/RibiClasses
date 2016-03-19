//---------------------------------------------------------------------------
/*
MenuDialog, menu dialog class
Copyright (C) 2013-2016 Richel Bilderbeek

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
//From http://www.richelbilderbeek.nl/CppMenuDialog.htm
//---------------------------------------------------------------------------
#include "menudialog.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(ribi_menu_dialog_convert_arguments_test)
{
  char ** argv = new char*[2];
  argv[0] = new char[16];
  argv[1] = new char[16];
  const std::string s1 = "012345678901234";
  const std::string s2 = "ABCDEFGHIJKLMNO";
  std::strcpy(argv[0],s1.c_str());
  std::strcpy(argv[1],s2.c_str());
  const auto v = ribi::MenuDialog::ConvertArguments(2, argv);
  BOOST_CHECK(v.size() == 2);
  BOOST_CHECK(v[0] == s1);
  BOOST_CHECK(v[1] == s2);
  delete[] argv[0];
  delete[] argv[1];
  delete[] argv;
}
