#include "conceptmapregex.h"

#include <cassert>

#include "ribi_regex.h"

#include "trace.h"

ribi::cmap::Regex::Regex()
{

}

std::string ribi::cmap::Regex::GetRegexCenterNode() const noexcept
{
  return "(<center_node>.*?</center_node>)";
}


std::string ribi::cmap::Regex::GetRegexCompetency() const noexcept
{
  return "(<competency>.*?</competency>)";
}

std::string ribi::cmap::Regex::GetRegexComplexity() const noexcept
{
  return "(<complexity>.*?</complexity>)";
}

std::string ribi::cmap::Regex::GetRegexConcept() const noexcept
{
  return "(<concept>.*?</concept>)";
}

std::string ribi::cmap::Regex::GetRegexConceptMap() const noexcept
{
  return "(<conceptmap>.*?</conceptmap>)";
}

std::string ribi::cmap::Regex::GetRegexConceptIsComplex() const noexcept
{
  return "(<concept_is_complex>.*?</concept_is_complex>)";
}

std::string ribi::cmap::Regex::GetRegexConcreteness() const noexcept
{
  return "(<concreteness>.*?</concreteness>)";
}

std::string ribi::cmap::Regex::GetRegexEdge() const noexcept
{
  return "(<edge>.*?</edge>)";
}

std::string ribi::cmap::Regex::GetRegexEdges() const noexcept
{
  return "(<edges>.*?</edges>)";
}

std::string ribi::cmap::Regex::GetRegexExample() const noexcept
{
  return "(<example>.*?</example>)";
}

std::string ribi::cmap::Regex::GetRegexExamples() const noexcept
{
  return "(<examples>.*?</examples>)";
}

std::string ribi::cmap::Regex::GetRegexFrom() const noexcept
{
  return "(<from>.*?</from>)";
}

std::string ribi::cmap::Regex::GetRegexHasHead() const noexcept
{
  return "(<has_head>.*?</has_head>)";
}

std::string ribi::cmap::Regex::GetRegexHasTail() const noexcept
{
  return "(<has_tail>.*?</has_tail>)";
}

std::string ribi::cmap::Regex::GetRegexIsCenterNode() const noexcept
{
  return "(<is_center_node>.*?</is_center_node>)";
}

std::string ribi::cmap::Regex::GetRegexIsComplex() const noexcept
{
  return "(<is_complex>.*?</is_complex>)";
}

std::string ribi::cmap::Regex::GetRegexIsConcrete() const noexcept
{
  return "(<is_concrete>.*?</is_concrete>)";
}

std::string ribi::cmap::Regex::GetRegexIsSpecific() const noexcept
{
  return "(<is_specific>.*?</is_specific>)";
}

std::string ribi::cmap::Regex::GetRegexName() const noexcept
{
  return "(<name>.*?</name>)";
}

std::string ribi::cmap::Regex::GetRegexNode() const noexcept
{
  return "(<node>.*?</node>)";
}

std::string ribi::cmap::Regex::GetRegexNodes() const noexcept
{
  return "(<nodes>.*?</nodes>)";
}

std::string ribi::cmap::Regex::GetRegexSpecificity() const noexcept
{
  return "(<specificity>.*?</specificity>)";
}


std::string ribi::cmap::Regex::GetRegexText() const noexcept
{
  return "(<text>.*?</text>)";
}

std::string ribi::cmap::Regex::GetRegexTo() const noexcept
{
  return "(<to>.*?</to>)";
}

std::string ribi::cmap::Regex::GetRegexX() const noexcept
{
  return "(<x>.*?</x>)";
}

std::string ribi::cmap::Regex::GetRegexY() const noexcept
{
  return "(<y>.*?</y>)";
}

std::vector<std::string>
  ribi::cmap::Regex::GetRegexMatches(
  const std::string& s,
  const std::string& regex_str
) const noexcept
{
  assert(ribi::Regex().IsValid(regex_str));
  return ribi::Regex().GetRegexMatches(s,regex_str);
}


