//---------------------------------------------------------------------------
/*
LedWidget, Widget for the Led class
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
//From http://www.richelbilderbeek.nl/CppLedWidget.htm
//---------------------------------------------------------------------------



#include "ledwidget.h"

#include <iostream>

#include <boost/math/constants/constants.hpp>
#include <boost/geometry/algorithms/equals.hpp>

#include "textcanvas.h"


ribi::LedWidget::LedWidget(
  const int x,
  const int y,
  const int width,
  const int height,
  const double intensity,
  const unsigned char red,
  const unsigned char green,
  const unsigned char blue)
  : Widget(),
    m_led(intensity,red,green,blue)
{
  #ifndef NDEBUG
  Test();
  #endif
  this->SetGeometry(x,y,width,height);
}

std::string ribi::LedWidget::GetVersion() noexcept
{
  return "2.0";
}

std::vector<std::string> ribi::LedWidget::GetVersionHistory() noexcept
{
  return {
    "2011-07-03: version 1.0: initial version",
    "2011-08-17: Version 1.1: emit a signal when the color is changed",
    "2011-08-20: Version 1.2: added operator<<",
    "2011-09-08: Version 1.3: removed redundant signals",
    "2014-03-28: Version 1.4: replaced custom Rect class by Boost.Geometry",
    "2015-11-21: Version 2.0: made LedWidget a regular data type"
  };
}

#ifndef NDEBUG
void ribi::LedWidget::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  {
    const LedWidget a;
    const LedWidget b;
    assert(a == b);
  }
  {
    const LedWidget a(0,0,32,32,0.0,255,0,0);
    const LedWidget b(0,0,32,32,0.0,255,0,0);
    assert(a == b);
  }
  //operator=
  {
    const LedWidget a(0,0,32,32,0.0,255,0,0);
    LedWidget b(a);
    assert(a == b);
  }
  //operator=
  {
    const LedWidget a(0,0,32,32,0.0,255,0,0);
    LedWidget b(0,0,32,32,0.0,255,0,255);
    assert(a != b);
    b = a;
    assert(a == b);
  }
  //operator!= for each constructor element
  {
    const LedWidget a(0,0,32,32,0.0,255,0,0);
    const LedWidget b(1,0,32,32,0.0,255,0,0);
    assert(a != b);
  }
  {
    const LedWidget a(0,0,32,32,0.0,255,0,0);
    const LedWidget b(0,1,32,32,0.0,255,0,0);
    assert(a != b);
  }
  {
    const LedWidget a(0,0,32,32,0.0,255,0,0);
    const LedWidget b(0,0,16,32,0.0,255,0,0);
    assert(a != b);
  }
  {
    const LedWidget a(0,0,32,32,0.0,255,0,0);
    const LedWidget b(0,0,32,16,0.0,255,0,0);
    assert(a != b);
  }
  {
    const LedWidget a(0,0,32,32,0.0,255,0,0);
    const LedWidget b(0,0,32,32,1.0,255,0,0);
    assert(a != b);
  }
  {
    const LedWidget a(0,0,32,32,0.0,255,0,0);
    const LedWidget b(0,0,32,32,0.0,  0,0,0);
    assert(a != b);
  }
  {
    const LedWidget a(0,0,32,32,0.0,255,  0,0);
    const LedWidget b(0,0,32,32,0.0,255,255,0);
    assert(a != b);
  }
  {
    const LedWidget a(0,0,32,32,0.0,255,0,  0);
    const LedWidget b(0,0,32,32,0.0,255,0,255);
    assert(a != b);
  }
}
#endif

const boost::shared_ptr<ribi::TextCanvas> ribi::LedWidget::ToCanvas(const int radius) const noexcept
{
  assert(radius > 1);
  const int width  = 1+(radius*2);
  const int height = width;
  boost::shared_ptr<ribi::TextCanvas> canvas(
    new TextCanvas(width,height)
  );
  const double pi = boost::math::constants::pi<double>();

  //Inside circle
  {
    const std::vector<char> gradient { Canvas::GetAsciiArtGradient() };
    const double f_b { static_cast<double>(GetLed().GetBlue() ) / 255.0 };
    const double f_g { static_cast<double>(GetLed().GetGreen()) / 255.0 };
    const double f_r { static_cast<double>(GetLed().GetRed()  ) / 255.0 };
    const double f_c { (f_r + f_g + f_b) / 3.0 }; //Fraction of color
    const double f_i { GetLed().GetIntensity() };
    const double f { f_c * f_i };
    const int gradient_index {
      static_cast<int>(
        f * static_cast<double>(gradient.size())
      )
    };
    const char c { gradient[gradient_index] };

    const double midx = static_cast<double>(radius);
    const double midy = static_cast<double>(radius);

    for (int y=0; y!=height; ++y)
    {
      for (int x=0; x!=width; ++x)
      {
        const double x_co { static_cast<double>(x) };
        const double y_co { static_cast<double>(y) };
        const double dx { x_co - midx };
        const double dy { y_co - midy };
        const double dist { std::sqrt( (dx*dx) + (dy*dy) ) };
        if (dist < static_cast<double>(radius))
        {
          canvas->PutChar(x,y,c);
        }
      }
    }
  }

  //Circle
  {
    const int n_steps = std::ceil(2.0 * pi * static_cast<double>(radius));
    for (int i=0; i!=n_steps; ++i)
    {
      const double f = static_cast<double>(i) / static_cast<double>(n_steps);
      const double angle = f * 2.0 * pi;
      const double midx = static_cast<double>(radius);
      const double midy = static_cast<double>(radius);
      const double ray = static_cast<double>(radius);
      const int x = static_cast<int>(std::round(midx + (std::sin(angle) * ray)));
      const int y = static_cast<int>(std::round(midy - (std::cos(angle) * ray)));
      canvas->PutChar(x,y,'*');
    }
  }
  return canvas;
}

std::ostream& ribi::operator<<(std::ostream& os, const LedWidget& widget) noexcept
{
  os
    << "<LedWidget>"
    << widget.m_led
    << "</LedWidget>";
  return os;
}

bool ribi::operator==(const LedWidget& lhs, const LedWidget& rhs) noexcept
{
  using boost::geometry::equals;
  return lhs.GetLed() == rhs.GetLed()
    && equals(lhs.GetGeometry(),rhs.GetGeometry())
  ;
}

bool ribi::operator!=(const LedWidget& lhs, const LedWidget& rhs) noexcept
{
  return !(lhs == rhs);
}
