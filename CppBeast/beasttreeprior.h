#ifndef BEASTTREEPRIOR_H
#define BEASTTREEPRIOR_H

#include <iosfwd>

namespace ribi {

enum class TreePrior { birth_death, coalescent_constant_population };

std::string ToStr(const TreePrior p) noexcept;

std::ostream& operator<<(std::ostream& os, const TreePrior p) noexcept;

} //~namespace ribi

#endif // BEASTTREEPRIOR_H
