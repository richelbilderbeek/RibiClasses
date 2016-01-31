//---------------------------------------------------------------------------
/*
QtConceptMap, Qt classes for display and interaction with ConceptMap
Copyright (C) 2013-2016 Richel Bilderbeek

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
//From http://www.richelbilderbeek.nl/CppQtConceptMap.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtconceptmapqtedgedialog.h"

#include <cassert>
#include <sstream>

#include <boost/lambda/lambda.hpp>

#include <QDesktopWidget>
#include <QVBoxLayout>

#include "conceptmapedgefactory.h"
#include "conceptmapedge.h"
#include "conceptmapnodefactory.h"
#include "qtconceptmapedgedialog.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmapqtnodefactory.h"
#include "qtquadbezierarrowitem.h"
#include "qtitemdisplaystrategy.h"
#include "qtroundededitrectitemdialog.h"
#include "qtroundededitrectitem.h"
#include "testtimer.h"
#include "trace.h"
#include "ui_qtconceptmapqtedgedialog.h"
#pragma GCC diagnostic pop

ribi::cmap::QtQtEdgeDialog::QtQtEdgeDialog(
  const boost::shared_ptr<QtEdge>& qtedge,
  QWidget *parent
)
  : QtHideAndShowDialog(parent),
  ui(new Ui::QtQtEdgeDialog),
  m_qtedge{},
  m_qtedgedialog{new QtEdgeDialog(qtedge->GetEdge())},
  m_qtroundededitrectitem_dialog{new QtRoundedEditRectItemDialog}
{
  #ifndef NDEBUG
  Test();
  #endif
  ui->setupUi(this);

  {
    assert(!this->ui->groupBox->layout());
    //assert(!this->layout());
    QVBoxLayout * const my_layout{new QVBoxLayout};
    this->ui->groupBox->setLayout(my_layout);
    //this->setLayout(my_layout);
    my_layout->addWidget(m_qtedgedialog.get());
    my_layout->addWidget(m_qtroundededitrectitem_dialog.get());
  }

  {
    //Put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->setGeometry(0,0,screen.width() * 9 / 10, screen.height() * 9 / 10);
    this->move( screen.center() - this->rect().center() );
  }
}

ribi::cmap::QtQtEdgeDialog::~QtQtEdgeDialog()
{
  this->layout()->removeWidget(m_qtedgedialog.get());
  this->layout()->removeWidget(m_qtroundededitrectitem_dialog.get());
  this->SetQtEdge(nullptr);
  delete ui;
}

int ribi::cmap::QtQtEdgeDialog::GetMinimumHeight(const QtEdge& qtedge) noexcept
{
  const int margin = 16;
  return
    QtEdgeDialog::GetMinimumHeight(qtedge.GetEdge())
  + margin
  + QtRoundedEditRectItemDialog::GetMinimumHeight()
  ;
}

bool ribi::cmap::QtQtEdgeDialog::GetUiHasHeadArrow() const noexcept
{
  return this->m_qtedgedialog->GetUiHasHeadArrow();
}

bool ribi::cmap::QtQtEdgeDialog::GetUiHasTailArrow() const noexcept
{
  return this->m_qtedgedialog->GetUiHasTailArrow();
}

double ribi::cmap::QtQtEdgeDialog::GetUiX() const noexcept
{
  return this->m_qtedgedialog->GetUiX();
}

double ribi::cmap::QtQtEdgeDialog::GetUiY() const noexcept
{
  return this->m_qtedgedialog->GetUiY();
}

void ribi::cmap::QtQtEdgeDialog::OnQtRoundedRectItemChanged(QtEdge * const qtedge) noexcept
{
  m_qtedgedialog->SetEdge(qtedge->GetEdge());
}

void ribi::cmap::QtQtEdgeDialog::SetQtEdge(const boost::shared_ptr<QtEdge>& qtedge) noexcept
{
  const bool verbose{false};

  if (m_qtedge == qtedge)
  {
    return;
  }

  if (verbose)
  {
    std::stringstream s;
    s << "Setting edge '" << qtedge->ToStr() << "'\n";
  }


  bool qtroundededitrectitem_changed{true};
  bool edge_changed{true};

  if (m_qtedge && qtedge)
  {
    const auto qtroundededitrectitem_after = qtedge.get();
    const auto edge_after = qtedge->GetEdge();

    const auto qtroundededitrectitem_before = m_qtedge.get();
    const auto edge_before = m_qtedge->GetEdge();

    qtroundededitrectitem_changed = qtroundededitrectitem_before != qtroundededitrectitem_after;
    edge_changed = edge_before != edge_after;


    if (verbose)
    {
      if (qtroundededitrectitem_changed)
      {
        std::stringstream s;
        s
          << "DisplayStrategy will change from "
          << qtroundededitrectitem_before->ToStr()
          << " to "
          << qtroundededitrectitem_after->ToStr()
          << '\n'
        ;
        TRACE(s.str());
      }
      if (edge_changed)
      {
        std::stringstream s;
        s << "QtEdge will change from " << (edge_before)
          << " to " << (edge_after) << '\n';
        TRACE(s.str());
      }
    }
  }

  if (m_qtedge)
  {
    //Disconnect old
    m_qtedge->m_signal_base_changed.disconnect(
      boost::bind(&ribi::cmap::QtQtEdgeDialog::OnQtRoundedRectItemChanged,this,boost::lambda::_1)
    );
  }

  //Replace by the new
  m_qtedge = qtedge;

  if (!m_qtedge) return;

  m_qtedge->m_signal_base_changed.connect(
    boost::bind(&ribi::cmap::QtQtEdgeDialog::OnQtRoundedRectItemChanged,this,boost::lambda::_1)
  );

  //Emit everything that has changed
  if (qtroundededitrectitem_changed)
  {
    m_qtedge->m_signal_base_changed(m_qtedge.get());
  }
  if (edge_changed)
  {
    m_qtedge->m_signal_edge_changed(m_qtedge.get());
  }

  this->setMinimumHeight(
    this->GetMinimumHeight(
      *m_qtedge
    )
  );

  assert( qtedge ==  m_qtedge);
  assert(*qtedge == *m_qtedge);
}

void ribi::cmap::QtQtEdgeDialog::SetUiHasHeadArrow(const bool has_head) noexcept
{
  this->m_qtedgedialog->SetUiHasHeadArrow(has_head);
}

void ribi::cmap::QtQtEdgeDialog::SetUiHasTailArrow(const bool has_tail) noexcept
{
  this->m_qtedgedialog->SetUiHasTailArrow(has_tail);
}

void ribi::cmap::QtQtEdgeDialog::SetUiX(const double x) const noexcept
{
  this->m_qtedgedialog->SetUiX(x);
}

void ribi::cmap::QtQtEdgeDialog::SetUiY(const double y) const noexcept
{
  this->m_qtedgedialog->SetUiY(y);
}

#ifndef NDEBUG
void ribi::cmap::QtQtEdgeDialog::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  {
    NodeFactory();
    QtNodeFactory();
    QtRoundedEditRectItemDialog();
    QtRoundedEditRectItem();
    const auto from = NodeFactory().GetTest(1);
    const auto to = NodeFactory().GetTest(1);
    const auto qtfrom = QtNodeFactory().Create(from);
    const auto qtto = QtNodeFactory().Create(to);
    const Edge edge = EdgeFactory().GetTest(1);
    QtEdgeDialog qtedgedial(edge);
    //QtEdge(edge,qtfrom.get(),qtto.get());
    QtEdge qtedge(edge,qtfrom.get(),qtto.get());
  }

  const TestTimer test_timer(__func__,__FILE__,1.0);
  const bool verbose{false};

  const auto from = NodeFactory().GetTest(1);
  const auto to = NodeFactory().GetTest(1);
  const auto edge = EdgeFactory().GetTest(1);
  const auto qtfrom = QtNodeFactory().Create(from);
  const auto qtto = QtNodeFactory().Create(to);
  const boost::shared_ptr<QtEdge> qtedge(new QtEdge(edge,qtfrom.get(),qtto.get()));
  QtQtEdgeDialog dialog(qtedge);

  if (verbose) { TRACE("SetX and GetX must be symmetric"); }
  {
    const double new_x{dialog.GetUiX() + 10.0};
    dialog.SetUiX(new_x);
    assert(std::abs(dialog.GetUiX() - new_x) < 2.0);
  }
  if (verbose) { TRACE("SetY and GetY must be symmetric"); }
  {
    const double new_y{dialog.GetUiY() + 10.0};
    dialog.SetUiY(new_y);
    assert(std::abs(dialog.GetUiY() - new_y) < 2.0);
  }
  dialog.SetQtEdge(nullptr);
  //dialog = QtQtEdgeDialog();
}
#endif
