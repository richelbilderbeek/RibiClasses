#ifndef RIBI_TESTTIMER_H
#define RIBI_TESTTIMER_H

#include <string>
#include <vector>
#include <memory>

namespace ribi {

struct TestTimerImpl;

struct TestTimer
{
  explicit TestTimer(
    const std::string& function_name,
    const std::string& file_name,
    const double max_time_sec = 1.0
  ) noexcept;
  TestTimer(const TestTimer&) = delete;
  TestTimer& operator=(const TestTimer&) = delete;
  ~TestTimer() noexcept; //But might terminate the program instead
  static int GetMaxCnt() noexcept;
  static std::string GetVersion() noexcept;
  static std::vector<std::string> GetVersionHistory() noexcept;
  static void SetMaxCnt(const int max_cnt);

  private:
  const std::unique_ptr<TestTimerImpl> m_impl;
};

using test_timer = TestTimer; //Alias

} //~namespace ribi

#endif // RIBI_TESTTIMER_H
