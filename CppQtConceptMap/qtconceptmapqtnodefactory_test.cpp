#include <boost/test/unit_test.hpp>
#include "qtconceptmapqtnodefactory.h"

BOOST_AUTO_TEST_CASE(ribi_cmap_qtnode_factory_create_all_qtnodes)
{
  using namespace ribi::cmap;
  const QtNodeFactory f;
  const int n = f.GetNumberOfTests();
  for (int i=0; i!=n; ++i)
  {
    const auto qtnode = f.GetTest(i);
    BOOST_CHECK(qtnode);
  }
}
