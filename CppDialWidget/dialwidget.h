//---------------------------------------------------------------------------
/*
DialWidget, class for displaying a Dial
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
//From http://www.richelbilderbeek.nl/CppDialWidget.htm
//---------------------------------------------------------------------------
#ifndef DIALWIDGET_H
#define DIALWIDGET_H

#include <iosfwd>
#include <string>
#include <vector>




#include "dial.h"
#include "drawcanvas.h"
#include "widget.h"


namespace ribi {

struct Dial;
struct DrawCanvas;
struct TextCanvas;

///DialWidget is a class to display a Dial
struct DialWidget : public Widget
{
  explicit DialWidget(
    const double position = 0.0,
    const int x = 0,
    const int y = 0,
    const int width = 100,
    const int height = 100,
    const unsigned char red = 255,
    const unsigned char green = 255,
    const unsigned char blue = 255
  );

  ///Click on the Dial
  void Click(const int x, const int y) noexcept;

  const Dial& GetDial() const noexcept { return m_dial; }
        Dial& GetDial()       noexcept { return m_dial; }

  ///Obtain this class its version
  static std::string GetVersion() noexcept;

  ///Obtain this class its version history
  static std::vector<std::string> GetVersionHistory() noexcept;

  ///Is the dial clicked?
  bool IsClicked(const int x, const int y) const noexcept;

  ///Convert to a DrawCanvas
  DrawCanvas ToDrawCanvas(const int radius) const noexcept;

  ///Convert to a TextCanvas
  TextCanvas ToTextCanvas(const int radius) const noexcept;

  private:
  Dial m_dial;

  friend std::ostream& operator<<(std::ostream& os, const DialWidget& widget) noexcept;
};

std::ostream& operator<<(std::ostream& os, const DialWidget& widget) noexcept;

} //~namespace ribi

#endif // DIALWIDGET_H
