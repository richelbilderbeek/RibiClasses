#include "beasttreeprior.h"
<<<<<<< HEAD
=======

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
  throw std::logic_error("ribi::ToStr(const TreePrior p): unknown TreePrior");
}

std::ostream& ribi::operator<<(std::ostream& os, const TreePrior p) noexcept
{
  os << ToStr(p);
  return os;
}
>>>>>>> 50893edd518d344390f109492f1a73bb5e75d898
