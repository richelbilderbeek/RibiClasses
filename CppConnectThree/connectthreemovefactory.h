#ifndef CONNECTTHREEMOVEFACTORY_H
#define CONNECTTHREEMOVEFACTORY_H

#include "connectthreefwd.h"
#include "connectthreeplayer.h"

namespace ribi {
namespace con3 {

struct MoveFactory
{
  Move Create(const int x, const int y, const Player player) const noexcept;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

} //~namespace con3
} //~namespace ribi

#endif // CONNECTTHREEMOVEFACTORY_H
