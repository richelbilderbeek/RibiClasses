#include "qtconceptmapnodedialog.h"
#include <boost/test/unit_test.hpp>

#include <boost/bind/bind.hpp>
#include <boost/lambda/lambda.hpp>

#include "conceptmapcompetencies.h"
#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "conceptmapexamples.h"
#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"
#include "qtconceptmapconceptdialog.h"
#include "qtconceptmapexamplesdialog.h"
#include "ui_qtconceptmapnodedialog.h"
#include "testtimer.h"
#include "trace.h"

BOOST_AUTO_TEST_CASE(ribi_cmap_qtnode_dialog_test)
{
  using namespace ribi::cmap;
  QtNodeDialog dialog;
  Node node(NodeFactory().GetTest(1));
  dialog.SetNode(node);
  //if (verbose) { TRACE("X of QtNode and QtNodeDialog must match at start"); }
  {
    BOOST_CHECK(std::abs(dialog.GetUiX() - node.GetX()) < 2.0);
  }
}
