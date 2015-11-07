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

#include "conceptmapcenternode.h"
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
  Node node(ConceptFactory().Create(),0.0,0.0);
  return node;
}

ribi::cmap::Node ribi::cmap::NodeFactory::Create(
  const Concept& concept,
  const double x,
  const double y
) const noexcept
{
  Node node(concept,x,y);
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
  Node node(
    ConceptFactory().Create(name,examples),
    x,
    y
  );
  assert(node.GetX() == x);
  assert(node.GetY() == y);
  return node;
}

#ifndef NDEBUG
ribi::cmap::Node ribi::cmap::NodeFactory::DeepCopy(
  const Node& node) const noexcept
{
  /*
  const Concept new_concept(node->GetConcept());
  const boost::shared_ptr<Node> new_node{
    Create(
      new_concept,
      node->GetX(),
      node->GetY()
    )
  };
  */
  return Node(node);
}
#endif

ribi::cmap::Node ribi::cmap::NodeFactory::FromXml(const std::string& s) const
{
  try
  {
    const CenterNode center_node {
      CenterNodeFactory().FromXml(s)
    };
    return center_node;
  }
  catch (std::logic_error& e)
  {
    //OK, probably is a Node
  }

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
  Node node(NodeFactory().Create(concept,x,y));
  return node;
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
    assert(NodeFactory().DeepCopy(NodeFactory().GetTest(0)) == NodeFactory().GetTest(0));
  }
  //XLM <-> std::string conversions
  {
    for (const auto node: NodeFactory().GetTests())
    {
      const auto str = node.ToXml();
      const auto node_again = NodeFactory().FromXml(str);
      assert(node == node_again);
    }
  }
}
#endif
