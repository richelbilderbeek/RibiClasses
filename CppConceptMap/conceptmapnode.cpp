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
#include "conceptmapnode.h"

#include <boost/lambda/lambda.hpp>

#include "counter.h"
#include "container.h"
#include "conceptmapconcept.h"
#include "conceptmapconceptfactory.h"
#include "conceptmapnodefactory.h"
#include "conceptmapexamplefactory.h"
#include "conceptmaphelper.h"
#include "testtimer.h"
#include "trace.h"
#include "xml.h"
#pragma GCC diagnostic pop

int ribi::cmap::Node::sm_ids = 0; //ID to assign

ribi::cmap::Node::Node(
  const Concept& concept,
  const bool is_center_node,
  const double x,
  const double y
) noexcept
  : m_concept{concept},
    m_id{sm_ids++},
    m_is_center_node{is_center_node},
    m_x(x),
    m_y(y)
{
  #ifndef NDEBUG
  Test();
  #endif
  SetConcept(concept);
  assert(m_concept == concept);
}

int ribi::cmap::CountCenterNodes(
  const std::vector<Node>& nodes
) noexcept
{
  const int cnt = container().CountIf(nodes,
    [](const Node& node)
    {
      return IsCenterNode(node);
    }
  );
  return cnt;
}



std::vector<ribi::cmap::Node>::const_iterator  ribi::cmap::FindCenterNode(const std::vector<Node>& nodes) noexcept
{
  return std::find_if(std::begin(nodes),std::end(nodes),[](const Node& node) { return IsCenterNode(node); } );
}

std::vector<ribi::cmap::Node> ribi::cmap::Node::GetTests() noexcept
{
  const auto test_concepts = ConceptFactory().GetTests();
  std::vector<Node> result;
  std::for_each(test_concepts.begin(),test_concepts.end(),
    [&result](const Concept& concept)
    {
      const int x = (std::rand() % 256) - 128;
      const int y = (std::rand() % 256) - 128;
      const auto node = NodeFactory().Create(concept,x,y);
      result.push_back(node);
    }
  );
  return result;
}

bool ribi::cmap::HasSameContent(const Node& lhs, const Node& rhs) noexcept
{
  return lhs.GetConcept() == rhs.GetConcept();
}

bool ribi::cmap::HaveSameIds(const Node& lhs, const Node& rhs) noexcept
{
  return lhs.m_id == rhs.m_id;
}

bool ribi::cmap::IsCenterNode(const Node& node) noexcept
{
  return node.IsCenterNode();
}

void ribi::cmap::Node::OnConceptChanged(Concept * const) noexcept
{
  //m_signal_concept_changed(this);
}

void ribi::cmap::Node::SetConcept(const Concept& concept) noexcept
{
  const bool verbose{false};

  if (m_concept == concept)
  {
    return;
  }

  if (verbose)
  {
    std::stringstream s;
    s << "Setting concept '" << concept.ToStr() << "'\n";
  }

  const auto examples_after = concept.GetExamples();
  const auto is_complex_after = concept.GetIsComplex();
  const auto name_after = concept.GetName();
  const auto rating_complexity_after = concept.GetRatingComplexity();
  const auto rating_concreteness_after = concept.GetRatingConcreteness();
  const auto rating_specificity_after = concept.GetRatingSpecificity();

  bool examples_changed{true};
  bool is_complex_changed{true};
  bool name_changed{true};
  bool rating_complexity_changed{true};
  bool rating_concreteness_changed{true};
  bool rating_specificity_changed{true};

  //if (m_concept)
  {
    const auto examples_before = m_concept.GetExamples();
    const auto is_complex_before = m_concept.GetIsComplex();
    const auto name_before = m_concept.GetName();
    const auto rating_complexity_before = m_concept.GetRatingComplexity();
    const auto rating_concreteness_before = m_concept.GetRatingConcreteness();
    const auto rating_specificity_before = m_concept.GetRatingSpecificity();

    examples_changed = examples_before != examples_after;
    is_complex_changed = is_complex_before != is_complex_after;
    name_changed = name_before != name_after;
    rating_complexity_changed = rating_complexity_before != rating_complexity_after;
    rating_concreteness_changed = rating_concreteness_before != rating_concreteness_after;
    rating_specificity_changed = rating_specificity_before != rating_specificity_after;

    if (verbose)
    {
      if (examples_changed)
      {
        std::stringstream s;
        s
          << "Examples will change from "
          << examples_before.ToStr()
          << " to "
          << examples_after.ToStr()
          << '\n'
        ;
        TRACE(s.str());
      }
      if (is_complex_changed)
      {
        std::stringstream s;
        s << "Is complex will change from " << is_complex_before
          << " to " << is_complex_after << '\n';
        TRACE(s.str());
      }
      if (name_changed)
      {
        std::stringstream s;
        s << "Name will change from " << name_before
          << " to " << name_after << '\n';
        TRACE(s.str());
      }
      if (rating_complexity_changed)
      {
        std::stringstream s;
        s << "Rating_complexicity will change from " << rating_complexity_before
          << " to " << rating_complexity_after << '\n';
        TRACE(s.str());
      }
      if (rating_concreteness_changed)
      {
        std::stringstream s;
        s << "Rating_concreteness will change from " << rating_concreteness_before
          << " to " << rating_concreteness_after << '\n';
        TRACE(s.str());
      }
      if (rating_specificity_changed)
      {
        std::stringstream s;
        s << "Rating_specificity will change from " << rating_specificity_before
          << " to " << rating_specificity_after << '\n';
        TRACE(s.str());
      }

    }
  }

  //Replace m_example by the new one
  m_concept = concept;


  assert(m_concept.GetExamples() == examples_after );
  assert(m_concept.GetIsComplex() == is_complex_after );
  assert(m_concept.GetName() == name_after);
  assert(m_concept.GetRatingComplexity() == rating_complexity_after);
  assert(m_concept.GetRatingConcreteness() == rating_concreteness_after);
  assert(m_concept.GetRatingSpecificity() == rating_specificity_after);
  assert( concept ==  m_concept);
}

