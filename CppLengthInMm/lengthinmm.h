#ifndef RIBI_LENGTHINMM_H
#define RIBI_LENGTHINMM_H





#include <boost/units/quantity.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/systems/si/prefixes.hpp>


namespace ribi {

typedef boost::units::scaled_base_unit<
  boost::units::si::meter_base_unit,
  boost::units::scale<10, boost::units::static_rational<-3>>
> millimeter_base_unit;

typedef boost::units::make_system<millimeter_base_unit>::type mm_system;
typedef boost::units::unit<boost::units::length_dimension,mm_system> millimeter_unit;
BOOST_UNITS_STATIC_CONSTANT(millimeters, millimeter_unit);
typedef boost::units::quantity<millimeter_unit> LengthInMm;

} //~namespace ribi

#endif // RIBI_LENGTHINMM_H
