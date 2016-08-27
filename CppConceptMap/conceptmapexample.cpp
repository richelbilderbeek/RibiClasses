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
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "conceptmapexample.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <sstream>

#include <boost/lexical_cast.hpp>
#include <QRegExp>

#include "graphviz_decode.h"
#include "graphviz_encode.h"
#include "counter.h"
#include "conceptmapregex.h"
#include "conceptmapcompetencies.h"
#include "conceptmapcompetency.h"
#include "conceptmapexample.h"
#include "conceptmapexamplefactory.h"
#include "conceptmaphelper.h"
#include "ribi_regex.h"

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

}

void ribi::cmap::Example::Decode() noexcept
{
  m_text = graphviz_decode(m_text);
}

ribi::cmap::Competency ribi::cmap::ExtractExampleCompetencyFromXml(const std::string& s)
{
  const std::vector<std::string> v
    = Regex().GetRegexMatches(s,Regex().GetRegexCompetency());
  assert(v.size() == 1);
  return Competencies().ToType(ribi::xml::StripXmlTag(v[0]));
}

bool ribi::cmap::ExtractExampleIsComplexFromXml(const std::string& s)
{
  const std::vector<std::string> v
    = Regex().GetRegexMatches(s,Regex().GetRegexIsComplex());
  assert(v.size() == 1);
  return boost::lexical_cast<bool>(ribi::xml::StripXmlTag(v[0]));
}

bool ribi::cmap::ExtractExampleIsConcreteFromXml(const std::string& s)
{
  const std::vector<std::string> v
    = Regex().GetRegexMatches(s,Regex().GetRegexIsConcrete());
  assert(v.size() == 1);
  return boost::lexical_cast<bool>(ribi::xml::StripXmlTag(v[0]));
}

bool ribi::cmap::ExtractExampleIsSpecificFromXml(const std::string& s)
{
  const std::vector<std::string> v
    = Regex().GetRegexMatches(s,Regex().GetRegexIsSpecific());
  assert(v.size() == 1);
  return boost::lexical_cast<bool>(ribi::xml::StripXmlTag(v[0]));
}

std::string ribi::cmap::ExtractExampleTextFromXml(const std::string& s)
{
  const std::vector<std::string> v
    = Regex().GetRegexMatches(s,Regex().GetRegexText());
  assert(v.size() == 1 && "GetRegexText must be present once in an Example");
  return graphviz_decode(ribi::xml::StripXmlTag(v[0]));
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

ribi::cmap::Example ribi::cmap::XmlToExample(const std::string& s)
{
  if (s.size() < 17)
  {
    std::stringstream msg;
    msg << __func__ << ": XML string '" << s << "' is only " << s.size()
      << " characters long, need at least 27"
    ;
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
  Example example(
    ExtractExampleTextFromXml(s),
    ExtractExampleCompetencyFromXml(s),
    ExtractExampleIsComplexFromXml(s),
    ExtractExampleIsConcreteFromXml(s),
    ExtractExampleIsSpecificFromXml(s)
  );
  assert(ToXml(example) == s);
  return example;
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const Example& example) noexcept
{
  os << graphviz_encode(ToXml(example));
  return os;
}

std::istream& ribi::cmap::operator>>(std::istream& is, Example& example)
{
  std::string s;
  is >> s;
  assert(s != "0");
  example = XmlToExample(graphviz_decode(s));
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
