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
#include "conceptmapedgefactory.h"

#include <cassert>
#include <boost/lexical_cast.hpp>

#include "container.h"
#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "conceptmapedge.h"
#include "conceptmapedgefactory.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "conceptmapnodefactory.h"
#include "conceptmapregex.h"
#include "testtimer.h"
#include "trace.h"
#include "xml.h"
#pragma GCC diagnostic pop

ribi::cmap::EdgeFactory::EdgeFactory() noexcept
{
  #ifndef NDEBUG
  Test();
  #endif
}


ribi::cmap::Edge ribi::cmap::EdgeFactory::Create(
  const Node& from,
  const Node& to
) const noexcept
{
  //Nodes may be similar, but not the same
  assert(&from != &to);

  const double x{(from.GetX() + to.GetX()) / 2.0};
  const double y{(from.GetY() + to.GetY()) / 2.0};
  const auto concept = ConceptFactory().Create();
  const auto node = NodeFactory().Create(concept,x,y);
  Edge p(
    node
  );
  return p;
}

ribi::cmap::Edge ribi::cmap::EdgeFactory::Create(
  const Node& node
) const noexcept
{
  Edge p(node);
  return p;
}

/*
#ifndef NDEBUG
ribi::cmap::Edge ribi::cmap::EdgeFactory::DeepCopy(
  const Edge& edge,
  const Node& from,
  const Node& to
) const noexcept
{
  //Nodes may be similar, but not the same
  assert(&from != &to);
  assert(&from != &edge->GetNode());
  assert(&to   != &edge->GetNode());

  const Node node(edge->GetNode());
  const Edge p{
    EdgeFactory::Create(
      node,
      from,
      edge->HasTailArrow(),
      to,
      edge->HasHeadArrow()
    )
  };
  assert(edge == p);
  return p;
}
#endif
*/

ribi::cmap::Edge ribi::cmap::EdgeFactory::FromXml(
  const std::string& s
) const noexcept
{
  using ribi::xml::StripXmlTag;
  assert(s.size() >= 13);
  assert(s.substr(0,6) == "<edge>");
  assert(s.substr(s.size() - 7,7) == "</edge>");
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
    x = boost::lexical_cast<double>(StripXmlTag(v[0]));
  }
  //m_y
  double y = 0.0;
  {
    const std::vector<std::string> v
      = Regex().GetRegexMatches(s,Regex().GetRegexY());
    assert(v.size() == 1);
    y = boost::lexical_cast<double>(StripXmlTag(v[0]));
  }
  const auto node = NodeFactory().Create(concept,x,y);
  Edge edge(
    node
  );
  return edge;
}

int ribi::cmap::EdgeFactory::GetNumberOfTests() const noexcept
{
  return ConceptFactory().GetNumberOfTests();
}

ribi::cmap::Edge ribi::cmap::EdgeFactory::GetTest(
  const int index,
  const Node& from,
  const Node& to
) const noexcept
{
  //Nodes may be similar, but not the same
  assert(&from != &to);
  assert(index >= 0);
  assert(index < GetNumberOfTests());
  return GetTests(from,to)[index];
}

std::vector<ribi::cmap::Edge> ribi::cmap::EdgeFactory::GetTests(
  const Node& from,
  const Node& to
) const noexcept
{
  //Nodes may be similar, but not the same
  assert(&from != &to);

  const int n{NodeFactory().GetNumberOfTests()};
  std::vector<Edge> result;

  for(int i{0}; i!=n; ++i)
  {
    {
      const auto node = NodeFactory().GetTest(i);
      Edge edge(node);
      result.emplace_back(edge); //TODO: use emplace_back
    }
    /*
    {
      const EdgePtr edge(new Edge(concept,0,2,true));
      result.push_back(edge);
    }
    {
      const EdgePtr edge(new Edge(concept,0,3,true));
      result.push_back(edge);
    }
    {
      const EdgePtr edge(new Edge(concept,1,2,false));
      result.push_back(edge);
    }
    {
      const EdgePtr edge(new Edge(concept,1,3,false));
      result.push_back(edge);
    }
    {
      const EdgePtr edge(new Edge(concept,2,3,false));
      result.push_back(edge);
    }
    */
  }
  if (GetNumberOfTests() != static_cast<int>(result.size()))
  {
    TRACE("ERROR");
    std::stringstream s;
    s << "GetNumberOfTests should return " << result.size();
    TRACE(s.str());
  }
  assert(GetNumberOfTests() == static_cast<int>(result.size()));
  return result;
}


#ifndef NDEBUG
void ribi::cmap::EdgeFactory::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  EdgeFactory().GetTest(
     0,
     NodeFactory().GetTest(0),
     NodeFactory().GetTest(0)
  );
  const TestTimer test_timer(__func__,__FILE__,1.0);
}
#endif // NDEBUG
