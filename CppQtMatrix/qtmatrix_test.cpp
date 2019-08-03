#pragma GCC diagnostic push


#include "qtmatrix.h"

#include <cassert>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <QTableWidget>

#pragma GCC diagnostic pop


void ribi::QtMatrixTest() noexcept
{
  {
    const std::vector<std::string> v = {};
    QTableWidget * const table = new QTableWidget;
    StrVectorToTable(v,table);
    assert(table->rowCount() == 0);
    assert(table->columnCount() == 0);
  }
  {
    const std::vector<std::string> v = { "X" };
    QTableWidget * const table = new QTableWidget;
    StrVectorToTable(v,table);
    assert(table->rowCount() == 1);
    assert(table->columnCount() == 1);
  }
  {
    const std::vector<std::string> v = { "A","B" };
    QTableWidget * const table = new QTableWidget;
    StrVectorToTable(v,table);
    assert(table->rowCount() == 2);
    assert(table->columnCount() == 1);
  }
  {
    const std::vector<std::string> v = { "A","BB","CCC" };
    QTableWidget * const table = new QTableWidget;
    StrVectorToTable(v,table);
    assert(table->rowCount() == 3);
    assert(table->columnCount() == 1);
  }
}
