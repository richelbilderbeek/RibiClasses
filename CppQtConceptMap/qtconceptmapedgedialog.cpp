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
#include "qtconceptmapedgedialog.h"

#include <cassert>
#include <boost/bind/bind.hpp>
#include <boost/lambda/lambda.hpp>

#include <QLabel>

#include "conceptmapedge.h"
#include "conceptmapedgefactory.h"
#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"
#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "qtconceptmapexampledialog.h"
#include "qtconceptmapexamplesdialog.h"
#include "qtconceptmapconceptdialog.h"
#include "qtconceptmapnodedialog.h"
#include "ui_qtconceptmapedgedialog.h"

#include "trace.h"

#pragma GCC diagnostic pop

ribi::cmap::QtEdgeDialog::QtEdgeDialog(
  const Edge& edge,
  QWidget *parent
)
  : ribi::QtHideAndShowDialog(parent),
    ui(new Ui::QtEdgeDialog),
    m_edge{edge},
    m_qtnodedialog{new QtNodeDialog},
    m_qtnodedialog_from{new QtNodeDialog},
    m_qtnodedialog_to{new QtNodeDialog}
{
  ui->setupUi(this);
  {
    assert(ui->groupBox->layout());
    const auto my_layout = ui->groupBox->layout();
    {
      QLabel * const label = new QLabel("Center node:");
      my_layout->addWidget(label);
    }
    my_layout->addWidget(m_qtnodedialog.get());
    {
      QLabel * const label = new QLabel("From node:");
      my_layout->addWidget(label);
    }
    my_layout->addWidget(m_qtnodedialog_from.get());
    {
      QLabel * const label = new QLabel("To node:");
      my_layout->addWidget(label);
    }
    my_layout->addWidget(m_qtnodedialog_to.get());
  }
}

ribi::cmap::QtEdgeDialog::~QtEdgeDialog()
{
  delete ui;
}

int ribi::cmap::QtEdgeDialog::GetMinimumHeight(const Edge& ) noexcept
{
  return 400;
}


bool ribi::cmap::QtEdgeDialog::GetUiHasHeadArrow() const noexcept
{
  return ui->box_head_arrow->isChecked();
}

bool ribi::cmap::QtEdgeDialog::GetUiHasTailArrow() const noexcept
{
  return ui->box_tail_arrow->isChecked();
}

double ribi::cmap::QtEdgeDialog::GetUiX() const noexcept
{
  return m_qtnodedialog->GetUiX();
}

double ribi::cmap::QtEdgeDialog::GetUiY() const noexcept
{
  return m_qtnodedialog->GetUiY();
}

void ribi::cmap::QtEdgeDialog::SetEdge(const Edge& edge)
{
  const bool verbose{false};

  if (m_edge == edge)
  {
    return;
  }

  if (verbose)
  {
    std::stringstream s;
    s << "Setting edge '" << edge.ToStr() << "'\n";
  }
  const auto node_after = edge.GetNode();

  bool node_changed  = true;

  {
    const auto node_before = m_edge.GetNode();


    node_changed = node_before != node_after;


    if (verbose)
    {
      if (node_changed)
      {
        std::stringstream s;
        s
          << "Node will change from "
          << node_before.ToStr()
          << " to "
          << node_after.ToStr()
          << '\n'
        ;
        TRACE(s.str());
      }
    }
  }

  //Replace m_example by the new one
  m_edge = edge;

  assert(m_edge.GetNode() == node_after );

  setMinimumHeight(GetMinimumHeight(m_edge));

  assert(edge == m_edge);
}

//void ribi::cmap::QtEdgeDialog::OnFromChanged(Edge * const edge)
//{
//  const bool verbose{false};
//  assert(edge);

//  const Node from_before = m_qtnodedialog_from->GetNode();
//  const Node * from_after = edge.GetFrom();
//  assert(from_after);

//  if (verbose)
//  {
//    std::stringstream s;
//    s << "Change from from "
//      << from_before.ToStr()
//      << " to " << from_after->ToStr()
//    ;
//    TRACE(s.str());
//  }

//  m_qtnodedialog_from->SetNode(*from_after);

//  assert(m_qtnodedialog_from->GetNode() == *from_after);
//}

//void ribi::cmap::QtEdgeDialog::OnHeadArrowChanged(Edge * const edge)
//{
//  const bool verbose{false};
//  assert(edge);

//  const auto head_arrow_before = ui->box_head_arrow->isChecked();
//  const auto head_arrow_after = edge.HasHeadArrow();

//  if (verbose)
//  {
//    std::stringstream s;
//    s << "Change head arrow from "
//      << head_arrow_before
//      << " to " << head_arrow_after
//    ;
//    TRACE(s.str());
//  }

//  ui->box_head_arrow->setChecked(head_arrow_after);

//  assert(ui->box_head_arrow->isChecked() == head_arrow_after);
//}

//void ribi::cmap::QtEdgeDialog::OnNodeChanged(Edge * const edge)
//{
//  const bool verbose{false};
//  assert(edge);

//  const auto node_before = m_qtnodedialog->GetNode();
//  const auto node_after = edge.GetNode();
//  if (verbose)
//  {
//    std::stringstream s;
//    s << "Change center node from "
//    << node_before.ToStr()
//    << " to " << node_after.ToStr();
//    TRACE(s.str());
//  }

//  m_qtnodedialog->SetNode(node_after);

//  assert(m_qtnodedialog->GetNode() == node_after);
//}


//void ribi::cmap::QtEdgeDialog::OnTailArrowChanged(Edge * const edge)
//{
//  const bool verbose{false};
//  assert(edge);

//  const auto tail_arrow_before = ui->box_tail_arrow->isChecked();
//  const auto tail_arrow_after = edge.HasTailArrow();

//  if (verbose)
//  {
//    std::stringstream s;
//    s << "Change tail arrow from "
//      << tail_arrow_before
//      << " to " << tail_arrow_after
//    ;
//    TRACE(s.str());
//  }

//  ui->box_tail_arrow->setChecked(tail_arrow_after);

//  assert(ui->box_tail_arrow->isChecked() == tail_arrow_after);
//}


//void ribi::cmap::QtEdgeDialog::OnToChanged(Edge * const edge)
//{
//  const bool verbose{false};
//  assert(edge);

//  const auto to_before = m_qtnodedialog_to->GetNode();
//  const auto to_after = edge.GetTo();

//  if (verbose)
//  {
//    std::stringstream s;
//    s << "Change to from "
//      << to_before.ToStr()
//      << " to " << to_after->ToStr()
//    ;
//    TRACE(s.str());
//  }

//  m_qtnodedialog_to->SetNode(*to_after);

//  assert(m_qtnodedialog_to->GetNode() == *to_after);
//}

void ribi::cmap::QtEdgeDialog::SetUiHasHeadArrow(const bool has_head) noexcept
{
  ui->box_head_arrow->setChecked(has_head);
}

void ribi::cmap::QtEdgeDialog::SetUiHasTailArrow(const bool has_tail) noexcept
{
  ui->box_tail_arrow->setChecked(has_tail);
}

void ribi::cmap::QtEdgeDialog::SetUiX(const double x) noexcept
{
  m_qtnodedialog->SetUiX(x);
}


void ribi::cmap::QtEdgeDialog::SetUiY(const double y) noexcept
{
  m_qtnodedialog->SetUiY(y);
}

void ribi::cmap::QtEdgeDialog::on_box_head_arrow_stateChanged(int)
{

}

void ribi::cmap::QtEdgeDialog::on_box_tail_arrow_stateChanged(int)
{

}
