#include "connectthreemovefactory.h"

#include <cassert>
#include "connectthreemove.h"

#include "testtimer.h"
#include "trace.h"

ribi::con3::Move ribi::con3::MoveFactory::Create(
  const int x, const int y, const Player player) const noexcept
{
  const Move move {
    x,y,player
  };
  return move;
}

#ifndef NDEBUG
void ribi::con3::MoveFactory::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  const TestTimer test_timer(__func__,__FILE__,1.0);
}
#endif