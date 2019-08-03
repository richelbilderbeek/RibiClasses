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
#pragma GCC diagnostic push



#include "dial.h"

#include <cassert>
#include <boost/lexical_cast.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include "geometry.h"

#pragma GCC diagnostic pop

ribi::Dial::Dial(
  const double position,
  const unsigned char red,
  const unsigned char green,
  const unsigned char blue)
  : m_blue{blue},
    m_green{green},
    m_position{position},
    m_red{red}
{
  assert(m_position >= 0.0);
  assert(m_position <= 1.0);
}

std::string ribi::Dial::GetVersion() noexcept
{
  return "4.0";
}

std::vector<std::string> ribi::Dial::GetVersionHistory() noexcept
{
  return {
    "2011-04-11: Version 1.0: initial version",
    "2011-06-03: Version 2.0: moved widget logic to DialWidget class",
    "2011-08-07: Version 3.0: conformized architure for MysteryMachine",
    "2011-08-20: Version 3.1: added operator<<",
    "2011-08-31: Version 3.2: allow changing the dial its color",
    "2013-04-30: Version 3.3: added testing, fixed bug in GetAngle",
    "2015-11-21: Version 4.0: made this class a regular type"
  };
}

void ribi::Dial::SetBlue(const int b) noexcept
{
  assert(b >=   0);
  assert(b  < 256);

  if (m_blue != boost::numeric_cast<unsigned char>(b))
  {
    m_blue = boost::numeric_cast<unsigned char>(b);
  }
}

void ribi::Dial::SetColor(const int r,const int g,const int b) noexcept
{
  SetRed(r);
  SetGreen(g);
  SetBlue(b);
}

void ribi::Dial::SetGreen(const int g) noexcept
{
  assert(g >=   0);
  assert(g  < 256);

  if (m_green != boost::numeric_cast<unsigned char>(g))
  {
    m_green = boost::numeric_cast<unsigned char>(g);
  }
}

void ribi::Dial::SetRed(const int r) noexcept
{
  assert(r >=   0);
  assert(r  < 256);

  if (m_red != boost::numeric_cast<unsigned char>(r))
  {
    m_red = boost::numeric_cast<unsigned char>(r);
  }
}

void ribi::Dial::SetPosition(const double position) noexcept
{
  assert(position >= 0.0);
  assert(position <= 1.0);
  if (m_position != position)
  {
    m_position = position;
  }
}

std::ostream& ribi::operator<<(std::ostream& os, const Dial& dial)
{
  os
    << "<Dial>"
    << "<blue>"
    <<   static_cast<int>(dial.m_blue)
    << "</blue>"
    << "<green>"
    <<   static_cast<int>(dial.m_green)
    << "</green>"
    << "<position>"
    <<   dial.m_position
    << "</position>"
    << "<red>"
    <<   static_cast<int>(dial.m_red)
    << "</red>"
    << "</Dial>";
  return os;
}

bool ribi::operator==(const Dial& lhs, const Dial& rhs)
{
  return
       lhs.GetPosition() == rhs.GetPosition()
    && lhs.GetRed()      == rhs.GetRed()
    && lhs.GetGreen()    == rhs.GetGreen()
    && lhs.GetBlue()     == rhs.GetBlue()
  ;
}

bool ribi::operator!=(const Dial& lhs, const Dial& rhs)
{
  return !(lhs == rhs);
}
