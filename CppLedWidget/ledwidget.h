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
#ifndef LEDWIDGET_H
#define LEDWIDGET_H



#include <boost/scoped_ptr.hpp>
#include <boost/signals2.hpp>
#include "led.h"
#include "widget.h"



namespace ribi {

struct Led;
struct TextCanvas;

///Widget for displaying a Led
struct LedWidget : public Widget
{
  explicit LedWidget(
    const int x = 0,
    const int y = 0,
    const int width = 100,
    const int height = 100,
    const double intensity    = 0.0,
    const unsigned char red   = 255,
    const unsigned char green =   0,
    const unsigned char blue  =   0
  );
  ~LedWidget() noexcept {}

  ///Obtain the LED
  const Led& GetLed() const noexcept { return m_led; }
        Led& GetLed()       noexcept { return m_led; }

  ///Obtain the version of this class
  static std::string GetVersion() noexcept;

  ///Obtain the version history of this class
  static std::vector<std::string> GetVersionHistory() noexcept;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif

  const boost::shared_ptr<TextCanvas> ToCanvas(const int radius) const noexcept;

  private:
  ///The LED
  Led m_led;

  friend std::ostream& operator<<(std::ostream& os, const LedWidget& widget) noexcept;
};

std::ostream& operator<<(std::ostream& os, const LedWidget& widget) noexcept;
bool operator==(const LedWidget& lhs, const LedWidget& rhs) noexcept;
bool operator!=(const LedWidget& lhs, const LedWidget& rhs) noexcept;

} //~namespace ribi

#endif // LEDWIDGET_H
