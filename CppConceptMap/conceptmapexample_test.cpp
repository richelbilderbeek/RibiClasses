#include "conceptmapexample.h"
#include <boost/test/unit_test.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include "conceptmapexamplefactory.h"
#include "trace.h"

BOOST_AUTO_TEST_CASE(ribi_concept_map_example_test)
{
  using namespace ribi::cmap;
  //using ExampleFactory = ::ribi::cmap::ExampleFactory;
  const bool verbose{false};

  // Set and get must be symmetric
  {
    const Competency competency_before = Competency::uninitialized;
    const Competency competency_after = Competency::misc;
    const bool is_complex_before  = false;
    const bool is_complex_after   = true;
    const bool is_concrete_before = true;
    const bool is_concrete_after  = false;
    const bool is_specific_before = true;
    const bool is_specific_after  = false;
    const std::string text_before = "before";
    const std::string text_after  = "after";

    Example example(
      text_before,
      competency_before,
      is_complex_before,
      is_concrete_before,
      is_specific_before
    );
    assert(example.GetCompetency() == competency_before);
    assert(example.GetCompetency() != competency_after);
    assert(example.GetIsComplex() == is_complex_before);
    assert(example.GetIsComplex() != is_complex_after);
    assert(example.GetIsConcrete() == is_concrete_before);
    assert(example.GetIsConcrete() != is_concrete_after);
    assert(example.GetIsSpecific() == is_specific_before);
    assert(example.GetIsSpecific() != is_specific_after);
    assert(example.GetText() == text_before);
    assert(example.GetText() != text_after);

    example.SetCompetency(competency_after);
    example.SetIsComplex(is_complex_after);
    example.SetIsConcrete(is_concrete_after);
    example.SetIsSpecific(is_specific_after);
    example.SetText(text_after);

    assert(example.GetCompetency() != competency_before);
    assert(example.GetCompetency() == competency_after);
    assert(example.GetIsComplex() != is_complex_before);
    assert(example.GetIsComplex() == is_complex_after);
    assert(example.GetIsConcrete() != is_concrete_before);
    assert(example.GetIsConcrete() == is_concrete_after);
    assert(example.GetIsSpecific() != is_specific_before);
    assert(example.GetIsSpecific() == is_specific_after);
    assert(example.GetText() != text_before);
    assert(example.GetText() == text_after);
  }
  // Test of operator== and operator!=
  {
    const int sz = ExampleFactory().GetNumberOfTests();
    for (int i=0; i!=sz; ++i)
    {
      const Example a = ExampleFactory().GetTest(i);
            Example b = ExampleFactory().GetTest(i);
      assert(a == a);
      assert(a == b);
      assert(b == a);
      assert(b == b);
      for (int j=0; j!=sz; ++j)
      {
        const Example c = ExampleFactory().GetTest(j);
              Example d = ExampleFactory().GetTest(j);
        assert(c == c);
        assert(c == d);
        assert(d == c);
        assert(d == d);
        if (i==j)
        {
          assert(a == c); assert(a == d);
          assert(b == c); assert(b == d);
          assert(c == a); assert(c == b);
          assert(d == a); assert(d == b);
        }
        else
        {
          assert(a != c); assert(a != d);
          assert(b != c); assert(b != d);
          assert(c != a); assert(c != b);
          assert(d != a); assert(d != b);
        }
      }
    }
  }
  // Unrated and rated examples must be noticed as different
  {
    const Example a("1",Competency::misc);
    const Example b("1",Competency::misc);
    const Example c("1",Competency::uninitialized);
    assert(a == a); assert(a == b); assert(a != c);
    assert(b == a); assert(b == b); assert(b != c);
    assert(c != a); assert(c != b); assert(c == c);
  }
  //Conversion between std::string and competency
  //Checked by Competencies
  {
    const std::string xml = "<example><text>TEST</text>"
      "<competency>uninitialized</competency>"
      "<is_complex>1</is_complex><is_concrete>1</is_concrete>"
      "<is_specific>1</is_specific></example>"
    ;
    const auto example = XmlToExample(xml);
    assert(example.GetText() == "TEST");
  }
  // Conversion from class->XML->class must result in
  // something equal to the class
  {
    const std::vector<Example> v = ExampleFactory().GetTests();
    std::for_each(v.begin(),v.end(),
      [](const Example& e)
      {
        const std::string s { ToXml(e) };
        const Example f = XmlToExample(s);
        assert(e == f);
      }
    );
  }
  // Conversion from class->XML->class must result in
  // something equal to the class
  {
    const std::vector<Example> v = ExampleFactory().GetNastyTests();
    std::for_each(v.begin(),v.end(),
      [](const Example& e)
      {
        const std::string s { ToXml(e) };
        const Example f = XmlToExample(s);
        assert(e == f);
      }
    );
  }
  // Conversion from class->XML->class must differ between classes
  {
    const auto v = ExampleFactory().GetTests();
    const int sz = boost::numeric_cast<int>(v.size());
    for (int i=0; i!=sz; ++i)
    {
      const Example& e = v[i];
      const std::string s {ToXml(e) };
      for (int j=0; j!=sz; ++j)
      {
        const Example& f = v[j];
        const std::string t = ToXml(f);
        if (i == j)
        {
          assert(e == f);
          assert(s == t);
        }
        else
        {
          assert(e != f);
          assert(s != t);
        }
      }
    }
  }
  //Single stream
  {
    const Example e = ExampleFactory().GetTest(1);
    std::stringstream s;
    s << e;
    Example f;
    assert(e != f);
    s >> f;
    if (e != f) { TRACE(e); TRACE(f); }
    assert(e == f);
  }
  //Single stream
  {
    const Example e = ExampleFactory().GetTest(2);
    const Example f = ExampleFactory().GetTest(3);
    std::stringstream s;
    s << e << " " << f;
    Example g;
    Example h;
    s >> g >> h;
    if (e != g) { TRACE(e); TRACE(g); }
    if (f != h) { TRACE(f); TRACE(h); }
    assert(e == g);
    assert(f == h);
  }
  //Nasty examples
  for (const Example e: ExampleFactory().GetNastyTests())
  {
    std::stringstream s;
    s << e;
    Example f;
    assert(e != f);
    s >> f;
    if (e != f) { TRACE(e); TRACE(f); }
    assert(e == f);
  }
  //Nasty examples
  for (const Example e: ExampleFactory().GetNastyTests())
  {
    std::stringstream s;
    s << e << " " << e;
    Example g;
    Example h;
    s >> g >> h;
    if (e != g) { TRACE(e); TRACE(g); }
    if (e != h) { TRACE(e); TRACE(h); }
    assert(e == g);
    assert(e == h);
  }

}
