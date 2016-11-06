#ifndef QTROUNDEDEDITRECTITEMPADDING_H
#define QTROUNDEDEDITRECTITEMPADDING_H

#include <iosfwd>

namespace ribi {

struct QtRoundedEditRectItemPadding
{
  QtRoundedEditRectItemPadding(
    const double any_top = 0.0,
    const double any_right = 0.0,
    const double any_bottom = 0.0,
    const double any_left = 0.0
  )
    : bottom(any_bottom), left(any_left), right(any_right), top(any_top) {}
  double bottom;
  double left;
  double right;
  double top;
};

std::ostream& operator<<(std::ostream& os, const QtRoundedEditRectItemPadding &p) noexcept;
bool operator==(const QtRoundedEditRectItemPadding& lhs, const QtRoundedEditRectItemPadding& rhs) noexcept;
bool operator!=(const QtRoundedEditRectItemPadding& lhs, const QtRoundedEditRectItemPadding& rhs) noexcept;

} //~namespace ribi

#endif // QTROUNDEDEDITRECTITEMPADDING_H
