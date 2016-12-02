#include "dnasequence.h"

#include <algorithm>
#include <cassert>
#include <sstream>
#include <stdexcept>

#include "container.h"


ribi::DnaSequence::DnaSequence(const std::string& description, const std::string& sequence)
  : m_description{description},
    m_sequence{CleanSequence(sequence)}
{

}

std::string ribi::CleanSequence(std::string s)
{
  //Remove all whitespace
  s = Container().RemoveWhitespace(s);
  s = Container().ToUpper(s);
  const auto iter = std::find_if_not(
    std::begin(s),
    std::end(s),
    [](const char c) { return c == 'A' || c == 'C' ||  c == 'G' ||  c == 'T'; }
  );
  if (iter != std::end(s))
  {
    std::stringstream t;
    t << "Sequence::Sequence: sequence can only contain the characters 'A','C','G','T'"
      << ", found character: " << *iter
    ;
    throw std::invalid_argument(t.str().c_str());
  }
  return s;
}


bool ribi::operator==(const DnaSequence& lhs, const DnaSequence& rhs) noexcept
{
  return
    lhs.GetDescription() == rhs.GetDescription()
    && lhs.GetSequence() == rhs.GetSequence()
  ;
}

bool ribi::operator!=(const DnaSequence& lhs, const DnaSequence& rhs) noexcept
{
  return !(lhs == rhs);
}

