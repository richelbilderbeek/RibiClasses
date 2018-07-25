#ifndef RIBI_TIME_H
#define RIBI_TIME_H

#include <string>
#include <vector>

namespace ribi {

///Regex functions
struct Time
{
  Time();

  //Returns date in YYYY-MM-DD format
  std::string GetTodayIso8601() const noexcept { return GetTodayIso8601Stl(); }

  //Returns date in YYYY-MM-DD format using Boost
  std::string GetTodayIso8601Boost() const noexcept;

  //Returns date in YYYY-MM-DD format using the STL
  std::string GetTodayIso8601Stl() const noexcept;

  //Returns date in YYYYMMDD format
  int GetTodayIso8601AsInt() const noexcept;

  static std::string GetVersion() noexcept;
  static std::vector<std::string> GetVersionHistory() noexcept;

  void Wait(const double n_secs) const noexcept;

  private:


};

} //~namespace ribi

#endif // RIBI_TIME_H
