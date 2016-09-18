//---------------------------------------------------------------------------
/*
QtRoundedRectItem, rectangular-shaped QGraphicsItem
Copyright (C) 2012-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppQtRoundedRectItem.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtroundedrectitemdialog.h"

#include <cassert>
#include <boost/make_shared.hpp>
#include <boost/lambda/lambda.hpp>

#include <QKeyEvent>

#include "geometry.h"
#include "qtroundedrectitem.h"
#include "ui_qtroundedrectitemdialog.h"
#pragma GCC diagnostic pop

ribi::QtRoundedRectItemDialog::QtRoundedRectItemDialog(QWidget *parent) :
    QtHideAndShowDialog(parent),
    ui(new Ui::QtRoundedRectItemDialog),
    m_item{}
{
  ui->setupUi(this);
}

ribi::QtRoundedRectItemDialog::~QtRoundedRectItemDialog() noexcept
{
  delete ui;
}

void ribi::QtRoundedRectItemDialog::DisableSetSize() noexcept
{
  ui->box_height->setReadOnly(true);
  ui->box_height_including_pen->setReadOnly(true);
  ui->box_width->setReadOnly(true);
  ui->box_width_including_pen->setReadOnly(true);
}

void ribi::QtRoundedRectItemDialog::DoSomethingRandom() noexcept
{
  ui->box_contour_pen_width->setValue(
    ui->box_contour_pen_width->value()
    + static_cast<double>(std::rand() % 3) - 1.0
  );

  ui->box_focus_pen_width->setValue(
    ui->box_focus_pen_width->value()
    + static_cast<double>(std::rand() % 3) - 1.0
  );

  ui->box_height->setValue(
    ui->box_height->value()
    + static_cast<double>(std::rand() % 3) - 1.0
  );

  ui->box_height_including_pen->setValue(
    ui->box_height_including_pen->value()
    + static_cast<double>(std::rand() % 3) - 1.0
  );

  ui->box_radius_x->setValue(
    ui->box_radius_x->value()
    + static_cast<double>(std::rand() % 3) - 1.0
  );

  ui->box_radius_y->setValue(
    ui->box_radius_y->value()
    + static_cast<double>(std::rand() % 3) - 1.0
  );

  ui->box_width->setValue(
    ui->box_width->value()
    + static_cast<double>(std::rand() % 3) - 1.0
  );

  ui->box_width_including_pen->setValue(
    ui->box_width_including_pen->value()
    + static_cast<double>(std::rand() % 3) - 1.0
  );

  ui->box_x->setValue(
    ui->box_x->value()
    + static_cast<double>(std::rand() % 3) - 1.0
  );

  ui->box_y->setValue(
    ui->box_y->value()
    + static_cast<double>(std::rand() % 3) - 1.0
  );
}

boost::shared_ptr<const ribi::QtRoundedRectItem> ribi::QtRoundedRectItemDialog::GetItem() const noexcept
{
  return m_item;
}

double ribi::QtRoundedRectItemDialog::GetUiX() const noexcept
{
  return ui->box_x->value();
}

double ribi::QtRoundedRectItemDialog::GetUiY() const noexcept
{
  return ui->box_y->value();
}

std::string ribi::QtRoundedRectItemDialog::GetVersion() noexcept
{
  return "1.3";
}

std::vector<std::string> ribi::QtRoundedRectItemDialog::GetVersionHistory() noexcept
{
  return {
    "2014-06-15: version 1.0: initial version",
    "2014-06-23: version 1.1: allow cooperation with QtRoundedEditRectItemDialog",
    "2014-07-21: version 1.2: added CheckMe member function, for checking its invariants",
    "2014-08-04: version 1.3: removed CheckMe member function in favor of TDD"
  };
}

void ribi::QtRoundedRectItemDialog::keyPressEvent(QKeyEvent * event)
{
  if (event->key() == Qt::Key_Escape) { close(); return; }
}

void ribi::QtRoundedRectItemDialog::OnContourPenChanged(QtRoundedRectItem * const qtitem) noexcept
{
  assert(!"Am I called?");
  ui->box_contour_pen_width->setValue(qtitem->GetContourPen().widthF());
}

void ribi::QtRoundedRectItemDialog::OnFocusPenChanged(QtRoundedRectItem * const qtitem) noexcept
{
  assert(!"Am I called?");
  ui->box_focus_pen_width->setValue(qtitem->GetFocusPen().widthF());
}

void ribi::QtRoundedRectItemDialog::OnPosChanged(QtRoundedRectItem * const qtitem) noexcept
{
  assert(!"Am I called?");
  const double new_x = qtitem->GetCenterPos().x();
  const double new_y = qtitem->GetCenterPos().y();
  ui->box_x->setValue(new_x);
  ui->box_y->setValue(new_y);
}

void ribi::QtRoundedRectItemDialog::OnRadiusXchanged(QtRoundedRectItem * const qtitem) noexcept
{
  assert(!"Am I called?");
  ui->box_radius_x->setValue(qtitem->GetRadiusX());
}

void ribi::QtRoundedRectItemDialog::OnRadiusYchanged(QtRoundedRectItem * const qtitem) noexcept
{
  assert(!"Am I called?");
  ui->box_radius_y->setValue(qtitem->GetRadiusY());
}

void ribi::QtRoundedRectItemDialog::OnWidthChanged(QtRoundedRectItem * const qtitem) noexcept
{
  assert(!"Am I called?");
  const double new_width = qtitem->GetInnerWidth();
  ui->box_width->setValue(new_width);
  const double new_width_including_pen = qtitem->GetOuterWidth();
  ui->box_width_including_pen->setValue(new_width_including_pen);
}

void ribi::QtRoundedRectItemDialog::OnHeightChanged(QtRoundedRectItem * const qtitem) noexcept
{
  assert(!"Am I called?");
  const double new_height = qtitem->GetInnerHeight();
  ui->box_height->setValue(new_height);
  const double new_height_including_pen = qtitem->GetOuterHeight();
  ui->box_height_including_pen->setValue(new_height_including_pen);
}

void ribi::QtRoundedRectItemDialog::on_box_contour_pen_width_valueChanged(double arg1)
{
  const QPen pen(
    QBrush(qRgb(0,0,0)),
    arg1
  );
  m_item->SetContourPen(pen);
}

void ribi::QtRoundedRectItemDialog::on_box_focus_pen_width_valueChanged(double arg1)
{
  const QPen pen(
    QBrush(qRgb(0,0,0)),
    arg1,
    Qt::DashLine
  );
  m_item->SetFocusPen(pen);
}

void ribi::QtRoundedRectItemDialog::on_box_height_valueChanged(double arg1)
{
  assert(1 + 1 == 2);
  m_item->SetInnerHeight(arg1);
}

void ribi::QtRoundedRectItemDialog::on_box_height_including_pen_valueChanged(double arg1)
{
  m_item->SetOuterHeight(arg1);
}

void ribi::QtRoundedRectItemDialog::on_box_radius_x_valueChanged(double arg1)
{
  m_item->SetRadiusX(arg1);
}

void ribi::QtRoundedRectItemDialog::on_box_radius_y_valueChanged(double arg1)
{
  m_item->SetRadiusY(arg1);
}

void ribi::QtRoundedRectItemDialog::on_box_width_valueChanged(double arg1)
{
  m_item->SetInnerWidth(arg1);
}

void ribi::QtRoundedRectItemDialog::on_box_width_including_pen_valueChanged(double arg1)
{
  m_item->SetOuterWidth(arg1);
}

void ribi::QtRoundedRectItemDialog::on_box_x_valueChanged(double arg1)
{
  m_item->SetCenterPos(arg1,m_item->GetCenterPos().y());
}

void ribi::QtRoundedRectItemDialog::on_box_y_valueChanged(double arg1)
{
  m_item->SetCenterPos(m_item->GetCenterPos().x(),arg1);
}

void ribi::QtRoundedRectItemDialog::SetItem(const boost::shared_ptr<QtRoundedRectItem>& item) noexcept
{
  const bool verbose{false};

  assert(item);
  if (m_item == item)
  {
    //Let the GUI follow
    ui->box_contour_pen_width->setValue(GetItem()->GetContourPen().widthF());
    ui->box_focus_pen_width->setValue(GetItem()->GetFocusPen().widthF());
    ui->box_height->setValue(GetItem()->GetInnerHeight());
    ui->box_height_including_pen->setValue(GetItem()->GetOuterHeight());
    ui->box_radius_x->setValue(GetItem()->GetRadiusX());
    ui->box_radius_y->setValue(GetItem()->GetRadiusY());
    ui->box_width->setValue(GetItem()->GetInnerWidth());
    ui->box_width_including_pen->setValue(GetItem()->GetOuterWidth());
    ui->box_x->setValue(GetItem()->GetCenterX());
    ui->box_y->setValue(GetItem()->GetCenterY());
    return;
  }
  if (verbose)
  {
    std::stringstream s;
    s << "Setting item '" << (*item) << "'\n";
  }

  //Replace m_item by the new one
  m_item = item;
}

void ribi::QtRoundedRectItemDialog::SetUiX(const double x) noexcept
{
  this->ui->box_x->setValue(x);
}

void ribi::QtRoundedRectItemDialog::SetUiY(const double y) noexcept
{
  this->ui->box_y->setValue(y);
}
