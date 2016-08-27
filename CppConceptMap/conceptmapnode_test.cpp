#include "conceptmapnode.h"
#include <boost/test/unit_test.hpp>

#include "conceptmapnodefactory.h"
#include "conceptmapconceptfactory.h"
#include "trace.h"

BOOST_AUTO_TEST_CASE(ribi_cmap_node_copy_of_default_should_match)
{
  using namespace ribi::cmap;
  const Node a;
  const Node b(a);
  BOOST_CHECK(a == b);
}

BOOST_AUTO_TEST_CASE(ribi_cmap_node_copy_of_tests_should_mis_and_match)
{
  using namespace ribi::cmap;
  const Node a = NodeFactory().GetTest(1);
  const Node b = NodeFactory().GetTest(2);
  BOOST_CHECK(a != b);
  Node c(a);
  BOOST_CHECK(c == a);
  BOOST_CHECK(c != b);
  c = b;
  BOOST_CHECK(c != a);
  BOOST_CHECK(c == b);
}

BOOST_AUTO_TEST_CASE(ribi_cmap_node_set_and_get_x)
{
  using namespace ribi::cmap;
  Node a;
  a.SetX(1.2);
  BOOST_CHECK(a.GetX() == 1.2);
  a.SetY(3.4);
  BOOST_CHECK(a.GetX() == 1.2);
  BOOST_CHECK(a.GetY() == 3.4);
  a.SetX(5.6);
  BOOST_CHECK(a.GetX() == 5.6);
  BOOST_CHECK(a.GetY() == 3.4);
}

BOOST_AUTO_TEST_CASE(ribi_cmap_node_stream_one)
{
  using namespace ribi::cmap;
  Node a{NodeFactory().GetTest(1)};
  std::stringstream s;
  s << a;
  Node b;
  s >> b;
  BOOST_CHECK(a == b);
}


BOOST_AUTO_TEST_CASE(ribi_cmap_node_test)
{
  using namespace ribi::cmap;
  {
    const std::vector<Node> v = Node::GetTests();
    std::for_each(v.begin(),v.end(),
      [](const Node& node)
      {
        //Test copy constructor
        const Node c(node);
        BOOST_CHECK(node == c);
        const std::string s{ToXml(c)};
        const Node d = XmlToNode(s);
        BOOST_CHECK(c == d);
      }
    );
  }
  //Test HasSameContent
  {
    {
      const Concept c(ConceptFactory().Create("1"));
      const Concept d(ConceptFactory().Create("1"));
      BOOST_CHECK(c == d);
      const Node a{c};
      const Node b{d};
      BOOST_CHECK(HasSameContent(a,b));
      BOOST_CHECK(a == b);
    }
    const int sz = static_cast<int>(ConceptFactory().GetTests().size());
    for (int i=0; i!=sz; ++i)
    {
      const Concept c = ConceptFactory().Create(
        "1", { {"2", Competency::uninitialized} }
      );
      const Concept d = ConceptFactory().Create(
        "1", { {"2", Competency::uninitialized} }
      );
      BOOST_CHECK(c == d);
      const Node a{c};
      const Node b{d};
      BOOST_CHECK(HasSameContent(a,b));
    }

    {
      //Cannot shuffle Concept its examples. No need to as well: the order is important
      const Concept c = ConceptFactory().Create(
        "1",
        {
          {"2", Competency::uninitialized},{"3", Competency::uninitialized}
        }
      );
      const Concept d = ConceptFactory().Create(
        "1",
        {
          {"2", Competency::uninitialized},{"3", Competency::uninitialized}
        }
      );
      BOOST_CHECK(c == d);
      const Node a{c};
      const Node b{d};
      BOOST_CHECK(HasSameContent(a,b));
      BOOST_CHECK(a == b);
    }
    {
      //Cannot shuffle Concept its examples. No need to as well: the order is important
      const Concept c = ConceptFactory().Create(
        "1",
        {
          {"2", Competency::uninitialized},{"3", Competency::uninitialized}
        }
      );
      const Concept d = ConceptFactory().Create(
        "1",
        {
          {"3", Competency::uninitialized},{"2", Competency::uninitialized}
        }
      );
      BOOST_CHECK(c != d);
      const Node a{c};
      const Node b{d};
      BOOST_CHECK(!HasSameContent(a,b) && "Order in examples is important and cannot be shuffled");
      BOOST_CHECK(a != b);
    }
    {
      //Cannot shuffle Concept its examples. No need to as well: the order is important
      const Concept c = ConceptFactory().Create(
        "1",
        {
          {"2", Competency::uninitialized},{"3", Competency::uninitialized}
        }
      );
      const Concept d = ConceptFactory().Create("1", { {"2", Competency::uninitialized} } );
      BOOST_CHECK(c != d);
      const Node a{c};
      const Node b{d};
      BOOST_CHECK(a != b);
      BOOST_CHECK(!HasSameContent(a,b));
    }
  }
  //Test ConceptFactory reproductions
  {
    const int sz = static_cast<int>(ConceptFactory().GetTests().size());
    for (int i=0; i!=sz; ++i)
    {
      BOOST_CHECK(i < static_cast<int>(ConceptFactory().GetTests().size()));
      const Concept c = ConceptFactory().GetTests()[i];
      const Concept d = ConceptFactory().GetTests()[i];
      BOOST_CHECK(c == d);
      const Node a{c};
      const Node b{d};
      BOOST_CHECK(HasSameContent(a,b));
      BOOST_CHECK(a == b);
    }
  }
  //Test ConceptFactory reproductions
  {
    const int sz = static_cast<int>(ConceptFactory().GetTests().size());
    for (int i=0; i!=sz; ++i)
    {
      for (int j=0; j!=sz; ++j)
      {
        BOOST_CHECK(i < static_cast<int>(ConceptFactory().GetTests().size()));
        const Concept c = ConceptFactory().GetTests()[i];
        const Concept d = ConceptFactory().GetTests()[j];
        if (i!=j)
        {
          BOOST_CHECK(c != d);
          const Node a{c};
          const Node b{d};
          BOOST_CHECK(!HasSameContent(a,b));
          BOOST_CHECK(a != b);
        }
        else
        {
          BOOST_CHECK(c == d);
          const Node a{c};
          const Node b{d};
          BOOST_CHECK(HasSameContent(a,b));
          BOOST_CHECK(a == b);
        }
      }
    }
  }
  //Stream two
  {
    const Node e = NodeFactory().GetTest(1);
    const Node f = NodeFactory().GetTest(2);
    std::stringstream s;
    s << e << " " << f;
    Node g;
    Node h;
    s >> g >> h;
    if (e != g) { TRACE(e); TRACE(g); }
    if (f != h) { TRACE(f); TRACE(h); }
    BOOST_CHECK(e == g);
    BOOST_CHECK(f == h);
  }
  //Nasty examples
  for (const Node e: NodeFactory().GetNastyTests())
  {
    std::stringstream s;
    s << e;
    Node f;
    BOOST_CHECK(e != f);
    s >> f;
    if (e != f) { TRACE(e); TRACE(f); }
    BOOST_CHECK(e == f);
  }
  //Nasty examples
  for (const Node e: NodeFactory().GetNastyTests())
  {
    std::stringstream s;
    s << e << " " << e;
    Node g;
    Node h;
    s >> g >> h;
    if (e != g) { TRACE(e); TRACE(g); }
    if (e != h) { TRACE(e); TRACE(h); }
    BOOST_CHECK(e == g);
    BOOST_CHECK(e == h);
  }
}
