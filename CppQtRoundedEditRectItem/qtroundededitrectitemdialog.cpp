//---------------------------------------------------------------------------
/*
QtRoundedEditRectItem, editable rectangular-shaped QGraphicsItem
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
//From http://www.richelbilderbeek.nl/QtRoundedEditRectItem.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtroundededitrectitemdialog.h"

#include <boost/algorithm/string/trim_all.hpp>
#include <boost/make_shared.hpp>
#include <boost/lambda/lambda.hpp>

#include <QKeyEvent>
#include <QFont>
#include <QFontDialog>
#include <QString>
#include "container.h"
#include "qtroundedrectitemdialog.h"
#include "qtroundededitrectitem.h"

#include "trace.h"
#include "ui_qtroundededitrectitemdialog.h"
#pragma GCC diagnostic pop

ribi::QtRoundedEditRectItemDialog::QtRoundedEditRectItemDialog(QWidget *parent)
  : QtHideAndShowDialog(parent),
    ui(new Ui::QtRoundedEditRectItemDialog),
    m_dialog{boost::make_shared<QtRoundedRectItemDialog>()},
    m_item{}
{
  ui->setupUi(this);

  assert(this->layout());
  this->layout()->addWidget(m_dialog.get());

  m_dialog->DisableSetSize();
}

ribi::QtRoundedEditRectItemDialog::~QtRoundedEditRectItemDialog() noexcept
{
  delete ui;
}


double ribi::QtRoundedEditRectItemDialog::GetUiX() const noexcept
{
  assert(m_dialog);
  return m_dialog->GetUiX();
}

double ribi::QtRoundedEditRectItemDialog::GetUiY() const noexcept
{
  assert(m_dialog);
  return m_dialog->GetUiY();
}

std::string ribi::QtRoundedEditRectItemDialog::GetVersion() noexcept
{
  return "1.3";
}

std::vector<std::string> ribi::QtRoundedEditRectItemDialog::GetVersionHistory() noexcept
{
  return {
    "2014-06-22: version 1.0: initial version",
    "2014-07-21: version 1.1: added CheckMe member function",
    "2014-07-31: version 1.2: fixed bug in SetText"
    "2014-08-04: version 1.3: removed CheckMe member function in favor of TDD"
  };
}

void ribi::QtRoundedEditRectItemDialog::keyPressEvent(QKeyEvent * event)
{
  if (event->key() == Qt::Key_Escape) { close(); return; }
}

void ribi::QtRoundedEditRectItemDialog::on_button_font_clicked()
{
  assert(m_item);
  static QFont font = m_item->GetFont();
  bool ok = false;
  const QFont new_font = QFontDialog::getFont(&ok, font, this);
  if (ok) { m_item->SetFont(new_font); }
}

void ribi::QtRoundedEditRectItemDialog::on_text_textChanged()
{
  const auto s = ui->text->toPlainText().toStdString();
  const auto text = Container().SeperateString(s,'\n');
  assert(m_item);
  m_item->SetText(text);
}

void ribi::QtRoundedEditRectItemDialog::on_button_text_pen_clicked()
{
  const QPen pen(
    QBrush(qRgb(std::rand() % 256,std::rand() % 256,std::rand() % 256)),
    1.0 + (static_cast<double>(std::rand() % 100) / 10.0)
  );
  assert(m_item);
  m_item->SetTextPen(pen);
}

void ribi::QtRoundedEditRectItemDialog::OnBaseChanged(QtRoundedEditRectItem * const
#ifndef NDEBUG
qtitem
#endif // NDEBUG
) noexcept
{
  assert(!"Am I called?");
  assert(m_item.get() == qtitem);
  //boost::shared_ptr<QtRoundedRectItem> base(m_item);
  m_dialog->SetItem(m_item);
}

void ribi::QtRoundedEditRectItemDialog::OnFontChanged(QtRoundedEditRectItem * const qtitem) noexcept
{
  assert(!"Am I called?");
  ui->label_font->setText(
    qtitem->GetFont().toString()
  );
}

void ribi::QtRoundedEditRectItemDialog::OnPaddingChanged(QtRoundedEditRectItem * const qtitem) noexcept
{
  assert(!"Am I called?");
  ui->box_padding_bottom->setValue(qtitem->GetPadding().bottom);
  ui->box_padding_left->setValue(qtitem->GetPadding().left);
  ui->box_padding_right->setValue(qtitem->GetPadding().right);
  ui->box_padding_top->setValue(qtitem->GetPadding().top);
}

void ribi::QtRoundedEditRectItemDialog::OnTextChanged(QtRoundedEditRectItem * const qtitem) noexcept
{
  assert(!"Am I called?");
  const std::string s{Container().Concatenate(qtitem->GetText(),"\n")};
  ui->text->setPlainText(s.c_str());
}

void ribi::QtRoundedEditRectItemDialog::OnTextPenChanged(QtRoundedEditRectItem * const qtitem) noexcept
{
  assert(!"Am I called?");
  std::stringstream s;
  s << "Text pen: " << qtitem->GetTextPen().widthF();
  ui->label_text_pen->setText(s.str().c_str());
}

void ribi::QtRoundedEditRectItemDialog::SetItem(
  const boost::shared_ptr<QtRoundedEditRectItem>& item
) noexcept
{
  m_item = item;
  assert( item ==  m_item);
  assert(*item == *m_item);
}

void ribi::QtRoundedEditRectItemDialog::SetUiX(const double x) noexcept
{
  this->m_dialog->SetUiX(x);
}

void ribi::QtRoundedEditRectItemDialog::SetUiY(const double y) noexcept
{
  this->m_dialog->SetUiY(y);
}

void ribi::QtRoundedEditRectItemDialog::on_box_padding_left_valueChanged(double arg1)
{
  auto padding = m_item->GetPadding();
  padding.left = arg1;
  m_item->SetPadding(padding);
}

void ribi::QtRoundedEditRectItemDialog::on_box_padding_top_valueChanged(double arg1)
{
  auto padding = m_item->GetPadding();
  padding.top = arg1;
  m_item->SetPadding(padding);
}

void ribi::QtRoundedEditRectItemDialog::on_box_padding_right_valueChanged(double arg1)
{
  auto padding = m_item->GetPadding();
  padding.right = arg1;
  m_item->SetPadding(padding);
}

void ribi::QtRoundedEditRectItemDialog::on_box_padding_bottom_valueChanged(double arg1)
{
  auto padding = m_item->GetPadding();
  padding.bottom = arg1;
  m_item->SetPadding(padding);
}
