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
#include "conceptmapcenternodefactory.h"

#include <cassert>
#include <sstream>
#include <stdexcept>
#include <boost/lexical_cast.hpp>

#include "conceptmapcenternode.h"
#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "conceptmapexamplefactory.h"
#include "conceptmapexamplesfactory.h"
#include "conceptmaphelper.h"
#include "conceptmapnodefactory.h"
#include "conceptmapregex.h"
#include "counter.h"
#include "ribi_regex.h"
#include "testtimer.h"
#include "trace.h"
#include "xml.h"
#pragma GCC diagnostic pop

ribi::cmap::CenterNodeFactory::CenterNodeFactory()
{
  #ifndef NDEBUG
  Test();
  #endif
}

ribi::cmap::CenterNode ribi::cmap::CenterNodeFactory::Create(
  const Concept& concept,
  const double x,
  const double y) const noexcept
{
  Node node(concept,x,y);
  node.SetIsCenterNode(true);
  assert(concept == node.GetConcept());
  assert(node.GetX() == x);
  assert(node.GetY() == y);
  return node;
}

/*
ribi::cmap::CenterNode ribi::cmap::CenterNodeFactory::CreateFromStrings(
  const std::string& name,
  const std::vector<std::pair<std::string,Competency> >& examples,
  const double x,
  const double y
) const noexcept
{
  CenterNode node(
    ConceptFactory().Create(name,examples),
    x,
    y
  );
  node.SetIsCenterNode(true);
  assert(node.GetX() == x);
  assert(node.GetY() == y);
  return node;
}
*/

ribi::cmap::CenterNode ribi::cmap::CenterNodeFactory::FromXml(
  const std::string& s
) const
{
  if (s.size() < 27)
  {
    std::stringstream msg;
    msg << __func__ << ": XML string '" << s << "' is only " << s.size() << " characters long, need at least 27";
    throw std::logic_error(msg.str());
  }
  if (s.substr(0,13) != "<center_node>")
  {
    std::stringstream msg;
    msg << __func__ << ": XML string '" << s << "' does not begin with <center_node>";
    throw std::logic_error(msg.str());
  }
  if (s.substr(s.size() - 14,14) != "</center_node>")
  {
    std::stringstream msg;
    msg << __func__ << ": XML string '" << s << "' does not end with </center_node>";
    throw std::logic_error(msg.str());
  }

  //m_concept
  Concept concept = ConceptFactory().Create();
  {
    const auto v = Regex().GetRegexMatches(s,Regex().GetRegexConcept());
    assert(v.size() == 1);
    concept = ConceptFactory().FromXml(v[0]);
  }
  //m_x
  double x = 0.0;
  {
    const std::vector<std::string> v = Regex().GetRegexMatches(s,Regex().GetRegexX());
    assert(v.size() == 1);
    x = boost::lexical_cast<double>(ribi::xml::StripXmlTag(v[0]));
  }
  //m_x
  double y = 0.0;
  {
    const auto v = Regex().GetRegexMatches(s,Regex().GetRegexY());
    assert(v.size() == 1);
    y = boost::lexical_cast<double>(ribi::xml::StripXmlTag(v[0]));
  }
  CenterNode node(concept,x,y);
  node.SetIsCenterNode(true);

  assert(node.ToXml() == s);
  return node;
}

#ifndef NDEBUG
void ribi::cmap::CenterNodeFactory::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  {
    NodeFactory();
    ConceptFactory();
    Counter();
    ExampleFactory();
    ExamplesFactory();
    ::ribi::Regex();
    ::ribi::cmap::TestHelperFunctions();
    ::ribi::cmap::Regex();
  }
  const TestTimer test_timer{__func__,__FILE__,0.1};
  //Create from Concept constructor
  {
    const auto concept = ConceptFactory().GetTest(0);
    const double x{0.1};
    const double y{2.3};
    const auto node = CenterNodeFactory().Create(concept,x,y);
    assert(node.IsCenterNode());
    assert(concept == node.GetConcept());
    assert(node.GetX() == x);
    assert(node.GetY() == y);
    TRACE(node.ToXml());
  }
  {
    const std::string xml = "";
    const auto node = CenterNodeFactory().FromXml(xml);
    assert(node.IsCenterNode());
  }
  assert(!"Green");
}
#endif
