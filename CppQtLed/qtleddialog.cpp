//---------------------------------------------------------------------------
/*
QtLedWidget, Qt led for displaying the Led class
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
#include "qtleddialog.h"





#include <cassert>
#include <sstream>

#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>

#include "led.h"
#include "testtimer.h"
#include "trace.h"
#include "ui_qtleddialog.h"


ribi::QtLedDialog::QtLedDialog(QWidget *parent)
  : QtHideAndShowDialog(parent),
    ui(new Ui::QtLedDialog),
    m_led{}
{
  #ifndef NDEBUG
  Test();
  #endif
  ui->setupUi(this);
}

ribi::QtLedDialog::~QtLedDialog() noexcept
{
  delete ui;
}

void ribi::QtLedDialog::on_box_intensity_valueChanged(double arg1)
{
  m_led.SetIntensity(arg1);
  emit on_led_changed(m_led);
}

void ribi::QtLedDialog::on_box_blue_valueChanged(int arg1)
{
  m_led.SetBlue(arg1);
  emit on_led_changed(m_led);
}

void ribi::QtLedDialog::on_box_green_valueChanged(int arg1)
{
  m_led.SetGreen(arg1);
  emit on_led_changed(m_led);
}

void ribi::QtLedDialog::on_box_red_valueChanged(int arg1)
{
  m_led.SetRed(arg1);
  emit on_led_changed(m_led);
}

void ribi::QtLedDialog::OnColorChanged(Led * const led) noexcept
{
  assert(led);
  ui->box_red->setValue(led->GetRed());
  ui->box_green->setValue(led->GetGreen());
  ui->box_blue->setValue(led->GetBlue());
}

void ribi::QtLedDialog::OnIntensityChanged(Led * const led) noexcept
{
  assert(led);
  ui->box_intensity->setValue(led->GetIntensity());
}

void ribi::QtLedDialog::SetLed(const Led& led) noexcept
{
  const bool verbose{false};

  if (m_led == led)
  {
    return;
  }
  if (verbose)
  {
    std::stringstream s;
    s << "Setting led '" << led << "'\n";
  }

  const auto blue_after      = led.GetBlue();
  const auto green_after     = led.GetGreen();
  const auto intensity_after = led.GetIntensity();
  const auto red_after       = led.GetRed();


  bool blue_changed  = true;
  bool green_changed  = true;
  bool intensity_changed = true;
  bool red_changed = true;

  {
    const auto blue_before  = m_led.GetBlue();
    const auto green_before  = m_led.GetGreen();
    const auto intensity_before = m_led.GetIntensity();
    const auto red_before = m_led.GetRed();

    blue_changed  = blue_before != blue_after;
    green_changed  = green_before != green_after;
    intensity_changed = intensity_before != intensity_after;
    red_changed = red_before != red_after;

    if (verbose)
    {
      if (blue_changed)
      {
        std::stringstream s;
        s
          << "Blue will change from "
          << blue_before
          << " to "
          << blue_after
          << '\n'
        ;
        TRACE(s.str());
        ui->box_blue->setValue(blue_after);
      }
      if (green_changed)
      {
        std::stringstream s;
        s << "Green will change from " << green_before
          << " to " << green_after << '\n';
        TRACE(s.str());
        ui->box_green->setValue(green_after);
      }
      if (intensity_changed)
      {
        std::stringstream s;
        s << "Intensity will change from " << intensity_before
          << " to " << intensity_after << '\n';
        TRACE(s.str());
        ui->box_intensity->setValue(intensity_after);
      }
      if (red_changed)
      {
        std::stringstream s;
        s << "Red will change from " << red_before
          << " to " << red_after << '\n';
        TRACE(s.str());
        ui->box_red->setValue(red_after);
      }
    }

  }

  //Replace m_led by the new one
  m_led = led;

  assert(m_led.GetBlue() == blue_after );
  assert(m_led.GetGreen()  == green_after );
  assert(m_led.GetIntensity() == intensity_after);
  assert(m_led.GetRed() == red_after);

  assert(led == m_led);
}



#ifndef NDEBUG
void ribi::QtLedDialog::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  {
    Led();
  }
  const TestTimer test_timer(__func__,__FILE__,1.0);
  QtLedDialog d;
  const Led led;
  d.SetLed(led);
  assert(d.GetLed() == led);
}
#endif
