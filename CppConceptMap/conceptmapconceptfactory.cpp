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
#include "conceptmapconceptfactory.h"

#include "conceptmapcompetency.h"
#include "conceptmapconcept.h"
#include "conceptmapexample.h"
#include "conceptmapexamples.h"
#include "conceptmapexamplefactory.h"
#include "conceptmapexamplesfactory.h"
#include "conceptmaphelper.h"
#include "conceptmapregex.h"
#include "testtimer.h"
#include "counter.h"
#include "trace.h"
#include "xml.h"
#pragma GCC diagnostic push

ribi::cmap::ConceptFactory::ConceptFactory() noexcept
{
  #ifndef NDEBUG
  Test();
  #endif
}

ribi::cmap::Concept ribi::cmap::ConceptFactory::Create(
  const std::string& name,
  const Examples& examples,
  const bool is_complex,
  const int rating_complexity,
  const int rating_concreteness,
  const int rating_specificity
) const noexcept
{
  assert(rating_complexity >= -1);
  assert(rating_complexity <=  2);

  Concept concept(
    name,
    examples,
    is_complex,
    rating_complexity,
    rating_concreteness,
    rating_specificity
  );
  return concept;
}

ribi::cmap::Concept ribi::cmap::ConceptFactory::Create(
  const std::string& name,
  const std::vector<std::pair<std::string,Competency> >& v,
  const bool is_complex,
  const int rating_complexity,
  const int rating_concreteness,
  const int rating_specificity
) const noexcept
{
  assert(rating_complexity >= -1);
  assert(rating_complexity <=  2);

  std::vector<Example> w;
  std::transform(v.begin(),v.end(),std::back_inserter(w),
    [](const std::pair<std::string,Competency>& p)
    {
      const Example q
        = ExampleFactory().Create(
          p.first,
          p.second
      );
      return q;
    }
  );

  const Examples examples = ExamplesFactory().Create(w);

  Concept concept
    = Create(
    name,
    examples,
    is_complex,
    rating_complexity,
    rating_concreteness,
    rating_specificity
  );
  return concept;
}

ribi::cmap::Concept ribi::cmap::ConceptFactory::FromXml(const std::string& s) const noexcept
{
  assert(s.size() >= 19);
  assert(s.substr(0,9) == "<concept>");
  assert(s.substr(s.size() - 10,10) == "</concept>");

  std::string name;
  Examples examples;
  bool is_complex = false;
  int rating_complexity    = -2; //Not even unrated (which has -1 as its value)
  int rating_concreteness  = -2; //Not even unrated (which has -1 as its value)
  int rating_specificity   = -2; //Not even unrated (which has -1 as its value)
  //m_name
  {
    const std::vector<std::string> v
      = Regex().GetRegexMatches(s,Regex().GetRegexName());
    #ifndef NDEBUG
    if (v.size() != 1)
    {
      TRACE("ERROR");
      TRACE(s);
      TRACE(Regex().GetRegexName());
      TRACE(v.size());
      for (const auto& t: v) { TRACE(t); }
      TRACE("BREAK");
    }
    #endif
    assert(v.size() == 1);
    name = ribi::xml::StripXmlTag(v[0]);
  }
  //m_examples
  {
    const std::vector<std::string> v
      = Regex().GetRegexMatches(s,Regex().GetRegexExamples());
    assert(v.size() == 1 && "GetRegexExamples must be present once in a Concept");
    examples = ExamplesFactory().FromXml(v[0]);
  }

  //m_is_complex
  {
    const std::vector<std::string> v
      = Regex().GetRegexMatches(s,Regex().GetRegexConceptIsComplex());
    assert(v.size() == 1 && "GetRegexIsComplex must be present once per Concept");
    is_complex = boost::lexical_cast<bool>(ribi::xml::StripXmlTag(v[0]));
  }


  //m_rating_complexity
  {
    const std::vector<std::string> v
      = Regex().GetRegexMatches(s,Regex().GetRegexComplexity());
    assert(v.size() == 1 && "GetRegexComplexity must be present once per Concept");
    rating_complexity = boost::lexical_cast<int>(ribi::xml::StripXmlTag(v[0]));
    assert(rating_complexity >= -1);
    assert(rating_complexity <=  2);
  }
  //m_rating_concreteness
  {
    const std::vector<std::string> v
      = Regex().GetRegexMatches(s,Regex().GetRegexConcreteness());
    assert(v.size() == 1);
    rating_concreteness = boost::lexical_cast<int>(ribi::xml::StripXmlTag(v[0]));
  }
  //m_rating_specificity
  {
    const std::vector<std::string> v
      = Regex().GetRegexMatches(s,Regex().GetRegexSpecificity());
    assert(v.size() == 1);
    rating_specificity = boost::lexical_cast<int>(ribi::xml::StripXmlTag(v[0]));
  }

  Concept concept {
    ConceptFactory().Create(name,examples,is_complex,rating_complexity,rating_concreteness,rating_specificity)
  };
  return concept;
}

