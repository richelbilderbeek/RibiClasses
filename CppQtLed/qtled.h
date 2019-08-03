//---------------------------------------------------------------------------
/*
QtLed, Qt widget for displaying the Led class
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
//From http://www.richelbilderbeek.nl/CppQtLed.htm
//---------------------------------------------------------------------------
#ifndef QTLED_H
#define QTLED_H

#pragma GCC diagnostic push



#include <QWidget>
#pragma GCC diagnostic pop

#include "led.h"

namespace ribi {

struct Led;

///QtLed is the Qt display widhet of an LED
class QtLed : public QWidget
{
  Q_OBJECT
public:
  //Creates an Led
  explicit QtLed(
    const double intensity    = 0.0,
    const unsigned char red   = 255,
    const unsigned char green =   0,
    const unsigned char blue  =   0,
    QWidget *parent = 0
  );

  //Due to QWidget
  QtLed(const QtLed&) = delete;

  //Due to QWidget
  QtLed& operator=(const QtLed&) = delete;

  ///Draw a Led from a Led
  static void DrawLed(
    QPainter& painter,
    const int left, const int top,
    const int width, const int height,
    const Led& led
  ) noexcept;

  const Led& GetLed() const noexcept { return m_led; }
        Led& GetLed()       noexcept { return m_led; }
  static std::string GetVersion() noexcept;
  static std::vector<std::string> GetVersionHistory() noexcept;

public slots:
  void SetLed(const Led& led) noexcept;

protected:
  void paintEvent(QPaintEvent *);
  void resizeEvent(QResizeEvent *);

private:
  Led m_led;

  ///OnResize is called when the geometry of the LedWidget is changed
  //void OnResize();
  void OnColorChanged(Led * const) noexcept;
  void OnIntensityChanged(Led * const) noexcept;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

bool operator==(const QtLed& lhs, const QtLed& rhs) noexcept;
bool operator!=(const QtLed& lhs, const QtLed& rhs) noexcept;

} //~namespace ribi

#endif // QTLED_H
