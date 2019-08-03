#ifndef RIBI_TRIANGLEMESHCELLSCHECK_H
#define RIBI_TRIANGLEMESHCELLSCHECK_H

#include <iosfwd>
#include <vector>





#include <boost/shared_ptr.hpp>
#include "trianglemeshfwd.h"


namespace ribi {
namespace trim {

///Checks if Cells are OK
void CellsCheck(const std::vector<boost::shared_ptr<Cell>>& cells) noexcept;

} //~namespace trim
} //~namespace ribi

#endif // RIBI_TRIANGLEMESHCELLSCHECK_H
