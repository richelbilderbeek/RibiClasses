#include "conceptmapcenternodefactory.h"

#include <boost/test/unit_test.hpp>
#include "conceptmapconceptfactory.h"

BOOST_AUTO_TEST_CASE(ribi_concept_map_center_node_factory_test)
{
  using namespace ribi::cmap;

  //Create from Concept constructor
  {
    const auto concept = ConceptFactory().GetTest(0);
    const double x{0.1};
    const double y{2.3};
    const auto node = CenterNodeFactory().Create(concept,x,y);
    BOOST_CHECK(node.IsCenterNode());
    BOOST_CHECK(concept == node.GetConcept());
    BOOST_CHECK(node.GetX() == x);
    BOOST_CHECK(node.GetY() == y);
  }
  {
    const std::string xml = "<node><concept><name>Concept without examples</name><examples></examples><concept_is_complex>0</concept_is_complex><complexity>0</complexity><concreteness>1</concreteness><specificity>2</specificity></concept><x>0.1</x><y>2.3</y><is_center_node>1</is_center_node></node>";
    const auto node = XmlToNode(xml);
    BOOST_CHECK(node.IsCenterNode());
  }
}

