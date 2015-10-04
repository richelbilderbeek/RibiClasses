#ifndef BEASTTREEPRIOR_H
#define BEASTTREEPRIOR_H

<<<<<<< HEAD
=======
#include <iosfwd>

namespace ribi {

enum class TreePrior { birth_death, coalescent_constant_population };

std::string ToStr(const TreePrior p) noexcept;

std::ostream& operator<<(std::ostream& os, const TreePrior p) noexcept;

} //~namespace ribi
>>>>>>> 50893edd518d344390f109492f1a73bb5e75d898

#endif // BEASTTREEPRIOR_H
