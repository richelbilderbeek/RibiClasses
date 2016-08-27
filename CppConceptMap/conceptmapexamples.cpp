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
#include "conceptmapexamples.h"

#include <cassert>
#include <sstream>

#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <QRegExp>
#include "counter.h"
#include "conceptmapregex.h"
#include "graphviz_decode.h"
#include "graphviz_encode.h"
#include "conceptmaphelper.h"
#include "conceptmapexample.h"
#include "conceptmapexamplefactory.h"
#include "conceptmapexamplesfactory.h"

#include "trace.h"
#pragma GCC diagnostic pop


ribi::cmap::Examples::Examples(
  const std::vector<Example>& v)
  : m_v{v}
{

}

void ribi::cmap::Examples::Decode() noexcept
{
  for (auto& example: Get())
  {
    example.Decode();
  };
}

std::string ribi::cmap::Examples::ToStr() const noexcept
{
  std::stringstream s;
  s << "{ ";

  //const std::vector<const Example> > examples = c->GetExamples();
  for (const auto& example: Get())
  {
    s << example.ToStr() << " ";
  };
  s << "}";
  return s.str();
}

std::string ribi::cmap::ToXml(const Examples& any_examples) noexcept
{
  std::stringstream s;
  s << "<examples>";

  const auto& examples = any_examples.Get();
  std::for_each(
    std::begin(examples),
    std::end(examples),
    [&s](const Example& t)
    {
      s << ToXml(t);
    }
  );
  s << "</examples>";

  const std::string r = s.str();
  assert(r.size() >= 20);
  assert(r.substr(0,10) == "<examples>");
  assert(r.substr(r.size() - 11,11) == "</examples>");
  return r;
}

ribi::cmap::Examples ribi::cmap::XmlToExamples(const std::string& s)
{
  if (s.size() < 20)
  {
    std::stringstream msg;
    msg << __func__ << ": XML string '" << s << "' is only " << s.size()
      << " characters long, need at least 20";
    throw std::logic_error(msg.str());
  }
  if (s.substr(0,10) != "<examples>")
  {
    std::stringstream msg;
    msg << __func__ << ": XML string '" << s << "' does not begin with <examples>";
    throw std::logic_error(msg.str());
  }
  if (s.substr(s.size() - 11,11) != "</examples>")
  {
    std::stringstream msg;
    msg << __func__ << ": XML string '" << s << "' does not end with </examples>";
    throw std::logic_error(msg.str());
  }

  assert(Regex().GetRegexMatches(s,"(<examples>)").size()
      == Regex().GetRegexMatches(s,"(</examples>)").size());

  std::vector<Example> examples;
  {

    assert(Regex().GetRegexMatches(s,"(<example>)").size()
        == Regex().GetRegexMatches(s,"(</example>)").size());
    const auto v = Regex().GetRegexMatches(s,Regex().GetRegexExample());
    std::transform(v.begin(),v.end(),std::back_inserter(examples),
      [](const std::string& s)
      {
        return XmlToExample(s); //ExampleFactory().FromXml(s);
      }
    );
  }
  return Examples(examples);
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const Examples& examples) noexcept
{
  os << graphviz_encode(ToXml(examples));
  return os;
}

std::istream& ribi::cmap::operator>>(std::istream& is, Examples& examples)
{
  std::string s;
  is >> s;
  /*
  //eat until '</example>'
  is >> std::noskipws;
  std::string s;
  while (1)
  {
    char c;
    is >> c;
    s += c;
    if(s.size() > 11 && s.substr(s.size() - 11,11) == "</examples>") break;
  }
  */
  assert(s != "0");
  examples = XmlToExamples(graphviz_decode(s));
  return is;
}

bool ribi::cmap::operator==(const Examples& lhs, const Examples& rhs)
{
  if (lhs.Get().size() != rhs.Get().size())
  {
    return false;
  }
  if (lhs.Get().empty()) { assert(rhs.Get().empty()); return true; }
  const auto& v = lhs.Get();
  const auto& w = rhs.Get();
  return std::equal(v.begin(),v.end(),w.begin(),
    [](const Example& a, const Example& b)
    {
      return a == b;
    }
  );
}

bool ribi::cmap::operator!=(const Examples& lhs, const Examples& rhs)
{
  return !(lhs == rhs);
}

bool ribi::cmap::operator<(const Examples& lhs, const Examples& rhs)
{
  if (lhs.Get().size() < rhs.Get().size()) return true;
  if (lhs.Get().size() > rhs.Get().size()) return false;
  assert(lhs.Get().size() == rhs.Get().size());

  const auto& v = lhs.Get();
  const auto& w = rhs.Get();
  const int sz = lhs.Get().size();
  for (int i = 0; i!=sz; ++i)
  {
    if (v[i] < w[i]) return true;
    if (v[i] > w[i]) return false;
  }
  return false;
}
