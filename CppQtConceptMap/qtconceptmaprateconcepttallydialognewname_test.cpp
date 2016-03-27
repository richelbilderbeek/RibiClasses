#include "qtconceptmaprateconcepttallydialognewname.h"
#include <boost/test/unit_test.hpp>

#include <sstream>
#include <numeric>

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <QKeyEvent>

#include "conceptmapconceptfactory.h"

#include "conceptmapcenternodefactory.h"
#include "conceptmap.h"
#include "conceptmapfactory.h"
#include "conceptmapconcept.h"
#include "conceptmapedge.h"
#include "conceptmapedgefactory.h"
#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"
#include "conceptmapexample.h"
#include "conceptmapedge.h"
#include "conceptmapexamples.h"
#include "testtimer.h"
#include "qtconceptmaprating.h"
#include "trace.h"
#include "ui_qtconceptmaprateconcepttallydialognewname.h"
#pragma GCC diagnostic pop

BOOST_AUTO_TEST_CASE(ribi_cmap_qtrateconcepttallydialog_test)
{
  using namespace ribi::cmap;
  //Empty table
  {
    const ConceptMap conceptmap;
    QtRateConceptTallyDialog d(conceptmap);
  }

  const ConceptMap conceptmap = QtRateConceptTallyDialog::CreateTestConceptMap();

  QtRateConceptTallyDialog d(conceptmap);

  #ifndef NDEBUG
  {
    if(d.GetUi()->table->columnCount() != 4) TRACE(d.GetUi()->table->columnCount());
    if(d.GetUi()->table->rowCount() != 3) TRACE(d.GetUi()->table->rowCount());
  }
  #endif

  BOOST_CHECK(d.GetUi()->table->columnCount() == 4);
  BOOST_CHECK(d.GetUi()->table->rowCount() == 3);
  BOOST_CHECK(boost::num_vertices(conceptmap) == 2);
  BOOST_CHECK(boost::num_edges(conceptmap) == 1);
  const Node focal_node = GetFocalNode(conceptmap);
  //const Node other_node = conceptmap.GetNodes()[1]; //Don't care
  const Edge edge = ribi::cmap::GetFirstEdge(conceptmap);

  BOOST_CHECK(d.GetUi()->table->item(0,0)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  BOOST_CHECK(d.GetUi()->table->item(0,1)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  BOOST_CHECK(d.GetUi()->table->item(0,2)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  BOOST_CHECK(d.GetUi()->table->item(0,3)->flags() == (Qt::ItemIsSelectable | Qt::ItemIsEnabled));

  BOOST_CHECK(d.GetUi()->table->item(1,0)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  BOOST_CHECK(d.GetUi()->table->item(1,1)->flags() == Qt::ItemIsEnabled); //Empty
  BOOST_CHECK(d.GetUi()->table->item(1,2)->flags() == Qt::ItemIsEnabled); //Empty
  BOOST_CHECK(d.GetUi()->table->item(1,3)->flags() == (Qt::ItemIsSelectable | Qt::ItemIsEnabled));

  BOOST_CHECK(d.GetUi()->table->item(2,0)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  BOOST_CHECK(d.GetUi()->table->item(2,1)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  BOOST_CHECK(d.GetUi()->table->item(2,2)->flags() == (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
  BOOST_CHECK(d.GetUi()->table->item(2,3)->flags() == (Qt::ItemIsSelectable | Qt::ItemIsEnabled));

  //Check current state, before modification

  BOOST_CHECK(d.GetUi()->table->item(0,0)->checkState() == (focal_node.GetConcept().GetExamples().Get()[0].GetIsComplex() ? Qt::Checked : Qt::Unchecked));
  BOOST_CHECK(d.GetUi()->table->item(0,1)->checkState() == (focal_node.GetConcept().GetExamples().Get()[0].GetIsConcrete() ? Qt::Checked : Qt::Unchecked));
  BOOST_CHECK(d.GetUi()->table->item(0,2)->checkState() == (focal_node.GetConcept().GetExamples().Get()[0].GetIsSpecific() ? Qt::Checked : Qt::Unchecked));
  BOOST_CHECK(d.GetUi()->table->item(0,3)->text() == QString(focal_node.GetConcept().GetExamples().Get()[0].GetText().c_str()));

  BOOST_CHECK(d.GetUi()->table->item(1,0)->checkState() == (edge.GetNode().GetConcept().GetIsComplex() ? Qt::Checked : Qt::Unchecked));
  BOOST_CHECK(d.GetUi()->table->item(1,1)->text() == "");
  BOOST_CHECK(d.GetUi()->table->item(1,2)->text() == "");
  //NEW 20131231: now the text contains both
  //- the concept name of the edge
  //- the name of the node the edge is connected to
  #ifdef NOT_NOW_20160201
  BOOST_CHECK(d.GetUi()->table->item(1,3)->text().toStdString().find(edge.GetNode().GetConcept().GetName()) != std::string::npos);
  BOOST_CHECK(d.GetUi()->table->item(1,3)->text().toStdString().find(edge.GetTo()->GetConcept().GetName()) != std::string::npos);
  //OLD BOOST_CHECK(d.GetUi()->table->item(1,3)->text() == QString(edge.GetConcept().GetName().c_str()));
  #endif // NOT_NOW_20160201

  BOOST_CHECK(d.GetUi()->table->item(2,0)->checkState() == (edge.GetNode().GetConcept().GetExamples().Get()[0].GetIsComplex() ? Qt::Checked : Qt::Unchecked));
  BOOST_CHECK(d.GetUi()->table->item(2,1)->checkState() == (edge.GetNode().GetConcept().GetExamples().Get()[0].GetIsConcrete() ? Qt::Checked : Qt::Unchecked));
  BOOST_CHECK(d.GetUi()->table->item(2,2)->checkState() == (edge.GetNode().GetConcept().GetExamples().Get()[0].GetIsSpecific() ? Qt::Checked : Qt::Unchecked));
  BOOST_CHECK(d.GetUi()->table->item(2,3)->text() == QString(edge.GetNode().GetConcept().GetExamples().Get()[0].GetText().c_str()));

  //Modify table
  d.GetUi()->table->item(0,0)->setCheckState(d.GetUi()->table->item(0,0)->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
  d.GetUi()->table->item(0,1)->setCheckState(d.GetUi()->table->item(0,1)->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
  d.GetUi()->table->item(0,2)->setCheckState(d.GetUi()->table->item(0,2)->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
  //d.GetUi()->table->item(0,3)->setText("MODIFIED"); //User should not be able to modify this

  d.GetUi()->table->item(1,0)->setCheckState(d.GetUi()->table->item(1,0)->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
  //d.GetUi()->table->item(1,3)->setText("MODIFIED TOO"); //User should not be able to modify this

  d.GetUi()->table->item(2,0)->setCheckState(d.GetUi()->table->item(2,0)->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
  d.GetUi()->table->item(2,1)->setCheckState(d.GetUi()->table->item(2,1)->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
  d.GetUi()->table->item(2,2)->setCheckState(d.GetUi()->table->item(2,2)->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
  //d.GetUi()->table->item(2,3)->setText("MODIFIED AS WELL"); //User should not be able to modify this

  //Check that data is modified by GUI

  BOOST_CHECK(d.GetUi()->table->item(0,0)->checkState() == (focal_node.GetConcept().GetExamples().Get()[0].GetIsComplex() ? Qt::Checked : Qt::Unchecked));
  BOOST_CHECK(d.GetUi()->table->item(0,1)->checkState() == (focal_node.GetConcept().GetExamples().Get()[0].GetIsConcrete() ? Qt::Checked : Qt::Unchecked));
  BOOST_CHECK(d.GetUi()->table->item(0,2)->checkState() == (focal_node.GetConcept().GetExamples().Get()[0].GetIsSpecific() ? Qt::Checked : Qt::Unchecked));
  BOOST_CHECK(d.GetUi()->table->item(0,3)->text() == QString(focal_node.GetConcept().GetExamples().Get()[0].GetText().c_str()));

  BOOST_CHECK(d.GetUi()->table->item(1,0)->checkState() == (edge.GetNode().GetConcept().GetIsComplex() ? Qt::Checked : Qt::Unchecked));
  BOOST_CHECK(d.GetUi()->table->item(1,1)->text() == "");
  BOOST_CHECK(d.GetUi()->table->item(1,2)->text() == "");

  //NEW 20131231: now the text contains both
  //- the concept name of the edge
  //- the name of the node the edge is connected to
  #ifdef NOT_NOW_20160201
  BOOST_CHECK(d.GetUi()->table->item(1,3)->text().toStdString().find(edge.GetNode().GetConcept().GetName()) != std::string::npos);
  BOOST_CHECK(d.GetUi()->table->item(1,3)->text().toStdString().find(edge.GetTo()->GetConcept().GetName()) != std::string::npos);
  //OLD BOOST_CHECK(d.GetUi()->table->item(1,3)->text() == QString(edge.GetConcept().GetName().c_str()));
  #endif // NOT_NOW_20160201

  BOOST_CHECK(d.GetUi()->table->item(2,0)->checkState() == (edge.GetNode().GetConcept().GetExamples().Get()[0].GetIsComplex() ? Qt::Checked : Qt::Unchecked));
  BOOST_CHECK(d.GetUi()->table->item(2,1)->checkState() == (edge.GetNode().GetConcept().GetExamples().Get()[0].GetIsConcrete() ? Qt::Checked : Qt::Unchecked));
  BOOST_CHECK(d.GetUi()->table->item(2,2)->checkState() == (edge.GetNode().GetConcept().GetExamples().Get()[0].GetIsSpecific() ? Qt::Checked : Qt::Unchecked));
  BOOST_CHECK(d.GetUi()->table->item(2,3)->text() == QString(edge.GetNode().GetConcept().GetExamples().Get()[0].GetText().c_str()));

}
