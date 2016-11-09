#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include "qtublasvectorintmodel.h"

#include <cassert>
#include <numeric>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include "matrix.h"
#pragma GCC diagnostic pop

void ribi::QtUblasVectorIntModel::Test() noexcept
{
  QtUblasVectorIntModel d;
  d.SetRawData( Matrix::CreateVector( { 1,2,3 } ) );
  d.SetRange(1,4,1);
  assert(d.data(d.index(0,0)).toInt() == 1);
  d.setData(d.index(0,0),"0"); //Should not change the data
  assert(d.data(d.index(0,0)).toInt() == 1);
}
