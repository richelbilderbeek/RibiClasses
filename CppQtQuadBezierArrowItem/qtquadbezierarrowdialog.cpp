#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtquadbezierarrowdialog.h"

#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>

#include <QGraphicsItem>
#include <QKeyEvent>

#include <cassert>

#include "qtquadbezierarrowitem.h"

#include "ui_qtquadbezierarrowdialog.h"
#pragma GCC diagnostic pop

ribi::QtQuadBezierArrowDialog::QtQuadBezierArrowDialog(QWidget *parent)
  : QtHideAndShowDialog(parent),
    ui(new Ui::QtQuadBezierArrowDialog),
    m_arrow{}
{
  ui->setupUi(this);
}

ribi::QtQuadBezierArrowDialog::~QtQuadBezierArrowDialog() noexcept
{
  delete ui;
}

double ribi::QtQuadBezierArrowDialog::GetUiMidX() const noexcept
{
  return ui->box_mid_x->value();
}

double ribi::QtQuadBezierArrowDialog::GetUiMidY() const noexcept
{
  return ui->box_mid_y->value();
}

std::string ribi::QtQuadBezierArrowDialog::GetVersion() noexcept
{
  return "1.0";
}


std::vector<std::string> ribi::QtQuadBezierArrowDialog::GetVersionHistory() noexcept
{
  return {
    "2014-07-29: version 1.0: initial version"
  };
}

void ribi::QtQuadBezierArrowDialog::on_box_from_x_valueChanged(double arg1)
{
  this->m_arrow->SetFromX(arg1);
}

void ribi::QtQuadBezierArrowDialog::on_box_from_y_valueChanged(double arg1)
{
  this->m_arrow->SetFromY(arg1);
}

void ribi::QtQuadBezierArrowDialog::on_box_has_head_clicked()
{
  m_arrow->SetHasHead(ui->box_has_head->isChecked());
}

void ribi::QtQuadBezierArrowDialog::on_box_has_tail_clicked()
{
  m_arrow->SetHasTail(ui->box_has_tail->isChecked());
}

void ribi::QtQuadBezierArrowDialog::on_box_mid_x_valueChanged(double arg1)
{
  this->m_arrow->SetMidX(arg1);
}

void ribi::QtQuadBezierArrowDialog::on_box_mid_y_valueChanged(double arg1)
{
  this->m_arrow->SetMidY(arg1);
}

void ribi::QtQuadBezierArrowDialog::on_box_normal_pen_width_valueChanged(double arg1)
{
  QPen new_pen = m_arrow->GetPen();
  new_pen.setWidthF(arg1);
  m_arrow->SetPen(new_pen);
}

void ribi::QtQuadBezierArrowDialog::on_box_focus_pen_width_valueChanged(double arg1)
{
  QPen new_pen = m_arrow->GetFocusPen();
  new_pen.setWidthF(arg1);
  m_arrow->SetFocusPen(new_pen);

}

void ribi::QtQuadBezierArrowDialog::on_box_to_x_valueChanged(double arg1)
{
  this->m_arrow->SetToX(arg1);
}

void ribi::QtQuadBezierArrowDialog::on_box_to_y_valueChanged(double arg1)
{
  this->m_arrow->SetToY(arg1);
}

void ribi::QtQuadBezierArrowDialog::OnItemUpdated(const QtQuadBezierArrowItem * const item) noexcept
{
  assert(!"Am I called?");
  ui->box_focus_pen_width->setValue(item->GetFocusPen().widthF());
  ui->box_from_x->setValue(item->GetFromItem()->x());
  ui->box_from_y->setValue(item->GetFromItem()->y());
  ui->box_has_head->setChecked(item->HasHead());
  ui->box_has_tail->setChecked(item->HasTail());
  ui->box_mid_x->setValue(item->GetMidItem()->x());
  ui->box_mid_y->setValue(item->GetMidItem()->y());
  ui->box_normal_pen_width->setValue(item->GetPen().widthF());
  ui->box_to_x->setValue(item->GetToItem()->x());
  ui->box_to_y->setValue(item->GetToItem()->y());
}

void ribi::QtQuadBezierArrowDialog::keyPressEvent(QKeyEvent * event) noexcept
{
  if (event->key() == Qt::Key_Escape) { close(); return; }
}

void ribi::QtQuadBezierArrowDialog::SetArrow(const Arrow& arrow) noexcept
{
  assert(arrow);
  m_arrow = arrow;
  assert( arrow ==  m_arrow);
  assert(*arrow == *m_arrow);
}

void ribi::QtQuadBezierArrowDialog::SetUiMidX(const double x) noexcept
{
  ui->box_mid_x->setValue(x);
}

void ribi::QtQuadBezierArrowDialog::SetUiMidY(const double y) noexcept
{
  ui->box_mid_y->setValue(y);
}
