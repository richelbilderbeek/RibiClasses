#include "trianglemeshwindings.h"

#include <cassert>
#include <stdexcept>



std::vector<ribi::trim::Winding> ribi::trim::Windings::GetAll() const noexcept
{
  const std::vector<Winding> v {
    Winding::clockwise,
    Winding::counter_clockwise,
    Winding::indeterminate
  };
  assert(static_cast<int>(v.size()) == static_cast<int>(Winding::n_types));
  return v;
}

std::string ribi::trim::Windings::ToStr(const Winding winding) const noexcept
{
  switch (winding)
  {
    case Winding::clockwise: return "clockwise";
    case Winding::counter_clockwise: return "counter_clockwise";
    case Winding::indeterminate: return "indeterminate";
    default:
      assert(!"Should not get here");
      return "";
  }
}
