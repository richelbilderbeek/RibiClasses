#ifndef NDEBUG
#include "massflow.h"

#include <iostream>
#include <sstream>

#include <boost/units/io.hpp>

#include "volumetricflow.h"

void ribi::units::TestMassFlow() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  using MassFlow = boost::units::quantity<boost::units::si::mass_flow>;
  using Mass = boost::units::quantity<boost::units::si::mass>;
  using Time = boost::units::quantity<boost::units::si::time>;
  //MassFlow has unit mass per second, 'kg s^-1'
  {
    const MassFlow phi_m{
        1.0
      * boost::units::si::kilogram / boost::units::si::second
    };
    std::stringstream s;
    s << phi_m;
    const std::string t{s.str()};
    assert(t.substr(t.size() - 7,7) == "kg s^-1");
  }
  //MassFlow can be obtained by dividing a mass by a time unit
  {
    const Mass mass{1.0 * boost::units::si::kilogram};
    const Time time{1.0 * boost::units::si::second};
    const MassFlow phi_m{mass / time};
  }
  //MassFlow can be converted to VolumetricFlow
  {
    using Volume = boost::units::quantity<boost::units::si::volume>;
    using VolumetricFlow = boost::units::quantity<boost::units::si::volumetric_flow>;
    using Density = boost::units::quantity<boost::units::si::mass_density>;

    const Volume v{1.0 * boost::units::si::cubic_meter};
    const Time t{1.0 * boost::units::si::second};
    const MassFlow phi_m{
      1.0 * boost::units::si::kilogram / boost::units::si::second
    };
    const Density d{
      1.0 * boost::units::si::kilogram / boost::units::si::cubic_meter
    };
    const VolumetricFlow phi_v{phi_m / d};
    //const VolumetricFlow phi_v_wrong{phi_m * d}; //GOOD: does not compile
    //const VolumetricFlow phi_v_wrong{d / phi_m}; //GOOD: does not compile
  }
}
#endif

