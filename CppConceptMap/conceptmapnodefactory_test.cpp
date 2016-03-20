#include "conceptmapnodefactory.h"

#include <boost/test/unit_test.hpp>
#include "conceptmapnode.h"

BOOST_AUTO_TEST_CASE(ribi_concept_map_node_factory_test)
{
  using namespace ribi::cmap;
  //Number of tests
  {
    BOOST_CHECK(NodeFactory().GetNumberOfTests()
      == static_cast<int>(NodeFactory().GetTests().size())
    );
    BOOST_CHECK(NodeFactory().GetNumberOfNastyTests()
      == static_cast<int>(NodeFactory().GetNastyTests().size())
    );
  }
  //operator== and operator!=
  {
    BOOST_CHECK(NodeFactory().GetTest(0) == NodeFactory().GetTest(0));
    BOOST_CHECK(NodeFactory().GetTest(0) != NodeFactory().GetTest(1));
  }
  //Deep copy
  {
    const Node a = NodeFactory().GetTest(0);
    const Node b(a);
    BOOST_CHECK(b == NodeFactory().GetTest(0));
  }
  //XLM <-> std::string conversions
  {
    for (const auto node: NodeFactory().GetTests())
    {
      const std::string str{ToXml(node)};
      const Node node_again{XmlToNode(str)};
      BOOST_CHECK(node == node_again);
    }
  }
}
