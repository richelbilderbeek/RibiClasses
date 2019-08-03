//---------------------------------------------------------------------------
/*
Matrix, functions working on vectors and matrices
Copyright (C) 2013-2015 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppMatrix.htm
//---------------------------------------------------------------------------



#include "matrix.h"

#include <iostream>

#include <boost/numeric/conversion/cast.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/blas.hpp> //boost::numeric::ublas::equals


#include "ribi_random.h"

double ribi::Matrix::CalcDeterminant(boost::numeric::ublas::matrix<double> m)
{
  assert(m.size1() == m.size2() && "Can only calculate the determinant of square matrices");
  boost::numeric::ublas::permutation_matrix<std::size_t> pivots(m.size1() );

  const int is_singular = boost::numeric::ublas::lu_factorize(m, pivots);

  if (is_singular) return 0.0;

  double d = 1.0;
  const std::size_t sz = pivots.size();
  for (std::size_t i=0; i != sz; ++i)
  {
    if (pivots(i) != i)
    {
      d *= -1.0;
    }
    d *= m(i,i);
  }
  return d;
}

const std::vector<boost::numeric::ublas::matrix<double> > ribi::Matrix::Chop(
  const boost::numeric::ublas::matrix<double>& m)
{
  using boost::numeric::ublas::range;
  using boost::numeric::ublas::matrix;
  using boost::numeric::ublas::matrix_range;
  std::vector<matrix<double> > v;
  v.reserve(4);
  const int midy = m.size1() / 2;
  const int midx = m.size2() / 2;
  const matrix_range<const matrix<double> > top_left(    m,range(0   ,midy     ),range(0   ,midx     ));
  const matrix_range<const matrix<double> > bottom_left( m,range(midy,m.size1()),range(0   ,midx     ));
  const matrix_range<const matrix<double> > top_right(   m,range(0   ,midy     ),range(midx,m.size2()));
  const matrix_range<const matrix<double> > bottom_right(m,range(midy,m.size1()),range(midx,m.size2()));
  v.push_back(matrix<double>(top_left));
  v.push_back(matrix<double>(top_right));
  v.push_back(matrix<double>(bottom_left));
  v.push_back(matrix<double>(bottom_right));
  return v;
}

boost::numeric::ublas::matrix<double> ribi::Matrix::CreateMatrix(
  const std::size_t n_rows,
  const std::size_t n_cols,
  const std::vector<double>& v) noexcept
{
  assert(n_rows * n_cols == v.size());
  boost::numeric::ublas::matrix<double> m(n_rows,n_cols);
  for (std::size_t row = 0; row!=n_rows; ++row)
  {
    for (std::size_t col = 0; col!=n_cols; ++col)
    {
      m(row,col) = v[ (col * n_rows) + row];
    }
  }
  return m;
}

boost::numeric::ublas::matrix<double> ribi::Matrix::CreateRandomMatrix(
  const std::size_t n_rows, const std::size_t n_cols) noexcept
{
  boost::numeric::ublas::matrix<double> m(n_rows,n_cols);
  for (std::size_t row=0; row!=n_rows; ++row)
  {
    for (std::size_t col=0; col!=n_cols; ++col)
    {
      m(row,col) = Random().GetFraction();
    }
  }
  return m;
}

boost::numeric::ublas::vector<double> ribi::Matrix::CreateVector(
  const std::vector<double>& v) noexcept
{
  boost::numeric::ublas::vector<double> w(v.size());
  std::copy(v.begin(),v.end(),w.begin());
  return w;
}

const boost::numeric::ublas::vector<int> ribi::Matrix::CreateVectorInt(
  const std::vector<int>& v) noexcept
{
  boost::numeric::ublas::vector<int> w(v.size());
  std::copy(v.begin(),v.end(),w.begin());
  return w;
}

std::string ribi::Matrix::GetVersion() noexcept
{
  return "1.3";
}

std::vector<std::string> ribi::Matrix::GetVersionHistory() noexcept
{
  return {
    "2013-04-28: version 1.0: initial version",
    "2013-06-11: version 1.1: fixed bugs in MatricesAreEqual and MatricesAreAboutEqual",
    "2013-06-27: version 1.2: added tests, renamed VectorsAreEqual to VectorsDoubleAreEqual and VectorsIntAreEqual"
    "2013-09-16: version 1.3: noexcept"
  };
}

const boost::numeric::ublas::matrix<double> ribi::Matrix::Inverse(
  const boost::numeric::ublas::matrix<double>& m)
{
  assert(m.size1() == m.size2() && "Can only calculate the inverse of square matrices");

  switch(m.size1())
  {
    case 0:
    {
      return m;
    }
    case 1:
    {
      assert(m.size1() == 1 && m.size2() == 1 && "Only for 1x1 matrices");
      const double determinant = CalcDeterminant(m);
      assert(determinant != 0.0);
      assert(m(0,0) != 0.0 && "Cannot take the inverse of matrix [0]");
      boost::numeric::ublas::matrix<double> n(1,1);
      n(0,0) =  1.0 / determinant;
      return n;
    }
    case 2:
    {
      assert(m.size1() == 2 && m.size2() == 2 && "Only for 2x2 matrices");
      const double determinant = CalcDeterminant(m);
      assert(determinant != 0.0);
      const double a = m(0,0);
      const double b = m(0,1);
      const double c = m(1,0);
      const double d = m(1,1);
      boost::numeric::ublas::matrix<double> n(2,2);
      n(0,0) =  d / determinant;
      n(0,1) = -b / determinant;
      n(1,0) = -c / determinant;
      n(1,1) =  a / determinant;
      return n;
    }
    case 3:
    {
      assert(m.size1() == 3 && m.size2() == 3 && "Only for 3x3 matrices");
      const double determinant = CalcDeterminant(m);
      assert(determinant != 0.0);
      const double a = m(0,0);
      const double b = m(0,1);
      const double c = m(0,2);
      const double d = m(1,0);
      const double e = m(1,1);
      const double f = m(1,2);
      const double g = m(2,0);
      const double h = m(2,1);
      const double k = m(2,2);
      boost::numeric::ublas::matrix<double> n(3,3);
      const double new_a =  ((e*k)-(f*h)) / determinant;
      const double new_b = -((d*k)-(f*g)) / determinant;
      const double new_c =  ((d*h)-(e*g)) / determinant;
      const double new_d = -((b*k)-(c*h)) / determinant;
      const double new_e =  ((a*k)-(c*g)) / determinant;
      const double new_f = -((a*h)-(b*g)) / determinant;
      const double new_g =  ((b*f)-(c*e)) / determinant;
      const double new_h = -((a*f)-(c*d)) / determinant;
      const double new_k =  ((a*e)-(b*d)) / determinant;
      n(0,0) = new_a;
      n(1,0) = new_b;
      n(2,0) = new_c;
      n(0,1) = new_d;
      n(1,1) = new_e;
      n(2,1) = new_f;
      n(0,2) = new_g;
      n(1,2) = new_h;
      n(2,2) = new_k;
      return n;
    }
    default:
    {
      //Use blockwise inversion
      //ribi::Matrix::Chop returns a std::vector
      //[ A at [0]   B at [1] ]
      //[ C at [2]   D at [4] ]
      assert(m.size1() > 3);
      assert(m.size2() > 3);
      const std::vector<boost::numeric::ublas::matrix<double> > v = Chop(m);
      const boost::numeric::ublas::matrix<double>& a = v[0];
      assert(a.size1() == a.size2());
      const boost::numeric::ublas::matrix<double>  a_inv = Inverse(a);
      const boost::numeric::ublas::matrix<double>& b = v[1];
      const boost::numeric::ublas::matrix<double>& c = v[2];
      const boost::numeric::ublas::matrix<double>& d = v[3];
      const boost::numeric::ublas::matrix<double> term
        = d
        - ribi::Matrix::Prod(
            boost::numeric::ublas::matrix<double>(ribi::Matrix::Prod(c,a_inv)),
            b
          );
      assert(term.size1() == term.size2());
      const boost::numeric::ublas::matrix<double> term_inv = Inverse(term);
      const boost::numeric::ublas::matrix<double> new_a
        = a_inv
        + boost::numeric::ublas::matrix<double>(ribi::Matrix::Prod(
            boost::numeric::ublas::matrix<double>(ribi::Matrix::Prod(
              boost::numeric::ublas::matrix<double>(ribi::Matrix::Prod(
                boost::numeric::ublas::matrix<double>(ribi::Matrix::Prod(
                  a_inv,
                  b)),
                term_inv)),
             c)),
            a_inv));

      const boost::numeric::ublas::matrix<double> new_b
        =
        - boost::numeric::ublas::matrix<double>(ribi::Matrix::Prod(
            boost::numeric::ublas::matrix<double>(ribi::Matrix::Prod(
              a_inv,
              b)),
            term_inv));

      const boost::numeric::ublas::matrix<double> new_c
        =
        - boost::numeric::ublas::matrix<double>(ribi::Matrix::Prod(
            boost::numeric::ublas::matrix<double>(ribi::Matrix::Prod(
              term_inv,
              c)),
            a_inv));

      const boost::numeric::ublas::matrix<double> new_d = term_inv;
      const std::vector<boost::numeric::ublas::matrix<double> > w = { new_a, new_b, new_c, new_d };
      const boost::numeric::ublas::matrix<double> result = Unchop(w);
      return result;
    }
  }
}


bool ribi::Matrix::IsAboutEqual(const double a, const double b) noexcept
{
  const double epsilon = 0.000001; //Rounding error
  return a - epsilon < b && a + epsilon > b;
}

const boost::numeric::ublas::matrix<double> ribi::Matrix::Power(
  const boost::numeric::ublas::matrix<double>& m,
  const int exponent)
{
  assert(exponent >= 0 && "Can only take the power of matrices with a positive exponent");
  assert(m.size1() == m.size2() && "Can only take the power of square matrices");
  const std::size_t sz = m.size1();
  if (exponent == 0) return boost::numeric::ublas::identity_matrix<double>(sz);
  if (exponent == 1) return m;
  boost::numeric::ublas::matrix<double> n(m);
  for (int i=1; i!=exponent; ++i)
  {
    n = ribi::Matrix::Prod(n,m);
  }
  return n;
}

bool ribi::Matrix::MatricesAreAboutEqual(
  const boost::numeric::ublas::matrix<double>& a,
  const boost::numeric::ublas::matrix<double>& b) noexcept
{
  if (a.size1() != b.size1()) return false;
  if (a.size2() != b.size2()) return false;
  //return std::equal(a.begin1(),a.end1(),b.begin1(),&ribi::Matrix::IsAboutEqual); //DON'T USE STD::EQUAL!

  assert(a.size1() == b.size1());
  assert(a.size2() == b.size2());

  const std::size_t n_rows = a.size1();
  const std::size_t n_cols = a.size2();
  for (std::size_t row = 0; row != n_rows; ++row)
  {
    for (std::size_t col = 0; col != n_cols; ++col)
    {
      if (!IsAboutEqual(a(row,col),b(row,col))) return false;
    }
  }
  return true;
}

bool ribi::Matrix::MatricesAreEqual(
  const boost::numeric::ublas::matrix<double>& a,
  const boost::numeric::ublas::matrix<double>& b) noexcept
{
  if ( a.size1() != b.size1()
    || a.size2() != b.size2()) return false;
  //const bool is_equal = std::equal(a.begin1(),a.end1(),b.begin1()); //DON'T USE STD::EQUAL!

  assert(a.size1() == b.size1());
  assert(a.size2() == b.size2());

  const std::size_t n_rows = a.size1();
  const std::size_t n_cols = a.size2();
  for (std::size_t row = 0; row != n_rows; ++row)
  {
    for (std::size_t col = 0; col != n_cols; ++col)
    {
      if (a(row,col) != b(row,col)) return false;
    }
  }
  return true;
}

bool ribi::Matrix::MatrixIsAboutEqual(
  const boost::numeric::ublas::matrix<double>& a,
  const boost::numeric::ublas::matrix<double>& b) noexcept
{
  //TRACE("Deprecated naming");
  return MatricesAreAboutEqual(a,b);
}

const boost::numeric::ublas::matrix<double> ribi::Matrix::MultiProd(
  const boost::numeric::ublas::matrix<double>& a,
  const boost::numeric::ublas::matrix<double>& b,
  const boost::numeric::ublas::matrix<double>& c)
{
  return Prod(Prod(a,b),c);
}

const boost::numeric::ublas::matrix<double> ribi::Matrix::Prod(
  const boost::numeric::ublas::matrix<double>& a,
  const boost::numeric::ublas::matrix<double>& b
  )
{
  assert(a.size2() == b.size1() && "A's width must be B's height");
  return boost::numeric::ublas::prod(a,b);
}

const boost::numeric::ublas::vector<double> ribi::Matrix::Prod(
  const boost::numeric::ublas::matrix<double>& a,
  const boost::numeric::ublas::vector<double>& b
  )
{
  assert(a.size2() == b.size() && "A's width must be B's height");
  return boost::numeric::ublas::prod(a,b);
}

const boost::numeric::ublas::matrix<double> ribi::Matrix::SimplifyMatrixOfMatrix(
  const boost::numeric::ublas::matrix<boost::numeric::ublas::matrix<double> >& m)
{
  // [ [A B]  [I J] ]
  // [ [C D]  [K L] ]    [ A B I J]
  // [              ]    [ C D K L]
  // [ [E F]  [M N] ]    [ E F M N]
  // [ [G H]  [O P] ] -> [ G H O P]
  //assert(m.size1() > 0);
  //assert(m.size2() > 0);
  //
  const int n_sub_rows = boost::numeric_cast<int>(m.size1());
  const int n_sub_cols = boost::numeric_cast<int>(m.size2());
  const int sub_rows   = n_sub_rows != 0 && n_sub_cols != 0 ? boost::numeric_cast<int>(m(0,0).size1()) : 0;
  const int sub_cols   = n_sub_cols != 0 && n_sub_cols != 0 ? boost::numeric_cast<int>(m(0,0).size2()) : 0;
  boost::numeric::ublas::matrix<double> v(n_sub_rows * sub_rows, n_sub_cols * sub_cols);
  for (int sub_row=0; sub_row!=n_sub_rows; ++sub_row)
  {
    for (int sub_col=0; sub_col!=n_sub_cols; ++sub_col)
    {
      assert(sub_row < boost::numeric_cast<int>(m.size1()));
      assert(sub_col < boost::numeric_cast<int>(m.size2()));
      const boost::numeric::ublas::matrix<double>& sub = m(sub_row,sub_col);
      const int offset_x = sub_col * sub_cols;
      const int offset_y = sub_row * sub_rows;
      for (int row=0; row!=sub_rows; ++row)
      {
        for (int col=0; col!=sub_cols; ++col)
        {
          assert(offset_y + row < boost::numeric_cast<int>(v.size1()));
          assert(offset_x + col < boost::numeric_cast<int>(v.size2()));
          assert(row < boost::numeric_cast<int>(sub.size1()));
          assert(col < boost::numeric_cast<int>(sub.size2()));
          v(offset_y + row,offset_x + col) = sub(row,col);
        }
      }
    }
  }
  //
  return v;
}


const boost::numeric::ublas::matrix<double> ribi::Matrix::SimplifyVectorOfMatrix(
  const boost::numeric::ublas::vector<boost::numeric::ublas::matrix<double> >& m)
{
  // [ [A B] ]
  // [ [C D] ]    [ A B ]
  // [       ]    [ C D ]
  // [ [E F] ]    [ E F ]
  // [ [G H] ] -> [ G H ]

  assert(!m.empty());
  const int n_subs   = boost::numeric_cast<int>(m.size());
  const int sub_rows = boost::numeric_cast<int>(m[0].size1());
  const int sub_cols = boost::numeric_cast<int>(m[0].size2());
  boost::numeric::ublas::matrix<double> v(n_subs * sub_rows, sub_cols);
  for (int sub_index=0; sub_index!=n_subs; ++sub_index)
  {
    assert(sub_index < boost::numeric_cast<int>(m.size()));
    const boost::numeric::ublas::matrix<double>& sub = m[sub_index];
    const int offset_x = 0;
    const int offset_y = sub_index * sub_rows;
    for (int row=0; row!=sub_rows; ++row)
    {
      for (int col=0; col!=sub_cols; ++col)
      {
        assert(offset_y + row < boost::numeric_cast<int>(v.size1()));
        assert(offset_x + col < boost::numeric_cast<int>(v.size2()));
        assert(row < boost::numeric_cast<int>(sub.size1()));
        assert(col < boost::numeric_cast<int>(sub.size2()));
        v(offset_y + row,offset_x + col) = sub(row,col);
      }
    }
  }
  return v;
}


const boost::numeric::ublas::vector<double> ribi::Matrix::SimplifyVectorOfVector(
  const boost::numeric::ublas::vector<boost::numeric::ublas::vector<double> >& m)
{
  // [ [A] ]
  // [ [B] ]    [ A ]
  // [     ]    [ B ]
  // [ [C] ]    [ C ]
  // [ [D] ] -> [ D ]
  //assert(!m.empty());
  const int n_subs = boost::numeric_cast<int>(m.size());
  const int subs_sz = n_subs > 0 ? boost::numeric_cast<int>(m[0].size()) : 0;
  boost::numeric::ublas::vector<double> v(n_subs * subs_sz);
  for (int i=0; i!=n_subs; ++i)
  {
    assert(i < boost::numeric_cast<int>(m.size()));
    const boost::numeric::ublas::vector<double>& sub = m[i];
    std::copy(sub.begin(),sub.end(),v.begin() + (i * subs_sz));
  }
  return v;

}

const boost::numeric::ublas::matrix<double> ribi::Matrix::Unchop(
  const std::vector<boost::numeric::ublas::matrix<double> >& v)
{
  //Chop returns a std::vector of sub-matrices
  //[ A at [0]   B at [1] ]
  //[ C at [2]   D at [4] ]
  using boost::numeric::ublas::range;
  using boost::numeric::ublas::matrix;
  using boost::numeric::ublas::matrix_range;
  assert(v.size() == 4);
  assert(v[0].size1() == v[1].size1());
  assert(v[2].size1() == v[3].size1());
  assert(v[0].size2() == v[2].size2());
  assert(v[1].size2() == v[3].size2());
  boost::numeric::ublas::matrix<double> m(v[0].size1() + v[2].size1(),v[0].size2() + v[1].size2());
  for (int quadrant=0; quadrant!=4; ++quadrant)
  {
    const boost::numeric::ublas::matrix<double>& w = v[quadrant];
    const std::size_t n_rows = v[quadrant].size1();
    const std::size_t n_cols = v[quadrant].size2();
    const int offset_x = quadrant % 2 ? v[0].size2() : 0;
    const int offset_y = quadrant / 2 ? v[0].size1() : 0;
    for (std::size_t row=0; row!=n_rows; ++row)
    {
      for (std::size_t col=0; col!=n_cols; ++col)
      {
        m(offset_y + row, offset_x + col) = w(row,col);
      }
    }
  }

  assert(v[0].size1() + v[2].size1() == m.size1());
  assert(v[1].size1() + v[3].size1() == m.size1());
  assert(v[0].size2() + v[1].size2() == m.size2());
  assert(v[2].size2() + v[3].size2() == m.size2());

  return m;
}

bool ribi::Matrix::VectorIsAboutEqual(
  const boost::numeric::ublas::vector<double>& a,
  const boost::numeric::ublas::vector<double>& b) noexcept
{
  //TRACE("Deprecated naming");
  return VectorsAreAboutEqual(a,b);
}

bool ribi::Matrix::VectorsAreAboutEqual(
  const boost::numeric::ublas::vector<double>& a,
  const boost::numeric::ublas::vector<double>& b) noexcept
{
  if (a.size() != b.size()) return false;
  return std::equal(a.begin(),a.end(),b.begin(),&ribi::Matrix::IsAboutEqual);
}

bool ribi::Matrix::VectorsDoubleAreEqual(
  const boost::numeric::ublas::vector<double>& a,
  const boost::numeric::ublas::vector<double>& b) noexcept
{
  if (a.size() != b.size()) return false;
  return std::equal(a.begin(),a.end(),b.begin());
}

bool ribi::Matrix::VectorsIntAreEqual(
  const boost::numeric::ublas::vector<int>& a,
  const boost::numeric::ublas::vector<int>& b) noexcept
{
  if (a.size() != b.size()) return false;
  return std::equal(a.begin(),a.end(),b.begin());
}
