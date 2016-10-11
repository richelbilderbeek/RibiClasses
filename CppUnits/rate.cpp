#include "rate.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/units/io.hpp>

#include "speciesdensity.h"

std::istream& boost::units::si::operator>>(std::istream& is, Rate& sd)
{
  double value = 0.0;
  is >> value;
  std::string unit;
  is >> unit;
  assert(unit == "s^-1");
  sd = Rate(
    value
    * boost::units::si::per_second);
  return is;
}
