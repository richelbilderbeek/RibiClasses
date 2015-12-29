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
#include "conceptmapnodefactory.h"


#include "conceptmapcenternodefactory.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "conceptmapregex.h"
#include "container.h"
#include "testtimer.h"
#include "trace.h"
#include "xml.h"
#pragma GCC diagnostic pop

ribi::cmap::NodeFactory::NodeFactory()
{
  #ifndef NDEBUG
  Test();
  #endif
}

ribi::cmap::Node ribi::cmap::NodeFactory::Create(
) const noexcept
{
  Node node(ConceptFactory().Create(),false,0.0,0.0);
  return node;
}

ribi::cmap::Node ribi::cmap::NodeFactory::Create(
  const Concept& concept,
  const double x,
  const double y
) const noexcept
{
  const bool is_center_node{false};
  Node node(concept,is_center_node,x,y);
  assert(concept == node.GetConcept());
  assert(node.GetX() == x);
  assert(node.GetY() == y);
  return node;
}

ribi::cmap::Node ribi::cmap::NodeFactory::CreateFromStrings(
  const std::string& name,
  const std::vector<std::pair<std::string,Competency> >& examples,
  const double x,
  const double y
) const noexcept
{
  const bool is_center_node{false};
  Node node(
    ConceptFactory().Create(name,examples),
    is_center_node,
    x,
    y
  );
  assert(node.GetX() == x);
  assert(node.GetY() == y);
  return node;
}

ribi::cmap::Node ribi::cmap::NodeFactory::FromXml(const std::string& s) const
{
  //const bool verbose{false};
  if (s.size() < 13)
  {
    std::stringstream msg;
    msg << __func__ << ": string too short";
    throw std::logic_error(msg.str());
  }
  if (s.substr(0,6) != "<node>")
  {
    std::stringstream msg;
    msg << __func__ << ": incorrect starting tag";
    throw std::logic_error(msg.str());
  }
  if (s.substr(s.size() - 7,7) != "</node>")
  {
    std::stringstream msg;
    msg << __func__ << ": incorrect ending tag";
    throw std::logic_error(msg.str());
  }

  //m_concept
  Concept concept = ConceptFactory().Create();
  {
    const std::vector<std::string> v
      = Regex().GetRegexMatches(s,Regex().GetRegexConcept());
    assert(v.size() == 1);
    concept = ConceptFactory().FromXml(v[0]);
  }

  //m_is_centernode
  bool is_center_node = false;
  {
    const std::vector<std::string> v
      = Regex().GetRegexMatches(s,Regex().GetRegexIsCenterNode());
    if (v.size() == 1) {
      is_center_node = boost::lexical_cast<bool>(ribi::xml::StripXmlTag(v[0]));
    }
  }
  //m_x
  double x = 0.0;
  {
    const std::vector<std::string> v
      = Regex().GetRegexMatches(s,Regex().GetRegexX());
    assert(v.size() == 1);
    x = boost::lexical_cast<double>(ribi::xml::StripXmlTag(v[0]));
  }
  //m_x
  double y = 0.0;
  {
    const std::vector<std::string> v
      = Regex().GetRegexMatches(s,Regex().GetRegexY());
    assert(v.size() == 1);
    y = boost::lexical_cast<double>(ribi::xml::StripXmlTag(v[0]));
  }
  if (is_center_node)
  {
    return CenterNodeFactory().Create(concept,x,y);
  }
  else
  {
    return NodeFactory().Create(concept,x,y);
  }
}

int ribi::cmap::NodeFactory::GetNumberOfTests() const noexcept
{
  return static_cast<int>(GetTests().size());
}

ribi::cmap::Node ribi::cmap::NodeFactory::GetTest(const int i) const noexcept
{
  const auto tests = GetTests();
  assert(i >= 0);
  assert(i < static_cast<int>(tests.size()));
  return tests[i];
}

std::vector<ribi::cmap::Node> ribi::cmap::NodeFactory::GetTests() const noexcept
{
  std::vector<Node> nodes;
  const auto v = ConceptFactory().GetTests();
  std::transform(v.begin(),v.end(),std::back_inserter(nodes),
    [](const Concept& c)
    {
      const int x = 0;
      const int y = 0;
      const Node p(NodeFactory().Create(c,x,y));
      return p;
    }
  );
  return nodes;
}

#ifndef NDEBUG
void ribi::cmap::NodeFactory::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  NodeFactory().GetTest(0);
  const TestTimer test_timer(__func__,__FILE__,1.0);
  //operator== and operator!=
  {
    assert(NodeFactory().GetTest(0) == NodeFactory().GetTest(0));
    assert(NodeFactory().GetTest(0) != NodeFactory().GetTest(1));
  }
  //Deep copy
  {
    const Node a = NodeFactory().GetTest(0);
    const Node b(a);
    assert(b == NodeFactory().GetTest(0));
  }
  //XLM <-> std::string conversions
  {
    for (const auto node: NodeFactory().GetTests())
    {
      const std::string str{ToXml(node)};
      const Node node_again{NodeFactory().FromXml(str)};
      assert(node == node_again);
    }
  }
}
#endif
