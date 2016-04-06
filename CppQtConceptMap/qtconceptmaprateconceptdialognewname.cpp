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
#include "qtconceptmaprateconceptdialognewname.h"

#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QKeyEvent>

#include "conceptmapconceptfactory.h"
#include "conceptmapconcept.h"
#include "conceptmapfactory.h"
#include "conceptmap.h"
#include "conceptmapedge.h"

#include "conceptmapexamplesfactory.h"
#include "conceptmapexamples.h"
//#include "conceptmapfile.h"
#include "conceptmapnodefactory.h"
#include "conceptmapnode.h"
#include "qtconceptmaprating.h"
#include "qtconceptmap.h"
#include "qtconceptmaprateconcepttallydialognewname.h"
#include "trace.h"
#include "ui_qtconceptmaprateconceptdialognewname.h"
#pragma GCC diagnostic pop


ribi::cmap::QtRateConceptDialog::QtRateConceptDialog(
  const ConceptMap conceptmap,
  QWidget* parent)
  : QtHideAndShowDialog(parent),
    ui(new Ui::QtRateConceptDialog),
    m_button_ok_clicked(false),
    m_concept{GetFocalNode(conceptmap).GetConcept()},
    m_initial_complexity(GetFocalNode(conceptmap).GetConcept().GetRatingComplexity()),
    m_initial_concreteness(GetFocalNode(conceptmap).GetConcept().GetRatingConcreteness()),
    m_initial_specificity(GetFocalNode(conceptmap).GetConcept().GetRatingSpecificity()),
    m_conceptmap(conceptmap),
    m_widget(new QtConceptMap)
{
  ui->setupUi(this);
  m_widget->SetConceptMap(conceptmap);
  assert(boost::num_vertices(m_conceptmap));

  assert(m_widget);
  assert(ui->conceptmap_layout);

  ui->conceptmap_layout->addWidget(m_widget.get());

  ui->box_complexity->setCurrentIndex(m_initial_complexity);
  ui->box_concreteness->setCurrentIndex(m_initial_concreteness);
  ui->box_specificity->setCurrentIndex(m_initial_specificity);
  ui->box_complexity->setFocus();

  //Set suggestions
  {
    const std::string s = "Formeel uitgangspunt: "
      + boost::lexical_cast<std::string>(cmap::Rating().SuggestComplexity(m_conceptmap,*vertices(m_conceptmap).first));
    ui->box_complexity->setToolTip(s.c_str());
  }
  {
    const std::string s = "Formeel uitgangspunt: "
      + boost::lexical_cast<std::string>(cmap::Rating().SuggestConcreteness(m_conceptmap,*vertices(m_conceptmap).first));
    ui->box_concreteness->setToolTip(s.c_str());
  }
  {
    const std::string s = "Formeel uitgangspunt: "
      + boost::lexical_cast<std::string>(cmap::Rating().SuggestSpecificity(m_conceptmap,*vertices(m_conceptmap).first));
    ui->box_specificity->setToolTip(s.c_str());
  }

  //Center the dialog
  {
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->setGeometry(screen.adjusted(64,64,-64,-64));
    this->move( screen.center() - this->rect().center() );
  }
  //The rating by the Tally dialog must be visible as of 2013-08-30
  //so let this dialog follow the ratings done by the tally dialog
}

ribi::cmap::QtRateConceptDialog::~QtRateConceptDialog() noexcept
{
  //If user clicked OK, keep the current ratings (which are updated by the comboboxes)
  //else the user cancelled, so put back the initial ratings
  if (!m_button_ok_clicked)
  {
    m_concept.SetRatingComplexity(m_initial_complexity);
    m_concept.SetRatingConcreteness(m_initial_concreteness);
    m_concept.SetRatingSpecificity(m_initial_specificity);
  }
  delete ui;
}

void ribi::cmap::QtRateConceptDialog::keyPressEvent(QKeyEvent* e)
{
  if (e->key()  == Qt::Key_Escape) { close(); return; }
  if ( (e->modifiers() & Qt::ControlModifier)
    && (e->modifiers() & Qt::ShiftModifier)
    && e->key() == Qt::Key_T)
  {
    //Translate
    this->setWindowTitle("Assess cluster");
    ui->button_ok->setText("OK");
    ui->button_tally_relevancies->setText("Evaluate illustrations");
    ui->label_complexity->setText("Complexity");
    ui->label_concreteness->setText("Concreteness");
    ui->label_specificity->setText("Specificity");
/*

De relevantie van voorbeelden en relaties		Relevance of illustrations
Voorbeelden/toelichting bij concept: Illustrations and relations of the cluster:

*/
    return;
  }

  //QDialog::keyPressEvent(e);
}

void ribi::cmap::QtRateConceptDialog::on_button_ok_clicked()
{
  //Ratings already set by comboboxes
  m_button_ok_clicked = true;
  close();
}

void ribi::cmap::QtRateConceptDialog::OnRatingComplexityChanged(const ribi::cmap::Concept& concept)
{
  if (ui->box_complexity->currentIndex() != concept.GetRatingComplexity())
  {
    ui->box_complexity->setCurrentIndex(concept.GetRatingComplexity());
  }
}

void ribi::cmap::QtRateConceptDialog::OnRatingConcretenessChanged(const ribi::cmap::Concept& concept)
{
  if (ui->box_concreteness->currentIndex() != concept.GetRatingConcreteness())
  {
    ui->box_concreteness->setCurrentIndex(concept.GetRatingConcreteness());
  }
}

void ribi::cmap::QtRateConceptDialog::OnRatingSpecificityChanged(const ribi::cmap::Concept& concept)
{
  if (ui->box_specificity->currentIndex() != concept.GetRatingSpecificity())
  {
    ui->box_specificity->setCurrentIndex(concept.GetRatingSpecificity());
  }
}

void ribi::cmap::QtRateConceptDialog::on_button_tally_relevancies_clicked()
{
 QtRateConceptTallyDialog d(m_conceptmap);
  d.exec(); //Keep this dialog visible, as of 2013-08-30
  ui->box_complexity->setCurrentIndex(d.GetSuggestedComplexity());
  ui->box_concreteness->setCurrentIndex(d.GetSuggestedConcreteness());
  ui->box_specificity->setCurrentIndex(d.GetSuggestedSpecificity());
}

void ribi::cmap::QtRateConceptDialog::on_box_complexity_currentIndexChanged(int)
{
  if (m_concept.GetRatingComplexity() != ui->box_complexity->currentIndex())
  {
    m_concept.SetRatingComplexity(ui->box_complexity->currentIndex());
  }
}

void ribi::cmap::QtRateConceptDialog::on_box_concreteness_currentIndexChanged(int)
{
  if (m_concept.GetRatingConcreteness() != ui->box_concreteness->currentIndex())
  {
    m_concept.SetRatingConcreteness(ui->box_concreteness->currentIndex());
  }
}

void ribi::cmap::QtRateConceptDialog::on_box_specificity_currentIndexChanged(int)
{
  if (m_concept.GetRatingSpecificity() != ui->box_specificity->currentIndex())
  {
    m_concept.SetRatingSpecificity(ui->box_specificity->currentIndex());
  }
}
