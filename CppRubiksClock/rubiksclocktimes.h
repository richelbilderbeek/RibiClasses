#ifndef RUBIKSCLOCKTIMES_H
#define RUBIKSCLOCKTIMES_H

#include <iosfwd>
#include <string>





#include <boost/shared_ptr.hpp>

#include "rubiksclockside.h"


namespace ribi {
namespace ruco {

struct ClockDialWidget;

///The nine times on a Rubik's Clock
struct Times
{
  Times(const bool is_front) noexcept;
  boost::shared_ptr<ClockDialWidget> times[3][3];
  std::string ToXml() const noexcept;

  private:
  ~Times() noexcept {}
  friend void boost::checked_delete<>(Times*);
  friend void boost::checked_delete<>(const Times*);
};

std::ostream& operator<<(std::ostream& os, const Times& t) noexcept;
bool operator==(const Times& lhs, const Times& rhs) noexcept;

} //~namespace ruco
} //~namespace ribi

#endif // RUBIKSCLOCKTIMES_H
