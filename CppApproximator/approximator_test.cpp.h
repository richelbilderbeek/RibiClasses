#include "approximator.h"

void Approximator_Test() noexcept
{
  Approximator<double,int> m;
  m.Add(1.0,10);
  m.Add(2.0,20);
  assert(m.Approximate(1.5) == 15);
  m.Add(4.0,40);
  assert(m.Approximate(3.0) == 30);
  m.Add(3.0,35);
  assert(m.Approximate(3.0) == 35);
  assert(m.GetMin() == 1.0);
  assert(m.GetMax() == 4.0);
}
