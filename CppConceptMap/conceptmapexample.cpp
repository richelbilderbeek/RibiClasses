//---------------------------------------------------------------------------
/*
ConceptMap, concept map classes
Copyright (C) 2013-2015 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppConceptMap.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "conceptmapexample.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <sstream>

#include <boost/lexical_cast.hpp>
#include <QRegExp>

#include "counter.h"
#include "conceptmapregex.h"
#include "conceptmapcompetencies.h"
#include "conceptmapcompetency.h"
#include "conceptmapexample.h"
#include "conceptmapexamplefactory.h"
#include "conceptmaphelper.h"
#include "ribi_regex.h"
#include "testtimer.h"
#include "trace.h"
#include "xml.h"
#pragma GCC diagnostic pop

ribi::cmap::Example::Example(
  const std::string& text,
  const cmap::Competency competency,
  const bool is_complex,
  const bool is_concrete,
  const bool is_specific
  )
  : m_competency(competency),
    m_is_complex(is_complex),
    m_is_concrete(is_concrete),
    m_is_specific(is_specific),
    m_text(text)
{
  #ifndef NDEBUG
  Test();
  #endif
}

void ribi::cmap::Example::SetCompetency(const Competency competency) noexcept
{
  m_competency = competency;
}

void ribi::cmap::Example::SetIsComplex(const bool is_complex) noexcept
{
  m_is_complex = is_complex;
}

void ribi::cmap::Example::SetIsConcrete(const bool is_concrete) noexcept
{
  m_is_concrete = is_concrete;
}

void ribi::cmap::Example::SetIsSpecific(const bool is_specific) noexcept
{
  m_is_specific = is_specific;
}

void ribi::cmap::Example::SetText(const std::string& text) noexcept
{
  if (m_text != text)
  {
    m_text = text;
  }
}

#ifndef NDEBUG
void ribi::cmap::Example::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  {
    ribi::Regex();
    ribi::cmap::Regex();
    Counter();
    Example();
    ExampleFactory();
    Examples();
    //ExamplesFactory();
  }
  const TestTimer test_timer(__func__,__FILE__,1.0);
  const bool verbose{false};


  if (verbose) { TRACE("Set and get must be symmetric"); }
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
  if (verbose) { TRACE("Test of operator== and operator!="); }
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
  if (verbose) { TRACE("Unrated and rated examples must be noticed as different"); }
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
    const std::string xml = "<example><text>TEST</text><competency>uninitialized</competency><is_complex>1</is_complex><is_concrete>1</is_concrete><is_specific>1</is_specific></example>";
    const auto example = XmlToExample(xml);
    assert(example.GetText() == "TEST");
  }

  if (verbose) { TRACE("Conversion from class->XML->class must result in something equal to the class"); }
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
  if (verbose) { TRACE("Conversion from class->XML->class must result in something equal to the class"); }
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
  if (verbose) { TRACE("Conversion from class->XML->class must differ between classes"); }
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
    s << e << f;
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
    s << e << e;
    Example g;
    Example h;
    s >> g >> h;
    if (e != g) { TRACE(e); TRACE(g); }
    if (e != h) { TRACE(e); TRACE(h); }
    assert(e == g);
    assert(e == h);
  }

}
#endif

std::string ribi::cmap::Example::ToStr() const noexcept
{
  std::stringstream s;
  s
    << GetText() << " "
    << Competencies().ToStr(GetCompetency()) << " "
    << GetIsComplex() << " "
    << GetIsConcrete() << " "
    << GetIsSpecific()
  ;
  return s.str();

}

std::string ribi::cmap::ToXml(const Example& example) noexcept
{
  std::stringstream s;
  s << "<example>";
  s <<   "<text>";
  s <<     example.GetText();
  s <<   "</text>";
  s <<   "<competency>";
  s << Competencies().ToStr(example.GetCompetency());
  s <<   "</competency>";
  s <<   "<is_complex>";
  s <<     example.GetIsComplex();
  s <<   "</is_complex>";
  s <<   "<is_concrete>";
  s <<     example.GetIsConcrete();
  s <<   "</is_concrete>";
  s <<   "<is_specific>";
  s <<     example.GetIsSpecific();
  s <<   "</is_specific>";
  s << "</example>";

  const std::string r = s.str();
  assert(r.size() >= 17);
  assert(r.substr(0,9) == "<example>");
  assert(r.substr(r.size() - 10,10) == "</example>");
  return r;
}

ribi::cmap::Example ribi::cmap::XmlToExample(const std::string& s) noexcept
{
  if (s.size() < 17)
  {
    std::stringstream msg;
    msg << __func__ << ": XML string '" << s << "' is only " << s.size() << " characters long, need at least 27";
    throw std::logic_error(msg.str());
  }
  if (s.substr(0,9) != "<example>")
  {
    std::stringstream msg;
    msg << __func__ << ": XML string '" << s << "' does not begin with <example>";
    throw std::logic_error(msg.str());
  }
  if (s.substr(s.size() - 10,10) != "</example>")
  {
    std::stringstream msg;
    msg << __func__ << ": XML string '" << s << "' does not end with </example>";
    throw std::logic_error(msg.str());
  }

  assert(s.size() >= 17);
  assert(s.substr(0,9) == "<example>");
  assert(s.substr(s.size() - 10,10) == "</example>");


  std::string text;
  cmap::Competency competency = cmap::Competency::uninitialized;
  bool is_complex = false;
  bool is_concrete = false;
  bool is_specific = false;

  //competency
  {
    const std::vector<std::string> v
      = Regex().GetRegexMatches(s,Regex().GetRegexCompetency());
    assert(v.size() == 1);
    competency = Competencies().ToType(ribi::xml::StripXmlTag(v[0]));
    //competency = Example::StrToCompetency(ribi::xml::StripXmlTag(v[0]));
  }
  //is_complex
  {
    const std::vector<std::string> v
      = Regex().GetRegexMatches(s,Regex().GetRegexIsComplex());
    assert(v.size() == 1);
    is_complex = boost::lexical_cast<bool>(ribi::xml::StripXmlTag(v[0]));
  }
  //is_concrete
  {
    const std::vector<std::string> v
      = Regex().GetRegexMatches(s,Regex().GetRegexIsConcrete());
    assert(v.size() == 1);
    is_concrete = boost::lexical_cast<bool>(ribi::xml::StripXmlTag(v[0]));
  }
  //is_specific
  {
    const std::vector<std::string> v
      = Regex().GetRegexMatches(s,Regex().GetRegexIsSpecific());
    assert(v.size() == 1);
    is_specific = boost::lexical_cast<bool>(ribi::xml::StripXmlTag(v[0]));
  }
  //text
  {
    const std::vector<std::string> v
      = Regex().GetRegexMatches(s,Regex().GetRegexText());
    assert(v.size() == 1 && "GetRegexText must be present once in an Example");
    text = ribi::xml::StripXmlTag(v[0]);
  }

  Example example(
    text,
    competency,
    is_complex,
    is_concrete,
    is_specific
  );
  assert(ToXml(example) == s);
  return example;
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const Example& example) noexcept
{
  os << ToXml(example);
  return os;
}

std::istream& ribi::cmap::operator>>(std::istream& is, Example& example) noexcept
{
  //eat until '</example>'
  is >> std::noskipws;
  std::string s;
  while (1)
  {
    char c;
    is >> c;
    s += c;
    if(s.size() > 10 && s.substr(s.size() - 10,10) == "</example>") break;
  }
  example = XmlToExample(s);
  return is;
}

bool ribi::cmap::operator==(const Example& lhs, const Example& rhs) noexcept
{
  return
       lhs.GetText() == rhs.GetText()
    && lhs.GetCompetency() == rhs.GetCompetency();
}

bool ribi::cmap::operator!=(const Example& lhs, const Example& rhs) noexcept
{
  return !(lhs == rhs);
}

bool ribi::cmap::operator<(const Example& lhs,const Example& rhs) noexcept
{
  if (lhs.GetText() < rhs.GetText()) return true;
  if (lhs.GetText() > rhs.GetText()) return false;
  return lhs.GetCompetency() < rhs.GetCompetency();
}

bool ribi::cmap::operator>(const Example& lhs,const Example& rhs) noexcept
{
  if (lhs.GetText() > rhs.GetText()) return true;
  if (lhs.GetText() < rhs.GetText()) return false;
  return lhs.GetCompetency() > rhs.GetCompetency();
}
