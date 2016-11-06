#ifndef RIBI_FUZZY_EQUAL_TO_H
#define RIBI_FUZZY_EQUAL_TO_H

#include <cassert>
#include <cmath>
#include <functional>
#include <limits>
#include <string>
#include <vector>


namespace ribi {

///fuzzy_equal_to is a predicate to test two doubles for equality
///with a certain tolerance. A tolerance of 0.0 denotes that
///an exact match is requested. Note that the value of 0.0 cannot
///be compared fuzzily.
///From http://www.richelbilderbeek.nl/CppFuzzy_equal_to.htm
struct fuzzy_equal_to
{
  explicit fuzzy_equal_to(const double tolerance = std::numeric_limits<double>::epsilon())
    : m_tolerance(tolerance)
  {
    assert(tolerance >= 0.0);
  }
  bool operator()(const double lhs, const double rhs) const
  {
    assert(lhs != 0.0);
    return rhs > (1.0 - m_tolerance) * lhs
        && rhs < (1.0 + m_tolerance) * lhs;
  }
  const double m_tolerance;

  static std::string GetVersion()
  {
    return "1.4";
  }
  static std::vector<std::string> GetVersionHistory()
  {
    return {
      "2011-02-24: version 1.0: initial version",
      "2012-11-14: version 1.1: set default tolerance to std::numeric_limits<double>::epsilon()",
      "2012-11-18: version 1.2: added #pragma's to file to suppress -Weffc++ warnings",
      "2012-11-18: version 1.3: added #include <limits>",
      "2014-12-27: version 1.4: do not inherit from deprecated std::binary_function"
    };
  }
};

///fuzzy_equal_to_abs is a predicate to test two doubles for equality
///by allowing a certain absolute difference between the two values
///A tolerance of 0.0 denotes that an exact match is requested.
///From http://www.richelbilderbeek.nl/CppFuzzy_equal_to.htm
struct fuzzy_equal_to_abs
{
  explicit fuzzy_equal_to_abs(const double tolerance = std::numeric_limits<double>::epsilon())
    : m_tolerance(tolerance)
  {
    assert(tolerance >= 0.0);
  }
  bool operator()(const double lhs, const double rhs) const noexcept
  {
    return std::abs(lhs-rhs) < m_tolerance;
  }
  const double m_tolerance;

  static std::string GetVersion() noexcept
  {
    return "1.0";
  }
  static std::vector<std::string> GetVersionHistory() noexcept
  {
    return {
      "2013-11-05: version 1.0: initial version from fuzzy_equal_to"
    };
  }
};

} //~namespace ribi

#endif // RIBI_FUZZY_EQUAL_TO_H
