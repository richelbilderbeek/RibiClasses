#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>
#include <string>
#include <vector>

namespace ribi {

struct Stopwatch
{
  Stopwatch() noexcept;
  double GetElapsedSecs() const noexcept;
  std::string GetVersion() const noexcept;
  std::vector<std::string> GetVersionHistory() const noexcept;
  private:
  std::chrono::system_clock::time_point m_time_start;
};

} //~namespace ribi

#endif // STOPWATCH_H