void ribi::cmap::Node::SetX(const double x) noexcept
{
  //const bool verbose{false};
  if (m_x != x)
  {
    m_x = x;
    //if (verbose) { TRACE("Emitting m_signal_x_changed"); }
    //m_signal_x_changed(this);
  }
}

void ribi::cmap::Node::SetY(const double y) noexcept
{
  if (m_y != y)
  {
    m_y = y;
    //m_signal_y_changed(this);
  }
}

#ifndef NDEBUG
void ribi::cmap::Node::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  const TestTimer test_timer(__func__,__FILE__,1.0);
  //const bool verbose{false};
  //Copy constructable
  {
    const Node a = NodeFactory().Create();
    const Node b(a);
    assert(a == b);
  }
  {
    const Node a = NodeFactory().GetTest(1);
    const Node b = NodeFactory().GetTest(2);
    assert(a != b);
    Node c(a);
    assert(c == a);
    assert(c != b);
    c = b;
    assert(c != a);
    assert(c == b);
  }
  {
    const std::vector<Node> v = Node::GetTests();
    std::for_each(v.begin(),v.end(),
      [](const Node& node)
      {
        //Test copy constructor
        const Node c(node);
        assert(node == c);
        const std::string s{ToXml(c)};
        const Node d = NodeFactory().FromXml(s);
        assert(c == d);
      }
    );
  }
  //Test HasSameContent
  {
    {
      const Concept c(ConceptFactory().Create("1"));
      const Concept d(ConceptFactory().Create("1"));
      assert(c == d);
      const Node a(NodeFactory().Create(c));
      const Node b(NodeFactory().Create(d));
      assert(HasSameContent(a,b));
      assert(a == b);
    }
    const int sz = static_cast<int>(ConceptFactory().GetTests().size());
    for (int i=0; i!=sz; ++i)
    {
      const Concept c = ConceptFactory().Create("1", { {"2", cmap::Competency::uninitialized} } );
      const Concept d = ConceptFactory().Create("1", { {"2", cmap::Competency::uninitialized} } );
      assert(c == d);
      const Node a(NodeFactory().Create(c));
      const Node b(NodeFactory().Create(d));
      assert(HasSameContent(a,b));
    }

    {
      //Cannot shuffle Concept its examples. No need to as well: the order is important
      const Concept c = ConceptFactory().Create("1", { {"2", cmap::Competency::uninitialized},{"3", cmap::Competency::uninitialized} } );
      const Concept d = ConceptFactory().Create("1", { {"2", cmap::Competency::uninitialized},{"3", cmap::Competency::uninitialized} } );
      assert(c == d);
      const Node a(NodeFactory().Create(c));
      const Node b(NodeFactory().Create(d));
      assert(HasSameContent(a,b));
      assert(a == b);
    }
    {
      //Cannot shuffle Concept its examples. No need to as well: the order is important
      const Concept c = ConceptFactory().Create("1", { {"2", cmap::Competency::uninitialized},{"3", cmap::Competency::uninitialized} } );
      const Concept d = ConceptFactory().Create("1", { {"3", cmap::Competency::uninitialized},{"2", cmap::Competency::uninitialized} } );
      assert(c != d);
      const Node a(NodeFactory().Create(c));
      const Node b(NodeFactory().Create(d));
      assert(!HasSameContent(a,b) && "Order in examples is important and cannot be shuffled");
      assert(a != b);
    }
    {
      //Cannot shuffle Concept its examples. No need to as well: the order is important
      const Concept c = ConceptFactory().Create("1", { {"2", cmap::Competency::uninitialized},{"3", cmap::Competency::uninitialized} } );
      const Concept d = ConceptFactory().Create("1", { {"2", cmap::Competency::uninitialized} } );
      assert(c != d);
      const Node a(NodeFactory().Create(c));
      const Node b(NodeFactory().Create(d));
      assert(a != b);
      assert(!HasSameContent(a,b));
    }
  }
  //Test ConceptFactory reproductions
  {
    const int sz = static_cast<int>(ConceptFactory().GetTests().size());
    for (int i=0; i!=sz; ++i)
    {
      assert(i < static_cast<int>(ConceptFactory().GetTests().size()));
      const Concept c = ConceptFactory().GetTests()[i];
      const Concept d = ConceptFactory().GetTests()[i];
      assert(c == d);
      const Node a(NodeFactory().Create(c));
      const Node b(NodeFactory().Create(d));
      assert(HasSameContent(a,b));
      assert(a == b);
    }
  }
  //Test ConceptFactory reproductions
  {
    const int sz = static_cast<int>(ConceptFactory().GetTests().size());
    for (int i=0; i!=sz; ++i)
    {
      for (int j=0; j!=sz; ++j)
      {
        assert(i < static_cast<int>(ConceptFactory().GetTests().size()));
        const Concept c = ConceptFactory().GetTests()[i];
        const Concept d = ConceptFactory().GetTests()[j];
        if (i!=j)
        {
          assert(c != d);
          const Node a(NodeFactory().Create(c));
          const Node b(NodeFactory().Create(d));
          assert(!HasSameContent(a,b));
          assert(a != b);
        }
        else
        {
          assert(c == d);
          const Node a(NodeFactory().Create(c));
          const Node b(NodeFactory().Create(d));
          assert(HasSameContent(a,b));
          assert(a == b);
        }
      }
    }
  }
}
#endif

