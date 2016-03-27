//---------------------------------------------------------------------------
/*
CaesarCipher, Caesar cipher class
Copyright (C) 2010-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
// From http://www.richelbilderbeek.nl/CppCaesarCipher.htm
//---------------------------------------------------------------------------
#include "caesarcipher.h"
#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <numeric>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "trace.h"
#include "testtimer.h"
#include "loopreader.h"
#pragma GCC diagnostic pop

BOOST_AUTO_TEST_CASE(ribi_caesarcipher_test)
{
  using namespace ribi;
  {
    const CaesarCipher e(0);
    const std::string s = "ABCDEFGH";
    BOOST_CHECK(!CaesarCipher::IsClean(s));
    const std::string t = CaesarCipher::Clean(s);
    BOOST_CHECK(t == "abcdefgh");
    BOOST_CHECK(CaesarCipher::IsClean(t));
    BOOST_CHECK(t == e.Encrypt(t));
    BOOST_CHECK(t == e.Deencrypt(t));
  }
  {
    const std::vector<std::string> v {
      "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ",
      "A",
      "AB",
      "I am a dirty string"
    };
    for (const std::string& s: v)
    {
      for (const int key: {0,1,2,4,8,16,257,-13,-12345} )
      {
        const CaesarCipher e(key);
        const std::string t = CaesarCipher::Clean(s);
        BOOST_CHECK(CaesarCipher::IsClean(t));
        BOOST_CHECK(e.Deencrypt(e.Encrypt(t)) == t);
        //Test encryption with real, decryption with faker
        const CaesarCipher faker(key + 1);
        BOOST_CHECK(faker.Deencrypt(e.Encrypt(t)) != t);
      }
    }
  }
}
