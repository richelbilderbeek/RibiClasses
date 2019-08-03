//---------------------------------------------------------------------------
/*
Dial, dial class
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
//From http://www.richelbilderbeek.nl/CppDial.htm
//---------------------------------------------------------------------------




#include "dial.h"

#include <cassert>
#include <boost/lexical_cast.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include "geometry.h"



void ribi::DialTest() noexcept
{
  {
    const Dial a;
    const Dial b;
    assert(a == b);
  }
  {
    const Dial a(0.0,255,0,0);
    const Dial b(0.0,255,0,0);
    assert(a == b);
  }
  {
    const Dial a(0.0,255,0,0);
    const Dial b(1.0,255,0,0);
    assert(a != b);
  }
  {
    const Dial a(0.0,255,0,0);
    const Dial b(0.0,0,0,0);
    assert(a != b);
  }
  {
    const Dial a(0.0,255,0,0);
    const Dial b(0.0,255,255,0);
    assert(a != b);
  }
  {
    const Dial a(0.0,255,0,0);
    const Dial b(0.0,255,0,255);
    assert(a != b);
  }
  {
    const Dial a;
    const Dial b(a);
    assert(a == b);
  }
  {
    const Dial a(0.0,255,0,0);
    Dial b(1.0,255,0,0);
    assert(a != b);
    b = a;
    assert(a == b);
  }
}
