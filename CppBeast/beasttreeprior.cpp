#include "beasttreeprior.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

std::string ribi::ToStr(const TreePrior p) noexcept
{
  switch (p)
  {
    case TreePrior::birth_death: return "birth_death";
    case TreePrior::coalescent_constant_population: return "coalescent_constant_population";
  }
  assert(!"Should not get here");
  return "";
}

std::ostream& ribi::operator<<(std::ostream& os, const TreePrior p) noexcept
{
  os << ToStr(p);
  return os;
}
