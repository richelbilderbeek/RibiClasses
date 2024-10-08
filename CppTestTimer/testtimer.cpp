#include "testtimer.h"

#include <cassert>
#include <sstream>
#include <iostream>

#include <boost/timer/timer.hpp>

namespace ribi {

std::string ExtractFilename(const std::string& s) noexcept
{
  const auto pos_1 = s.find_last_of('/');
  const auto pos_2 = s.find_last_of('\\');
  const auto pos = std::min(pos_1,pos_2);
  if (pos == std::string::npos) return s;
  return s.substr(pos + 1,s.size() - pos - 1);
}

struct TestTimerImpl
{
  explicit TestTimerImpl(
    const std::string& function_name,
    const std::string& file_name,
    const double max_time_sec
  )
    : m_file_name{file_name},
      m_function_name{function_name},
      m_max_time_sec{max_time_sec},
      m_timer{}
  {
    assert(m_file_name.find('\n') == std::string::npos);
    assert(m_file_name.find('\r') == std::string::npos);
    assert(m_file_name.find('\t') == std::string::npos);
    assert(m_function_name.find('\n') == std::string::npos);
    assert(m_function_name.find('\r') == std::string::npos);
    assert(m_function_name.find('\t') == std::string::npos);
    ++m_cnt;
    ++m_n_tests;
  }
  ~TestTimerImpl() noexcept
  {
    --m_cnt;
  }

  ///Number of active tests
  static int m_cnt;
  ///Maximum number of active tests
  static int m_max_cnt;
  ///Filename where the most recent test is active
  const std::string m_file_name;
  ///Function name where the most recent test is active
  const std::string m_function_name;
  ///Maximum allowed time for a test
  const double m_max_time_sec;
  ///Timer to measure the duration of a test
  boost::timer::auto_cpu_timer m_timer;
  ///The number of tests started
  static int m_n_tests;
};

int TestTimerImpl::m_cnt = 0;
int TestTimerImpl::m_max_cnt = 2;
int TestTimerImpl::m_n_tests = 0;

} //~namespace ribi


ribi::TestTimer::TestTimer(
  const std::string& function_name,
  const std::string& file_name,
  const double max_time_sec
) noexcept

#if __cplusplus >= 201402L //C++17
  : m_impl(std::make_unique<TestTimerImpl>(function_name,file_name,max_time_sec))
#endif
#if __cplusplus < 201402L
  : m_impl(new TestTimerImpl(function_name,file_name,max_time_sec))
#endif
{
  #ifdef REALLY_KEEP_TESTTIMER_OUTPUT_BEFORE_IT_IS_OBSOLETED
  std::clog
    << std::string(m_impl->m_cnt - 1,' ')
    << "\\ START: "
    << ExtractFilename(m_impl->m_file_name)
    << ','
    << m_impl->m_function_name
    << " #"
    << m_impl->m_n_tests
    << std::endl
  ;

  if (m_impl->m_cnt == m_impl->m_max_cnt)
  {
    std::clog
      << "WARNING: "
      << m_impl->m_file_name
      << ','
      << m_impl->m_function_name
      << ": count equals " << m_impl->m_cnt
      << std::endl
    ;
  }
  #endif // REALLY_KEEP_TESTTIMER_OUTPUT_BEFORE_IT_IS_OBSOLETED
  assert(m_impl->m_cnt < m_impl->m_max_cnt && "TestTimer can only have max_cnt TestTimer instances active");
}

ribi::TestTimer::~TestTimer() noexcept
{
  #ifdef REALLY_KEEP_TESTTIMER_OUTPUT_BEFORE_IT_IS_OBSOLETED
  const double elapsed_secs = m_impl->m_timer.elapsed();
  if (elapsed_secs > m_impl->m_max_time_sec)
  {
    std::clog
      << std::string(m_impl->m_cnt - 1,' ')
      << " | FUNCTION '"
      << m_impl->m_function_name
      << "' IN FILE '"
      << m_impl->m_file_name
      << "' TOOK TOO LONG"
      << std::endl
    ;
  }
  std::clog
    << std::string(m_impl->m_cnt - 1,' ')
    << "/ DONE : "
    << ExtractFilename(m_impl->m_file_name)
    << ','
    << m_impl->m_function_name
    << " ("
    << elapsed_secs
    << " seconds)"
    << std::endl
  ;
  #endif // REALLY_KEEP_TESTTIMER_OUTPUT_BEFORE_IT_IS_OBSOLETED
}

std::string ribi::TestTimer::GetVersion() noexcept
{
  return "1.5";
}

std::vector<std::string> ribi::TestTimer::GetVersionHistory() noexcept
{
  return {
    "2014-08-02: version 1.0: initial version",
    "2014-08-08: version 1.1: allow setting a maximum amount of TestTimers active",
    "2014-08-10: version 1.2: count the number of constructed TestTimers",
    "2014-12-24: version 1.3: added GetMaxCnt",
    "2014-12-31: version 1.4: use of std::unique_ptr",
    "2016-04-09: version 1.5: should not call std::exit in desctructor",
  };
}

int ribi::TestTimer::GetMaxCnt() noexcept
{
  return TestTimerImpl::m_max_cnt - 1;
}

void ribi::TestTimer::SetMaxCnt(const int max_cnt)
{
  if (max_cnt < 1)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "TestTimer max_count must be >= 1, instead of " << max_cnt
    ;
    throw std::invalid_argument(msg.str());
  }
  TestTimerImpl::m_max_cnt = max_cnt + 1;
}
