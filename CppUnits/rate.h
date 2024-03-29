#ifndef RATE_H
#define RATE_H

//Rate
// The relative (unitless) change of something in time




#include <boost/units/io.hpp>
#include <boost/units/systems/si.hpp>
#include <boost/units/systems/si/prefixes.hpp>


namespace boost {
  namespace units {
    typedef derived_dimension<
      time_base_dimension,-1
    >::type rate_dimension;
  } // namespace units
} // namespace boost

namespace boost {
  namespace units {
    namespace si {
      typedef unit<rate_dimension,si::system> rate;
      BOOST_UNITS_STATIC_CONSTANT(per_second,rate);

      using Rate = boost::units::quantity<boost::units::si::rate>;
      std::istream& operator>>(std::istream& is,Rate& sd);

    } // namespace si
  } // namespace units
} //namespace boost

namespace ribi {
  namespace units {

    using Rate = boost::units::quantity<boost::units::si::rate>;

    #ifndef NDEBUG
    void TestRate() noexcept;
    #endif
  } //~namespace units
} //~namespace ribi

#endif // RATE_H
