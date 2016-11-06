#include "counter.h"

#include <iostream>

ribi::Counter::Counter(const int initial_count) noexcept
  : m_count(initial_count), m_verbosity{false}
{

}

std::string ribi::Counter::GetVersion() noexcept
{
  return "1.3";
}

std::vector<std::string> ribi::Counter::GetVersionHistory() noexcept
{
  return {
    "2011-08-20: Version 1.0: initial version",
    "2014-08-01: Version 1.1: added tests",
    "2014-04-18: Version 1.2: added operator++, operator== and operator!=",
    "2014-04-18: Version 1.3: added verbosity"
  };
}

void ribi::Counter::Inc() noexcept
{
  ++m_count;

  if (m_verbosity)
  {
    std::clog << "Incremented counter to " << m_count;
  }
}

bool ribi::operator==(const ribi::Counter& lhs, const ribi::Counter& rhs) noexcept
{
  return lhs.Get() == rhs.Get();
}

bool ribi::operator!=(const ribi::Counter& lhs, const ribi::Counter& rhs) noexcept
{
  return !(lhs == rhs);
}
