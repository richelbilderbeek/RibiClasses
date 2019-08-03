#pragma GCC diagnostic push


#include "matrix.h"

#ifndef NDEBUG
void ribi::Matrix::Test() noexcept
{
  using boost::numeric::ublas::detail::equals;
  using boost::numeric::ublas::matrix;
  using boost::numeric::ublas::vector;
  //Test CreateMatrix
  {
    // [1,4]
    // [2,5]
    // [3,6]
    const matrix<int> m = CreateMatrix(3,2, {1,2,3,4,5,6} );
    assert(m(0,0) == 1);
    assert(m(1,0) == 2);
    assert(m(2,0) == 3);
    assert(m(0,1) == 4);
    assert(m(1,1) == 5);
    assert(m(2,1) == 6);
  }
  //Simplify vector of vector
  {
    /// [ [A] ]
    /// [ [B] ]    [ A ]
    /// [     ]    [ B ]
    /// [ [C] ]    [ C ]
    /// [ [D] ] -> [ D ]
    boost::numeric::ublas::vector<boost::numeric::ublas::vector<double> > v(2);
    v[0] = CreateVector( { 1.0, 2.0 } );
    v[1] = CreateVector( { 3.0, 4.0 } );
    const boost::numeric::ublas::vector<double> w
      = SimplifyVectorOfVector(v);
    assert(w.size() == 4);
    assert(VectorsAreAboutEqual(w, CreateVector( { 1.0, 2.0, 3.0, 4.0 } )));
  }
  {
    ///Simplify a structure
    /// [ [A B] ]
    /// [ [C D] ]    [ A B ]
    /// [       ]    [ C D ]
    /// [ [E F] ]    [ E F ]
    /// [ [G H] ] -> [ G H ]
    boost::numeric::ublas::vector<boost::numeric::ublas::matrix<double> > v(2);
    v[0] = CreateMatrix(2,2, {1.0, 3.0, 2.0, 4.0} );
    v[1] = CreateMatrix(2,2, {5.0, 7.0, 6.0, 8.0} );
    const boost::numeric::ublas::matrix<double> w
      = SimplifyVectorOfMatrix(v);
    assert(w.size1() == 4);
    assert(w.size2() == 2);
    assert(MatricesAreAboutEqual(w, CreateMatrix(4,2, { 1.0, 3.0, 5.0, 7.0, 2.0, 4.0, 6.0, 8.0 } )));
  }
  {
    //Simplify a structure
    // [ [A B]  [I J] ]
    // [ [C D]  [K L] ]    [ A B I J]
    // [              ]    [ C D K L]
    // [ [E F]  [M N] ]    [ E F M N]
    // [ [G H]  [O P] ] -> [ G H O P]

    // [ [ 1  2]  [ 3  4] ]
    // [ [ 5  6]  [ 7  8] ]    [ A B I J]
    // [                  ]    [ C D K L]
    // [ [ 9 10]  [11 12] ]    [ E F M N]
    // [ [13 14]  [15 16] ] -> [ G H O P]
    boost::numeric::ublas::matrix<boost::numeric::ublas::matrix<double> > v(2,2);
    v(0,0) = CreateMatrix(2,2, { 1.0,  5.0,  2.0,  6.0} );
    v(1,0) = CreateMatrix(2,2, { 9.0, 13.0, 10.0, 14.0} );
    v(0,1) = CreateMatrix(2,2, { 3.0,  7.0,  4.0,  8.0} );
    v(1,1) = CreateMatrix(2,2, {11.0, 15.0, 12.0, 16.0} );
    const boost::numeric::ublas::matrix<double> w
      = SimplifyMatrixOfMatrix(v);
    assert(w.size1() == 4);
    assert(w.size2() == 4);
    const boost::numeric::ublas::matrix<double> expected
      = CreateMatrix(4,4,
      {
        1.0, 5.0,  9.0, 13.0,
        2.0, 6.0, 10.0, 14.0,
        3.0, 7.0, 11.0, 15.0,
        4.0, 8.0, 12.0, 16.0
      } );
    if (!MatricesAreAboutEqual(w,expected))
    {
      std::cerr
        << "w: " << w << '\n'
        << "e: " << expected << '\n';
    }
    assert(MatricesAreAboutEqual(w,expected));
  }

  //Test Chop on 3x3
  {
    //                     [ 1.0 ] | [ 2.0   3.0 ]
    // [ 1.0 2.0 3.0 ]     --------+--------------
    // [ 4.0 5.0 6.0 ]     [ 4.0 ] | [ 5.0   6.0 ]
    // [ 7.0 8.0 9.0 ] ->  [ 7.0 ] | [ 8.0   9.0 ]
    const matrix<double> m = CreateMatrix(3,3, {1.0,4.0,7.0,2.0,5.0,8.0,3.0,6.0,9.0} );
    assert(m(0,0) == 1.0); assert(m(0,1) == 2.0); assert(m(0,2) == 3.0);
    assert(m(1,0) == 4.0); assert(m(1,1) == 5.0); assert(m(1,2) == 6.0);
    assert(m(2,0) == 7.0); assert(m(2,1) == 8.0); assert(m(2,2) == 9.0);
    const std::vector<matrix<double> > n = Chop(m);
    assert(n.size() == 4);
    assert(n[0].size1() == 1);
    assert(n[0].size2() == 1);
    assert(n[1].size1() == 1);
    assert(n[1].size2() == 2);
    assert(n[2].size1() == 2);
    assert(n[2].size2() == 1);
    assert(n[3].size1() == 2);
    assert(n[3].size2() == 2);
    assert(n[0].size1() + n[2].size1() == m.size1());
    assert(n[1].size1() + n[3].size1() == m.size1());
    assert(n[0].size2() + n[1].size2() == m.size2());
    assert(n[2].size2() + n[3].size2() == m.size2());
  }
  //Test Chop on 5x5
  {
    const matrix<double> m = CreateMatrix(5,5,
      {
        1.0, 6.0,11.0,16.0,21.0,
        2.0, 7.0,12.0,17.0,22.0,
        3.0, 8.0,13.0,18.0,23.0,
        4.0, 9.0,14.0,19.0,24.0,
        5.0,10.0,15.0,20.0,25.0
      }
    );
    assert(m(0,0) ==  1.0); assert(m(0,1) ==  2.0); assert(m(0,2) ==  3.0); assert(m(0,3) ==  4.0); assert(m(0,4) ==  5.0);
    assert(m(1,0) ==  6.0); assert(m(1,1) ==  7.0); assert(m(1,2) ==  8.0); assert(m(1,3) ==  9.0); assert(m(1,4) == 10.0);
    assert(m(2,0) == 11.0); assert(m(2,1) == 12.0); assert(m(2,2) == 13.0); assert(m(2,3) == 14.0); assert(m(2,4) == 15.0);
    assert(m(3,0) == 16.0); assert(m(3,1) == 17.0); assert(m(3,2) == 18.0); assert(m(3,3) == 19.0); assert(m(3,4) == 20.0);
    assert(m(4,0) == 21.0); assert(m(4,1) == 22.0); assert(m(4,2) == 23.0); assert(m(4,3) == 24.0); assert(m(4,4) == 25.0);
    const std::vector<matrix<double> > n = Chop(m);
    assert(n.size() == 4);
    assert(n[0].size1() == 2);
    assert(n[0].size2() == 2);
    assert(n[1].size1() == 2);
    assert(n[1].size2() == 3);
    assert(n[2].size1() == 3);
    assert(n[2].size2() == 2);
    assert(n[3].size1() == 3);
    assert(n[3].size2() == 3);
    assert(n[0].size1() + n[2].size1() == m.size1());
    assert(n[1].size1() + n[3].size1() == m.size1());
    assert(n[0].size2() + n[1].size2() == m.size2());
    assert(n[2].size2() + n[3].size2() == m.size2());
  }
  //Test VectorsAreAboutEqual
  {
    const vector<double> m = CreateVector( {1.0,3.0,2.0,4.0} );
    assert(VectorsAreAboutEqual(m,m));
    vector<double> n(m);
    n *= 3.0;
    n /= 6.0;
    n *= 2.0;
    assert(VectorsAreAboutEqual(m,n));
  }
  //Test Power
  {
    const int sz = 2;
    const matrix<double> m = CreateMatrix(sz,sz, {1.0,3.0,2.0,4.0} );
    const matrix<double> expected_0 = boost::numeric::ublas::identity_matrix<double>(sz);
    const matrix<double> expected_1 = m;
    const matrix<double> expected_2 = CreateMatrix(sz,sz, {7.0,15.0,10.0,22.0} );
    assert(ribi::Matrix::MatricesAreAboutEqual(m,m));
    assert(ribi::Matrix::MatricesAreAboutEqual(Power(m,0),expected_0));
    assert(ribi::Matrix::MatricesAreAboutEqual(Power(m,1),expected_1));
    assert(ribi::Matrix::MatricesAreAboutEqual(Power(m,2),expected_2));
  }
  //Test Unchop
  {
    //Check 0x0 to and including 9x9 matrices
    for (std::size_t n_rows = 0; n_rows!=10; ++n_rows)
    {
      for (std::size_t n_cols = 0; n_cols!=10; ++n_cols)
      {
        //Epsilon is more or less the smallest round-off error
        const double epsilon = std::numeric_limits<double>::epsilon();

        //Create a random matrix
        const matrix<double> m = ribi::Matrix::CreateRandomMatrix(n_rows,n_cols);

        //Assume it is found identical to itself
        assert(equals(m,m,epsilon,epsilon));

        //Chop and unchop the input matrix
        const matrix<double> n = ribi::Matrix::Unchop(Chop(m));

        //Assume input matrix and result are identical
        assert(equals(m,n,epsilon,epsilon));
      }
    }
  }
  //Test Inverse on 2x2 matrix
  {
    // [ 1.0 2.0 ] -1    [ -2.0   1.0 ]
    // [ 3.0 4.0 ]     = [  1.5  -0.5 ]
    const matrix<double> m = ribi::Matrix::CreateMatrix(2,2, {1.0,3.0,2.0,4.0} );
    assert(m(0,0) == 1.0);
    assert(m(1,0) == 3.0);
    assert(m(0,1) == 2.0);
    assert(m(1,1) == 4.0);
    const matrix<double> n = ribi::Matrix::Inverse(m);
    const double epsilon = 0.0000001; //Rounding error
    assert(n(0,0) > -2.0 - epsilon && n(0,0) < -2.0 + epsilon);
    assert(n(1,0) >  1.5 - epsilon && n(1,0) <  1.5 + epsilon);
    assert(n(0,1) >  1.0 - epsilon && n(0,1) <  1.0 + epsilon);
    assert(n(1,1) > -0.5 - epsilon && n(1,1) < -0.5 + epsilon);
    assert(ribi::Matrix::Prod(m,n)(0,0) > 1.0 - epsilon && ribi::Matrix::Prod(m,n)(0,0) < 1.0 + epsilon);
    assert(ribi::Matrix::Prod(m,n)(1,0) > 0.0 - epsilon && ribi::Matrix::Prod(m,n)(1,0) < 0.0 + epsilon);
    assert(ribi::Matrix::Prod(m,n)(0,1) > 0.0 - epsilon && ribi::Matrix::Prod(m,n)(0,1) < 0.0 + epsilon);
    assert(ribi::Matrix::Prod(m,n)(1,1) > 1.0 - epsilon && ribi::Matrix::Prod(m,n)(1,1) < 1.0 + epsilon);
  }


  {
    // [ 1.0 2.0 3.0] -1    [ -24.0   18.0   5.0]
    // [ 0.0 1.0 4.0]       [  20.0  -15.0  -4.0]
    // [ 5.0 6.0 0.0]     = [ - 5.0    4.0   1.0]
    const matrix<double> m = ribi::Matrix::CreateMatrix(3,3, {1.0,0.0,5.0,2.0,1.0,6.0,3.0,4.0,0.0} );
    assert(m(0,0) == 1.0); assert(m(0,1) == 2.0); assert(m(0,2) == 3.0);
    assert(m(1,0) == 0.0); assert(m(1,1) == 1.0); assert(m(1,2) == 4.0);
    assert(m(2,0) == 5.0); assert(m(2,1) == 6.0); assert(m(2,2) == 0.0);
    const matrix<double> n = ribi::Matrix::Inverse(m);
    const double epsilon = 0.0001; //Rounding error
    assert(n(0,0) > -24.0 - epsilon && n(0,0) < -24.0 + epsilon);
    assert(n(1,0) >  20.0 - epsilon && n(1,0) <  20.0 + epsilon);
    assert(n(2,0) > - 5.0 - epsilon && n(2,0) < - 5.0 + epsilon);
    assert(n(0,1) >  18.0 - epsilon && n(0,1) <  18.0 + epsilon);
    assert(n(1,1) > -15.0 - epsilon && n(1,1) < -15.0 + epsilon);
    assert(n(2,1) >   4.0 - epsilon && n(2,1) <   4.0 + epsilon);
    assert(n(0,2) >   5.0 - epsilon && n(0,2) <   5.0 + epsilon);
    assert(n(1,2) >  -4.0 - epsilon && n(1,2) < - 4.0 + epsilon);
    assert(n(2,2) >   1.0 - epsilon && n(2,2) <   1.0 + epsilon);
    const matrix<double> i = ribi::Matrix::Prod(m,n);
    assert(i(0,0) > 1.0 - epsilon && i(0,0) < 1.0 + epsilon);
    assert(i(1,0) > 0.0 - epsilon && i(1,0) < 0.0 + epsilon);
    assert(i(2,0) > 0.0 - epsilon && i(2,0) < 0.0 + epsilon);
    assert(i(0,1) > 0.0 - epsilon && i(0,1) < 0.0 + epsilon);
    assert(i(1,1) > 1.0 - epsilon && i(1,1) < 1.0 + epsilon);
    assert(i(2,1) > 0.0 - epsilon && i(2,1) < 0.0 + epsilon);
    assert(i(0,2) > 0.0 - epsilon && i(0,2) < 0.0 + epsilon);
    assert(i(1,2) > 0.0 - epsilon && i(1,2) < 0.0 + epsilon);
    assert(i(2,2) > 1.0 - epsilon && i(2,2) < 1.0 + epsilon);
  }
  //Test Inverse on 3x3 matrix
  {
    // [ 1.0 2.0 3.0] -1
    // [ 4.0 4.0 6.0]
    // [ 7.0 8.0 9.0]
    // Note: cannot make the center value equal to 5.0, as this makes
    // the matrix un-invertible (the determinant becomes equal to zero)
    const matrix<double> m = ribi::Matrix::CreateMatrix(3,3, {1.0,4.0,7.0,2.0,4.0,8.0,3.0,6.0,9.0} );
    assert(m(0,0) == 1.0); assert(m(0,1) == 2.0); assert(m(0,2) == 3.0);
    assert(m(1,0) == 4.0); assert(m(1,1) == 4.0); assert(m(1,2) == 6.0);
    assert(m(2,0) == 7.0); assert(m(2,1) == 8.0); assert(m(2,2) == 9.0);
    const matrix<double> n = ribi::Matrix::Inverse(m);
    const double epsilon = 0.00001; //Rounding error
    const matrix<double> i = ribi::Matrix::Prod(m,n);
    assert(i(0,0) > 1.0 - epsilon && i(0,0) < 1.0 + epsilon);
    assert(i(1,0) > 0.0 - epsilon && i(1,0) < 0.0 + epsilon);
    assert(i(2,0) > 0.0 - epsilon && i(2,0) < 0.0 + epsilon);
    assert(i(0,1) > 0.0 - epsilon && i(0,1) < 0.0 + epsilon);
    assert(i(1,1) > 1.0 - epsilon && i(1,1) < 1.0 + epsilon);
    assert(i(2,1) > 0.0 - epsilon && i(2,1) < 0.0 + epsilon);
    assert(i(0,2) > 0.0 - epsilon && i(0,2) < 0.0 + epsilon);
    assert(i(1,2) > 0.0 - epsilon && i(1,2) < 0.0 + epsilon);
    assert(i(2,2) > 1.0 - epsilon && i(2,2) < 1.0 + epsilon);
  }
  //Test Inverse on 4x4 matrix
  {
    const matrix<double> m = ribi::Matrix::CreateRandomMatrix(4,4);
    const matrix<double> n = ribi::Matrix::Inverse(m);
    const double epsilon = 0.00001; //Rounding error
    const matrix<double> i = ribi::Matrix::Prod(m,n);
    //Test if i is identity matrix
    assert(i(0,0) > 1.0 - epsilon && i(0,0) < 1.0 + epsilon);
    assert(i(1,0) > 0.0 - epsilon && i(1,0) < 0.0 + epsilon);
    assert(i(2,0) > 0.0 - epsilon && i(2,0) < 0.0 + epsilon);
    assert(i(3,0) > 0.0 - epsilon && i(3,0) < 0.0 + epsilon);
    assert(i(0,1) > 0.0 - epsilon && i(0,1) < 0.0 + epsilon);
    assert(i(1,1) > 1.0 - epsilon && i(1,1) < 1.0 + epsilon);
    assert(i(2,1) > 0.0 - epsilon && i(2,1) < 0.0 + epsilon);
    assert(i(3,1) > 0.0 - epsilon && i(3,1) < 0.0 + epsilon);
    assert(i(0,2) > 0.0 - epsilon && i(0,2) < 0.0 + epsilon);
    assert(i(1,2) > 0.0 - epsilon && i(1,2) < 0.0 + epsilon);
    assert(i(2,2) > 1.0 - epsilon && i(2,2) < 1.0 + epsilon);
    assert(i(3,2) > 0.0 - epsilon && i(3,2) < 0.0 + epsilon);
    assert(i(0,3) > 0.0 - epsilon && i(0,3) < 0.0 + epsilon);
    assert(i(1,3) > 0.0 - epsilon && i(1,3) < 0.0 + epsilon);
    assert(i(2,3) > 0.0 - epsilon && i(2,3) < 0.0 + epsilon);
    assert(i(3,3) > 1.0 - epsilon && i(3,3) < 1.0 + epsilon);
  }
  //TRACE("Test Inverse on bigger matrices");
  for (std::size_t sz = 5; sz!=20; ++sz)
  {
    const matrix<double> m = ribi::Matrix::CreateRandomMatrix(sz,sz);
    const matrix<double> n = ribi::Matrix::Inverse(m);
    const double epsilon = 0.00001; //Rounding error
    const matrix<double> i = ribi::Matrix::Prod(m,n);
    //Test if i is identity matrix
    for (std::size_t y = 0; y!=sz; ++y)
    {
      for (std::size_t x = 0; x!=sz; ++x)
      {
        assert(
             (x == y && i(y,x) > 1.0 - epsilon && i(y,x) < 1.0 + epsilon)
          || (x != y && i(y,x) > 0.0 - epsilon && i(y,x) < 0.0 + epsilon)
        );
      }
    }
  }
  //TRACE("Test MatricesAreEqual");
  {
    {
      const auto a = boost::numeric::ublas::zero_matrix<double>(2,3);
      const auto b = boost::numeric::ublas::zero_matrix<double>(3,2);
      const auto c = boost::numeric::ublas::zero_matrix<double>(2,2);
      const auto d = boost::numeric::ublas::zero_matrix<double>(3,3);
      assert( MatricesAreEqual(a,a));
      assert( MatricesAreEqual(b,b));
      assert( MatricesAreEqual(c,c));
      assert( MatricesAreEqual(d,d));
      assert(!MatricesAreEqual(a,b));
      assert(!MatricesAreEqual(a,c));
      assert(!MatricesAreEqual(a,d));
      assert(!MatricesAreEqual(b,c));
      assert(!MatricesAreEqual(b,d));
      assert(!MatricesAreEqual(c,d));
    }
    {
      const auto a = CreateMatrix(2,2, { 1.0,0.0,0.0,1.0 } );
      auto b = a;
      assert(MatricesAreEqual(a,b));
      assert(MatricesAreAboutEqual(a,b));
      b(1,1) = 0.0;
      assert(!MatricesAreEqual(a,b));
      assert(!MatricesAreAboutEqual(a,b));
      b(1,1) = 1.0;
      assert(MatricesAreEqual(a,b));
      assert(MatricesAreAboutEqual(a,b));
    }
  }
  //TRACE("Test VectorsAreEqual (int)");
  {
    {
      const auto a = boost::numeric::ublas::vector<int>(2,0);
      const auto b = boost::numeric::ublas::vector<int>(3,0);
      assert( VectorsIntAreEqual(a,a));
      assert( VectorsIntAreEqual(b,b));
      assert(!VectorsIntAreEqual(a,b));
      assert(!VectorsIntAreEqual(b,a));
    }
    {
      const auto a = CreateVector( { 1,2,3 } );
      auto b = a;
      assert(VectorsIntAreEqual(a,a));
      assert(VectorsIntAreEqual(a,b));
      assert(VectorsIntAreEqual(b,a));
      assert(VectorsIntAreEqual(b,b));
      b(1) = 0;
      assert( VectorsIntAreEqual(a,a));
      assert(!VectorsIntAreEqual(a,b));
      assert(!VectorsIntAreEqual(b,a));
      assert( VectorsIntAreEqual(b,b));
      b(1) = 2;
      assert(VectorsIntAreEqual(a,a));
      assert(VectorsIntAreEqual(a,b));
      assert(VectorsIntAreEqual(b,a));
      assert(VectorsIntAreEqual(b,b));
    }
  }
  //TRACE("Test VectorsAreEqual (double)");
  {
    {
      const auto a = boost::numeric::ublas::zero_vector<double>(2);
      const auto b = boost::numeric::ublas::zero_vector<double>(3);
      assert( VectorsDoubleAreEqual(a,a));
      assert( VectorsDoubleAreEqual(b,b));
      assert(!VectorsDoubleAreEqual(a,b));
      assert(!VectorsDoubleAreEqual(b,a));
    }
    {
      const auto a = CreateVector( { 1.1,2.2,3.3 } );
      auto b = a;
      assert(VectorsDoubleAreEqual(a,b));
      assert(VectorsAreAboutEqual(a,b));
      b(1) = 0.0;
      assert(!VectorsDoubleAreEqual(a,b));
      assert(!VectorsAreAboutEqual(a,b));
      b(1) = 2.2;
      assert(VectorsDoubleAreEqual(a,b));
      assert(VectorsAreAboutEqual(a,b));
    }
  }
}
#endif
