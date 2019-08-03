#pragma GCC diagnostic push


#include "qtublasvectordoublemodel.h"

#include <cassert>
#include <boost/lexical_cast.hpp>
#include "matrix.h"
#pragma GCC diagnostic pop

void ribi::QtUblasVectorDoubleModelTest() noexcept
{
  QtUblasVectorDoubleModel model;
  assert(model.rowCount()    == 0);
  assert(model.columnCount() == 0);
  model.insertRow(1);
  assert(model.rowCount()    == 1);
  assert(model.columnCount() == 1);
  model.removeRow(1);
  assert(model.rowCount()    == 0);
  assert(model.columnCount() == 0);
}
