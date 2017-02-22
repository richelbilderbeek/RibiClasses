#include "multivector.h"

#ifndef NDEBUG
void ribi::TestMultiVector() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  {

  }
  ribi::MultiVector<int> x;
  //1 dimensional
  x.Store( { 0 }, 0);
  x.Store( { 1 }, 1);
  x.Store( { 2 }, 2);
  x.Store( { 3 }, 3);
  x.Store( { 4 }, 4);
  assert(x.Retrieve( { 0 } ) ==  0);
  assert(x.Retrieve( { 1 } ) ==  1);
  assert(x.Retrieve( { 2 } ) ==  2);
  assert(x.Retrieve( { 3 } ) ==  3);
  assert(x.Retrieve( { 4 } ) ==  4);
  //2 dimensional;
  x.Store( { 0,0 },  0);
  x.Store( { 1,1 },  1);
  x.Store( { 2,2 },  4);
  x.Store( { 3,3 },  9);
  x.Store( { 4,4 }, 16);
  assert(x.Retrieve( { 0,0 } ) ==  0);
  assert(x.Retrieve( { 1,1 } ) ==  1);
  assert(x.Retrieve( { 2,2 } ) ==  4);
  assert(x.Retrieve( { 3,3 } ) ==  9);
  assert(x.Retrieve( { 4,4 } ) == 16);
  //3 dimensional;
  x.Store( { 0,0,0 },  0);
  x.Store( { 1,1,1 },  1);
  x.Store( { 2,2,2 },  8);
  x.Store( { 3,3,3 }, 27);
  x.Store( { 4,4,4 }, 54);
  assert(x.Retrieve( { 0,0,0 } ) ==   0);
  assert(x.Retrieve( { 1,1,1 } ) ==   1);
  assert(x.Retrieve( { 2,2,2 } ) ==   8);
  assert(x.Retrieve( { 3,3,3 } ) ==  27);
  assert(x.Retrieve( { 4,4,4 } ) ==  54);
  //4 dimensional;
  x.Store( { 0,0,0,0 },   0);
  x.Store( { 1,1,1,1 },   1);
  x.Store( { 2,2,2,2 },  16);
  x.Store( { 3,3,3,3 },  81);
  x.Store( { 4,4,4,4 }, 216);
  assert(x.Retrieve( { 0,0,0,0 } ) ==   0);
  assert(x.Retrieve( { 1,1,1,1 } ) ==   1);
  assert(x.Retrieve( { 2,2,2,2 } ) ==  16);
  assert(x.Retrieve( { 3,3,3,3 } ) ==  81);
  assert(x.Retrieve( { 4,4,4,4 } ) == 216);
  //5 dimensional;
  x.Store( { 0,0,0,0,0 },   0);
  x.Store( { 1,1,1,1,1 },   1);
  x.Store( { 2,2,2,2,2 },  32);
  x.Store( { 3,3,3,3,3 }, 243);
  x.Store( { 4,4,4,4,4 }, 864);
  assert(x.Retrieve( { 0,0,0,0,0 } ) ==    0);
  assert(x.Retrieve( { 1,1,1,1,1 } ) ==    1);
  assert(x.Retrieve( { 2,2,2,2,2 } ) ==   32);
  assert(x.Retrieve( { 3,3,3,3,3 } ) ==  243);
  assert(x.Retrieve( { 4,4,4,4,4 } ) ==  864);

}
#endif
