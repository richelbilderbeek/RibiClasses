#include "openfoamcellindex.h"

#include <cassert>
#include <iostream>




ribi::foam::CellIndex::CellIndex(const int index)
  : m_index(index)
{
  #ifndef NDEBUG
  Test();
  #endif

  assert(m_index >= -1
    && "A CellIndex must be minus one or higher");
}

ribi::foam::CellIndex& ribi::foam::CellIndex::operator++() noexcept
{
  ++m_index;
  return *this;
}

ribi::foam::CellIndex ribi::foam::CellIndex::operator++(int) noexcept
{
  CellIndex old(*this);
  ++(*this);
  return old;
}


#ifndef NDEBUG
void ribi::foam::CellIndex::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  
  CellIndex a(2);
  CellIndex b(2);
  CellIndex c(3);
  assert(a == a);
  assert(a == b);
  assert(b == a);
  assert(b == b);
  assert(b != c);
  assert(c != b);
  assert(c == c);
}
#endif

std::ostream& ribi::foam::operator<<(std::ostream& os, const CellIndex& cell_index) noexcept
{
  os << cell_index.Get();
  return os;
}

std::istream& ribi::foam::operator>>(std::istream& is, CellIndex& cell_index)
{
  is >> cell_index.m_index;
  assert(is);
  return is;
}

bool ribi::foam::operator==(const CellIndex& lhs, const CellIndex& rhs) noexcept
{
  return lhs.Get() == rhs.Get();
}

bool ribi::foam::operator!=(const CellIndex& lhs, const CellIndex& rhs) noexcept
{
  return !(lhs == rhs);
}

bool ribi::foam::operator<(const CellIndex& lhs, const CellIndex& rhs) noexcept
{
  return lhs.Get() < rhs.Get();
}

bool ribi::foam::operator>(const CellIndex& lhs, const CellIndex& rhs) noexcept
{
  return lhs.Get() > rhs.Get();
}

bool ribi::foam::operator>=(const CellIndex& lhs, const CellIndex& rhs) noexcept
{
  return !(lhs < rhs);
}

bool ribi::foam::operator<=(const CellIndex& lhs, const CellIndex& rhs) noexcept
{
  return !(lhs > rhs);
}

