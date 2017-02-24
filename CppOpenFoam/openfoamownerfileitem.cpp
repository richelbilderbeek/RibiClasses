#include "openfoamownerfileitem.h"

#include <cassert>
#include <iostream>
#include <sstream>

ribi::foam::OwnerFileItem::OwnerFileItem(
  const CellIndex& cell_index
  )
  : m_cell_index{cell_index}
{
  #ifndef NDEBUG
  assert(m_cell_index.Get() >= 0);
  #endif
}

#ifndef NDEBUG
void ribi::foam::TestOwnerFileItem() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  
  //operator==
  {
    const OwnerFileItem i(CellIndex(123));
    assert(i == i);
    const OwnerFileItem j(CellIndex(123));
    assert(i == j);
    assert(j == i);
    assert(j == j);
  }
  //operator!=
  {
    const OwnerFileItem i(CellIndex(123));
    const OwnerFileItem j(CellIndex(234));
    assert(i != j);
    assert(j != i);
  }
  //operator<< and operator>>
  {
    const OwnerFileItem i(CellIndex(123));
    std::stringstream s;
    s << i;
    OwnerFileItem j;
    s >> j;
    assert(i == j);
  }
}
#endif

bool ribi::foam::operator==(const OwnerFileItem& lhs, const OwnerFileItem& rhs)
{
  return
       lhs.GetCellIndex() == rhs.GetCellIndex()
  ;
}

bool ribi::foam::operator!=(const OwnerFileItem& lhs, const OwnerFileItem& rhs)
{
  return !(lhs == rhs);
}

std::ostream& ribi::foam::operator<<(std::ostream& os, const OwnerFileItem& item)
{
  os << item.GetCellIndex();
  return os;
}

std::istream& ribi::foam::operator>>(std::istream& is, OwnerFileItem& f)
{
  is >> f.m_cell_index;
  assert(is);
  assert(f.GetCellIndex().Get() >= 0);
  return is;
}