ribi::cmap::Concept ribi::cmap::ConceptFactory::GetTest(
  const int i) const noexcept
{
  assert(i < GetNumberOfTests());
  const Concept concept {
    GetTests()[i]
  };
  return concept;
}

std::vector<ribi::cmap::Concept> ribi::cmap::ConceptFactory::GetTests() const noexcept
{
  std::vector<Concept> v;
  {
    const Examples examples = ExamplesFactory().Create();
    const Concept p = Create("Concept without examples", examples, false, 0, 1, 2);
    assert(p.GetRatingComplexity() >= -1);
    assert(p.GetRatingComplexity() <=  2);
    v.push_back(p);
  }
  {
    const Concept p = Create("Concept with one example", { { "Only example", cmap::Competency::profession } }, 1, 2, 0);
    assert(p.GetRatingComplexity() >= -1);
    assert(p.GetRatingComplexity() <=  2);
    v.push_back(p);
  }
  /*
  {
    const Concept p = Create("Concept with two examples", { { "First example", cmap::Competency::organisations }, { "Second example", cmap::Competency::social_surroundings } }, 2, 0, 1);
    assert(p);
    assert(p.GetRatingComplexity() >= -1);
    assert(p.GetRatingComplexity() <=  2);
    v.push_back(p);
  }
  {
    const Concept p = Create("Concept with three examples", { { "Example 1 of 3", cmap::Competency::target_audience }, { "Example 2 of 3", cmap::Competency::ti_knowledge }, { "Example 3 of 3", cmap::Competency::prof_growth } }, 0, 1, 2);
    assert(p);
    assert(p.GetRatingComplexity() >= -1);
    assert(p.GetRatingComplexity() <=  2);
    v.push_back(p);
  }
  {
    const Concept p = Create("Concept with four Roman examples", { { "Example I/IV", cmap::Competency::misc }, { "Example II/IV", cmap::Competency::uninitialized }, { "Example III/IV", cmap::Competency::profession }, { "Example III/IV", cmap::Competency::social_surroundings } }, 1, 2, 0);
    assert(p);
    assert(p.GetRatingComplexity() >= -1);
    assert(p.GetRatingComplexity() <=  2);
    v.push_back(p);
  }
  */
  {
    const Concept p = Create(
      "Very long multi-line concept with four Roman examples that also each span multiple lines, that is, eighty characters",
      {
        { "Example I/IV, spanning multiple lines (that is, having at least eight characters) and is rated as cmap::Competency::misc", cmap::Competency::misc },
        { "Example II/IV, spanning multiple lines (that is, having at least eight characters) and is rated as cmap::Competency::uninitialized", cmap::Competency::uninitialized },
        { "Example III/IV, spanning multiple lines (that is, having at least eight characters) and is rated as cmap::Competency::profession", cmap::Competency::profession },
        { "Example III/IV, spanning multiple lines (that is, having at least eight characters) and is rated as cmap::Competency::social_surroundings", cmap::Competency::social_surroundings }
      }, 1, 2, 0
    );
    assert(p.GetRatingComplexity() >= -1);
    assert(p.GetRatingComplexity() <=  2);
    v.push_back(p);
  }
  return v;
}

#ifndef NDEBUG
void ribi::cmap::ConceptFactory::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  Counter();
  ConceptFactory().GetTest(0);
  ExamplesFactory();
  const bool verbose{false};
  const TestTimer test_timer(__func__,__FILE__,1.0);
  if (verbose) { TRACE("Concept -> XML -> Concept "); }
  {
    const auto concept = ConceptFactory().GetTest(2);
    const auto xml = concept.ToXml();
    const auto new_concept = ConceptFactory().FromXml(xml);
    const auto new_xml = new_concept.ToXml();
    assert(xml == new_xml);
  }
}
#endif
