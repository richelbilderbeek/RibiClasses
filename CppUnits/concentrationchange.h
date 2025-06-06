#ifndef CONCENTRATIONCHANGE_H
#define CONCENTRATIONCHANGE_H

//Species growth:
// The change in species density per time

#include "concentration.h"
#include <boost/units/systems/si.hpp>




#include <boost/units/io.hpp>
#include <boost/units/systems/si.hpp>
#include <boost/units/systems/si/prefixes.hpp>


namespace boost {
  namespace units {
    typedef derived_dimension<
      amount_base_dimension,1,
      length_base_dimension,-3,
      time_base_dimension,-1
    >::type concentration_change_dimension;
  } // namespace units
} // namespace boost

namespace boost {
  namespace units {
    namespace si {
      typedef unit<concentration_change_dimension,si::system> concentration_change;
      BOOST_UNITS_STATIC_CONSTANT(mole_per_cubic_meter_per_second,concentration_change);

      using ConcentrationChange = boost::units::quantity<boost::units::si::concentration_change>;
      std::istream& operator>>(std::istream& is,ConcentrationChange& sd);

    } // namespace si
  } // namespace units
} //namespace boost

namespace ribi {
  namespace units {

    using ConcentrationChange = boost::units::quantity<boost::units::si::concentration_change>;

    #ifndef NDEBUG
    void TestConcentrationChange() noexcept;
    #endif
  } //~namespace units
} //~namespace ribi


#endif // CONCENTRATIONCHANGE_H
