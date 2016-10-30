#include "conceptmapexamples.h"

#include <boost/test/unit_test.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include "conceptmapexamplesfactory.h"
#include "trace.h"

BOOST_AUTO_TEST_CASE(ribi_concept_map_examples_test)
{
  using namespace ribi::cmap;
  //Test of operator== and operator!=
  {
    const int sz = ExamplesFactory().GetNumberOfTests();
    for (int i=0; i!=sz; ++i)
    {
      const Examples a = ExamplesFactory().GetTest(i);
            Examples b = ExamplesFactory().GetTest(i);
      BOOST_CHECK(a == a);
      BOOST_CHECK(a == b);
      BOOST_CHECK(b == a);
      BOOST_CHECK(b == b);
      for (int j=0; j!=sz; ++j)
      {
        const Examples c = ExamplesFactory().GetTest(j);
              Examples d = ExamplesFactory().GetTest(j);
        BOOST_CHECK(c == c);
        BOOST_CHECK(c == d);
        BOOST_CHECK(d == c);
        BOOST_CHECK(d == d);
        if (i==j)
        {
          BOOST_CHECK(a == c); BOOST_CHECK(a == d);
          BOOST_CHECK(b == c); BOOST_CHECK(b == d);
          BOOST_CHECK(c == a); BOOST_CHECK(c == b);
          BOOST_CHECK(d == a); BOOST_CHECK(d == b);
        }
        else
        {
          BOOST_CHECK(a != c); BOOST_CHECK(a != d);
          BOOST_CHECK(b != c); BOOST_CHECK(b != d);
          BOOST_CHECK(c != a); BOOST_CHECK(c != b);
          BOOST_CHECK(d != a); BOOST_CHECK(d != b);
        }
      }
    }
  }
  {
    const std::string xml = "<examples></examples>";
    const auto examples = XmlToExamples(xml);
    BOOST_CHECK(examples.Get().empty());
  }
  //Conversion between class and XML, test for equality
  {
    const std::vector<Examples> v = ExamplesFactory().GetTests();
    std::for_each(v.begin(),v.end(),
      [](const Examples& e)
      {
        const std::string s { ToXml(e) };
        const Examples f(XmlToExamples(s));
        BOOST_CHECK(e == f);
      }
    );
  }
  //Conversion between class and XML, test for equality
  {
    const std::vector<Examples> v = ExamplesFactory().GetNastyTests();
    std::for_each(v.begin(),v.end(),
      [](const Examples& e)
      {
        const std::string s { ToXml(e) };
        const Examples f(XmlToExamples(s));
        BOOST_CHECK(e == f);
      }
    );
  }
  {
    const std::vector<Examples> v = ExamplesFactory().GetTests();
    const int sz = boost::numeric_cast<int>(v.size());
    for (int i=0; i!=sz; ++i)
    {
      const Examples& e = v[i];
      const std::string s { ToXml(e) };
      for (int j=0; j!=sz; ++j)
      {
        const Examples& f = v[j];
        const std::string t { ToXml(f) };
        if (i == j)
        {
          BOOST_CHECK(e == f);
          BOOST_CHECK(s == t);
        }
        else
        {
          BOOST_CHECK(e != f);
          BOOST_CHECK(s != t);
        }
      }
    }
  }
  //Test if unrated and rated examples are noticed as different
  {
    const Example a("1",Competency::misc);
    const Example b("1",Competency::misc);
    const Example c("1",Competency::uninitialized);
    BOOST_CHECK(a == a); BOOST_CHECK(a == b); BOOST_CHECK(a != c);
    BOOST_CHECK(b == a); BOOST_CHECK(b == b); BOOST_CHECK(b != c);
    BOOST_CHECK(c != a); BOOST_CHECK(c != b); BOOST_CHECK(c == c);
    std::vector<Example> v; v.push_back(a);
    std::vector<Example> w; w.push_back(b);
    std::vector<Example> x; x.push_back(c);
    const Examples d(v);
    const Examples e(w);
    const Examples f(x);
    BOOST_CHECK(d == d); BOOST_CHECK(d == e); BOOST_CHECK(d != f);
    BOOST_CHECK(e == d); BOOST_CHECK(e == e); BOOST_CHECK(e != f);
    BOOST_CHECK(f != d); BOOST_CHECK(f != e); BOOST_CHECK(f == f);
  }
  //Single stream
  {
    const Examples e = ExamplesFactory().GetTest(1);
    std::stringstream s;
    s << e;
    Examples f;
    BOOST_CHECK(e != f);
    s >> f;
    if (e != f) { TRACE(e); TRACE(f); }
    BOOST_CHECK(e == f);
  }
  //Single stream
  {
    const Examples e = ExamplesFactory().GetTest(2);
    const Examples f = ExamplesFactory().GetTest(3);
    std::stringstream s;
    s << e << " " << f;
    Examples g;
    Examples h;
    s >> g >> h;
    if (e != g) { TRACE(e); TRACE(g); }
    if (f != h) { TRACE(f); TRACE(h); }
    BOOST_CHECK(e == g);
    BOOST_CHECK(f == h);
  }
  //Nasty examples
  for (const Examples e: ExamplesFactory().GetNastyTests())
  {
    std::stringstream s;
    s << e;
    Examples f;
    BOOST_CHECK(e != f);
    s >> f;
    if (e != f) { TRACE(e); TRACE(f); }
    BOOST_CHECK(e == f);
  }
  //Nasty examples
  for (const Examples e: ExamplesFactory().GetNastyTests())
  {
    std::stringstream s;
    s << e << " " << e;
    Examples g;
    Examples h;
    s >> g >> h;
    if (e != g) { TRACE(e); TRACE(g); }
    if (e != h) { TRACE(e); TRACE(h); }
    BOOST_CHECK(e == g);
    BOOST_CHECK(e == h);
  }
}