std::string ribi::cmap::Node::ToStr() const noexcept
{
  std::stringstream s;
  s << *this;
  return s.str();
}

std::string ribi::cmap::ToXml(const Node& node) noexcept
{
  std::stringstream s;
  s << "<node>";
  s << node.GetConcept().ToXml();
  s << "<x>" << node.GetX() << "</x>";
  s << "<y>" << node.GetY() << "</y>";
  s << "<is_center_node>" << node.IsCenterNode() << "</is_center_node>";
  s << "</node>";
  const std::string r = s.str();
  assert(r.size() >= 13);
  assert(r.substr(0,6) == "<node>");
  assert(r.substr(r.size() - 7,7) == "</node>");

  return r;
}



bool ribi::cmap::operator==(const Node& lhs, const Node& rhs) noexcept
{
  const bool verbose{false};
  const double e{0.1};
  if (lhs.GetConcept() != rhs.GetConcept())
  {
    if (verbose) { TRACE("Concepts differ"); }
    return false;
  }
  if (std::abs(lhs.GetX() - rhs.GetX()) > e)
  {
    if (verbose) { TRACE("X coordinats differ"); }
    return false;
  }
  if (std::abs(lhs.GetY() - rhs.GetY()) > e)
  {
    if (verbose) { TRACE("Y coordinats differ"); }
    return false;
  }
  return true;
}

bool ribi::cmap::operator!=(const Node& lhs, const Node& rhs) noexcept
{
  return !(lhs == rhs);
}

bool ribi::cmap::operator<(const Node& lhs, const Node& rhs) noexcept
{
  if (lhs.GetX() < rhs.GetX()) return true;
  if (lhs.GetX() > rhs.GetX()) return false;
  if (lhs.GetY() < rhs.GetY()) return true;
  if (lhs.GetY() > rhs.GetY()) return false;
  return lhs.GetConcept() < rhs.GetConcept();
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const Node& node) noexcept
{
  os
    << node.GetConcept() << " "
    << node.GetX() << " "
    << node.GetY()
  ;
  return os;
}


