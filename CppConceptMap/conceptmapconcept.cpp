//---------------------------------------------------------------------------
/*
ConceptMap, concept map classes
Copyright (C) 2013-2016 Richel Bilderbeek

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
#include "conceptmapconcept.h"

#include <sstream>
#include <stdexcept>
#include <boost/lexical_cast.hpp>

#include "conceptmapconceptfactory.h"
#include "conceptmaphelper.h"
#include "conceptmapcompetency.h"
#include "conceptmapexample.h"
#include "conceptmapexamples.h"
#include "conceptmapexamplesfactory.h"
#include "conceptmapregex.h"
#include "graphviz_decode.h"
#include "graphviz_encode.h"
#include "trace.h"
#include "xml.h"
#pragma GCC diagnostic pop

ribi::cmap::Concept::Concept(
  const std::string& name,
  const Examples& examples,
  const bool is_complex,
  const int rating_complexity,
  const int rating_concreteness,
  const int rating_specificity)
  : m_examples{examples},
    m_is_complex{is_complex},
    m_name{name},
    m_rating_complexity{rating_complexity},
    m_rating_concreteness{rating_concreteness},
    m_rating_specificity{rating_specificity}

{
  #ifndef NDEBUG
  assert(m_rating_complexity   >= -1);
  assert(m_rating_complexity   <=  2);
  assert(m_rating_concreteness >= -1);
  assert(m_rating_concreteness <=  2);
  assert(m_rating_specificity  >= -1);
  assert(m_rating_specificity  <=  2);
  #endif
}

void ribi::cmap::Concept::Decode() noexcept
{
  m_name = graphviz_decode(m_name);
  m_examples.Decode();
}

ribi::cmap::Concept ribi::cmap::ExtractConceptFromXml(const std::string& s)
{
  const std::vector<std::string> v
    = Regex().GetRegexMatches(s,Regex().GetRegexConcept());
  if (v.size() != 1)
  {
    std::stringstream msg;
    msg << __func__ << ": must have one concept tag in XML";
    throw std::invalid_argument(msg.str());
  }
  return XmlToConcept(v[0]);
}

ribi::cmap::Examples ribi::cmap::ExtractExamplesFromXml(const std::string& s)
{
  const std::vector<std::string> v
    = Regex().GetRegexMatches(s,Regex().GetRegexExamples());
  if (v.size() != 1)
  {
    std::stringstream msg;
    msg << __func__ << ": must have one examples tag in XML";
    throw std::invalid_argument(msg.str());
  }
  return XmlToExamples(v[0]);
}

bool ribi::cmap::ExtractIsComplexFromXml(const std::string& s)
{
  const std::vector<std::string> v
    = Regex().GetRegexMatches(s,Regex().GetRegexConceptIsComplex());
  if (v.size() != 1)
  {
    std::stringstream msg;
    msg << __func__ << ": must have one is_complex tag in XML";
    throw std::invalid_argument(msg.str());
  }
  return boost::lexical_cast<bool>(ribi::xml::StripXmlTag(v[0]));
}

std::string ribi::cmap::ExtractNameFromXml(const std::string& s)
{
  const std::vector<std::string> v
    = Regex().GetRegexMatches(s,Regex().GetRegexName());
  if (v.size() != 1)
  {
    std::stringstream msg;
    msg << __func__ << ": must have one name tag in XML";
    throw std::invalid_argument(msg.str());
  }
  return graphviz_decode(ribi::xml::StripXmlTag(v[0]));
}

int ribi::cmap::ExtractRatingComplexityFromXml(const std::string& s)
{
  const std::vector<std::string> v
    = Regex().GetRegexMatches(s,Regex().GetRegexComplexity());

  if (v.size() != 1)
  {
    std::stringstream msg;
    msg << __func__ << ": must have one rating_complexity tag in XML";
    throw std::invalid_argument(msg.str());
  }

  return boost::lexical_cast<int>(ribi::xml::StripXmlTag(v[0]));
}

int ribi::cmap::ExtractRatingConcretenessFromXml(const std::string& s)
{
  const std::vector<std::string> v
    = Regex().GetRegexMatches(s,Regex().GetRegexConcreteness());

  if (v.size() != 1)
  {
    std::stringstream msg;
    msg << __func__ << ": must have one rating_concreteness tag in XML";
    throw std::invalid_argument(msg.str());
  }

  return boost::lexical_cast<int>(ribi::xml::StripXmlTag(v[0]));

}

int ribi::cmap::ExtractRatingSpecificityFromXml(const std::string& s)
{
  const std::vector<std::string> v
    = Regex().GetRegexMatches(s,Regex().GetRegexSpecificity());

  if (v.size() != 1)
  {
    std::stringstream msg;
    msg << __func__ << ": must have one rating_specificity tag in XML";
    throw std::invalid_argument(msg.str());
  }

  return boost::lexical_cast<int>(ribi::xml::StripXmlTag(v[0]));
}

void ribi::cmap::Concept::SetExamples(const Examples& examples) noexcept
{
  m_examples = examples;
}

void ribi::cmap::Concept::SetIsComplex(const bool is_complex) noexcept
{
  m_is_complex = is_complex;
}


void ribi::cmap::Concept::SetName(const std::string& name) noexcept
{
  m_name = name;
}

void ribi::cmap::Concept::SetRatingComplexity(const int rating_complexity) noexcept
{
  m_rating_complexity = rating_complexity;
  assert(m_rating_complexity >= -1);
  assert(m_rating_complexity <=  2);
}

void ribi::cmap::Concept::SetRatingConcreteness(const int rating_concreteness) noexcept
{
  m_rating_concreteness = rating_concreteness;
  assert(m_rating_concreteness >= -1);
  assert(m_rating_concreteness <=  2);
}

void ribi::cmap::Concept::SetRatingSpecificity(const int rating_specificity) noexcept
{
  m_rating_specificity = rating_specificity;
  assert(m_rating_specificity >= -1);
  assert(m_rating_specificity <=  2);
}

std::string ribi::cmap::Concept::ToStr() const noexcept
{
  std::stringstream s;
  s
    << GetName() << " "
    << GetExamples().ToStr() << " "
    << GetIsComplex() << " "
    << GetRatingComplexity() << " "
    << GetRatingConcreteness() << " "
    << GetRatingSpecificity()
  ;
  return s.str();
}

std::string ribi::cmap::ToXml(const Concept& concept) noexcept
{
  std::stringstream s;
  s
    << "<concept>"
    <<   "<name>"
    <<      concept.GetName()
    <<   "</name>"
    <<   ToXml(concept.GetExamples())
    <<   "<concept_is_complex>"
    <<     concept.GetIsComplex()
    <<   "</concept_is_complex>"
    <<   "<complexity>"
    <<     concept.GetRatingComplexity()
    <<   "</complexity>"
    <<   "<concreteness>"
    <<     concept.GetRatingConcreteness()
    <<   "</concreteness>"
    <<   "<specificity>"
    <<     concept.GetRatingSpecificity()
    <<   "</specificity>"
    << "</concept>"
  ;
  const std::string r = s.str();

  assert(r.size() >= 19);
  assert(r.substr(0,9) == "<concept>");
  assert(r.substr(r.size() - 10,10) == "</concept>");
  return r;
}

ribi::cmap::Concept ribi::cmap::XmlToConcept(const std::string& s)
{
  assert(s.size() >= 19);
  assert(s.substr(0,9) == "<concept>");
  assert(s.substr(s.size() - 10,10) == "</concept>");
  return Concept(
    ExtractNameFromXml(s),
    ExtractExamplesFromXml(s),
    ExtractIsComplexFromXml(s),
    ExtractRatingComplexityFromXml(s),
    ExtractRatingConcretenessFromXml(s),
    ExtractRatingSpecificityFromXml(s)
  );

}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const Concept& concept) noexcept
{
  os << graphviz_encode(ToXml(concept));
  return os;
}

std::istream& ribi::cmap::operator>>(std::istream& is, Concept& concept)
{
  std::string s;
  is >> s;
  concept = XmlToConcept(graphviz_decode(s));
  return is;
}

bool ribi::cmap::operator==(const ribi::cmap::Concept& lhs, const ribi::cmap::Concept& rhs)
{
  if (lhs.GetIsComplex() != rhs.GetIsComplex())
  {
    return false;
  }
  if (lhs.GetName() != rhs.GetName())
  {
    return false;
  }
  if (lhs.GetRatingComplexity() != rhs.GetRatingComplexity())
  {
    return false;
  }
  if (lhs.GetRatingConcreteness() != rhs.GetRatingConcreteness())
  {
    return false;
  }
  if (lhs.GetRatingSpecificity() != rhs.GetRatingSpecificity())
  {
    return false;
  }
  const auto lhs_examples = lhs.GetExamples();
  const auto rhs_examples = rhs.GetExamples();
  if (lhs_examples == rhs_examples)
  {
    return true;
  }
  return false;
}

bool ribi::cmap::operator!=(const ribi::cmap::Concept& lhs, const ribi::cmap::Concept& rhs)
{
  return !(lhs == rhs);
}

template <class T> int to_digit(const T& a, const T& b)
{
  if (a < b) return 0;
  if (a == b) return 1;
  return 2;
}

bool ribi::cmap::operator<(const ribi::cmap::Concept& lhs, const ribi::cmap::Concept& rhs)
{
  //Convert all comparisons to a number, like ABCDE
  //of which each digit is either 0,1 or 2
  //Each digit stands for a comparison of:
  //A: Name
  //B: Examples
  //C: GetRatingComplexity
  //D: GetRatingConcreteness
  //E: GetRatingSpecificity
  //Each digit has a value of
  // 0: lhs has less
  // 1: equal
  // 2: lhs has more
  std::vector<int> digits = {
    to_digit(lhs.GetName(), rhs.GetName()),
    to_digit(lhs.GetExamples(), rhs.GetExamples()),
    to_digit(lhs.GetRatingComplexity(), rhs.GetRatingComplexity()),
    to_digit(lhs.GetRatingConcreteness(), rhs.GetRatingConcreteness()),
    to_digit(lhs.GetRatingSpecificity(), rhs.GetRatingSpecificity())
  };

  for (const int& i: digits)
  {
    if (i == 0) return true;
    if (i == 2) return false;
  }
  return false;
}
