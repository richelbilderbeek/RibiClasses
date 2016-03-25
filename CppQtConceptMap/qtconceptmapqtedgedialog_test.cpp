#include "qtconceptmapqtedgedialog.h"
#include "qtconceptmapqtnodefactory.h"
#include "conceptmapnodefactory.h"
#include "conceptmapedgefactory.h"

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_CASE(ribi_cmap_qtedge_dialog_test)
{
  using namespace ribi::cmap;

  const auto from = NodeFactory().GetTest(1);
  const auto to = NodeFactory().GetTest(1);
  const auto edge = EdgeFactory().GetTest(1);
  const auto qtfrom = QtNodeFactory().Create(from);
  const auto qtto = QtNodeFactory().Create(to);
  const boost::shared_ptr<QtEdge> qtedge(new QtEdge(edge,qtfrom.get(),qtto.get()));
  QtQtEdgeDialog dialog(qtedge);

  //if (verbose) { TRACE("SetX and GetX must be symmetric"); }
  {
    const double new_x{dialog.GetUiX() + 10.0};
    dialog.SetUiX(new_x);
    BOOST_CHECK(std::abs(dialog.GetUiX() - new_x) < 2.0);
  }
  //if (verbose) { TRACE("SetY and GetY must be symmetric"); }
  {
    const double new_y{dialog.GetUiY() + 10.0};
    dialog.SetUiY(new_y);
    BOOST_CHECK(std::abs(dialog.GetUiY() - new_y) < 2.0);
  }
  dialog.SetQtEdge(nullptr);
  //dialog = QtQtEdgeDialog();
}
