//---------------------------------------------------------------------------
/*
QtLedWidget, Qt widget for displaying the Led class
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
//From http://www.richelbilderbeek.nl/CppQtLedWidget.htm
//---------------------------------------------------------------------------
#ifndef QTLEDDIALOG_H
#define QTLEDDIALOG_H

#include <string>
#include <vector>

#pragma GCC diagnostic push



#include "led.h"
#include "qthideandshowdialog.h"
#pragma GCC diagnostic pop

namespace Ui {
  class QtLedDialog;
}

namespace ribi {

struct Led;

///Displays an LED as a dialog
class QtLedDialog : public ::ribi::QtHideAndShowDialog
{
  Q_OBJECT

public:
  explicit QtLedDialog(QWidget *parent = 0);
  QtLedDialog(const QtLedDialog&) = delete;
  QtLedDialog& operator=(const QtLedDialog&) = delete;
  ~QtLedDialog() noexcept;

  void SetLed(const Led& led) noexcept;

  const Led& GetLed() const noexcept { return m_led; }
        Led& GetLed()       noexcept { return m_led; }


signals:
  void on_led_changed(const Led& led);

private slots:
  void on_box_blue_valueChanged(int arg1);
  void on_box_green_valueChanged(int arg1);
  void on_box_intensity_valueChanged(double arg1);
  void on_box_red_valueChanged(int arg1);

private:
  Ui::QtLedDialog *ui;

  ///The LED to work on
  Led m_led;

  void OnColorChanged(Led * const led) noexcept;
  void OnIntensityChanged(Led * const led) noexcept;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

} //~namespace ribi

#endif // QTLEDDIALOG_H
