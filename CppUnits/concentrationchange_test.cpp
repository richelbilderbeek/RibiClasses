#include "concentrationchange.h"

#include <fstream>
#include <boost/units/io.hpp>
#include "concentration.h"
#include "fileio.h"
#include "rate.h"

using namespace ribi::units;

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(ribi_units_concentrationchange)
{
  using ribi::fileio::FileIo;
  using ConcentrationChange = boost::units::quantity<boost::units::si::concentration_change>;
  //Concentration density is in species per square meter
  {
    const ConcentrationChange d{
      1.0 * boost::units::si::mole_per_cubic_meter_per_second};
    std::stringstream s;
    s << d;
    const std::string t{s.str()};
    //std::cerr << t << std::endl;
    BOOST_CHECK(t.substr(t.size() - 13,13) == "m^-3 mol s^-1");
  }
  //Concentration density uses a dot as a seperator
  {
    const ConcentrationChange d{12.34 * boost::units::si::mole_per_cubic_meter_per_second};
    std::stringstream s;
    s << d;
    const std::string t{s.str()};
    BOOST_CHECK(t == "12.34 m^-3 mol s^-1");
  }
  //Concentration growth can be multiplied by an area to obtain the growth rate
  {
    using Area = boost::units::quantity<boost::units::si::area>;
    const ConcentrationChange d{0.1 * boost::units::si::mole_per_cubic_meter_per_second};
    const Area a(123.4 * boost::units::si::square_meter);
    const auto n = d * a;
    std::stringstream s;
    s << n;
    const std::string t{s.str()};
    BOOST_CHECK(t.substr(t.size() - 4, 4) == "s^-1");
  }
  //Concentration density file I/O, one ConcentrationChange
  {
    const ConcentrationChange d{12.34 * boost::units::si::mole_per_cubic_meter_per_second};
    const std::string filename{FileIo().GetTempFileName(".txt")};
    {
      std::ofstream f{filename};
      f << d;
    }
    std::ifstream f{filename};
    ConcentrationChange d_too;
    f >> d_too;
    if (d != d_too)
    {
      std::cerr << d << '\n'
        << d_too << '\n'
      ;
    }
    BOOST_CHECK(d == d_too);
  }
  //Concentration density file I/O, two ConcentrationChange
  {
    const ConcentrationChange a{12.34 * boost::units::si::mole_per_cubic_meter_per_second};
    const ConcentrationChange b{23.45 * boost::units::si::mole_per_cubic_meter_per_second};
    const std::string filename{FileIo().GetTempFileName(".txt")};
    {
      std::ofstream f{filename};
      f << a << " " << b;
    }
    std::ifstream f{filename};
    ConcentrationChange a_too;
    ConcentrationChange b_too;
    f >> a_too >> b_too;
    if (a != a_too)
    {
      std::cerr << a << '\n'
        << a_too << '\n'
      ;
    }
    BOOST_CHECK(a == a_too);
    if (b != b_too)
    {
      std::cerr << b << '\n'
        << b_too << '\n'
      ;
    }
    BOOST_CHECK(b == b_too);
  }
  //Combining ConcentrationDensity, ConcentrationChange and Rate
  //Exponential growth
  {
    const double n_unitless{0.7};
    const double r_unitless{1.3};
    const Concentration n{n_unitless * boost::units::si::mole_per_cubic_meter};
    const Rate r{r_unitless * boost::units::si::per_second};
    const ConcentrationChange dndt{n * r};
    BOOST_CHECK(dndt.value() == n_unitless * r_unitless);
  }
  //Logistic growth
  {
    const double n_unitless{0.7};
    const double k_unitless{2.0};
    const double r_unitless{1.3};
    const Concentration n{n_unitless * boost::units::si::mole_per_cubic_meter};
    const Concentration k{k_unitless * boost::units::si::mole_per_cubic_meter};
    const Rate r{r_unitless * boost::units::si::per_second};
    const ConcentrationChange dndt{ n * r * (1.0 - (n / k))};
    BOOST_CHECK(dndt.value() == n_unitless * r_unitless * (1.0 - (n_unitless / k_unitless)));
  }
}

#pragma GCC diagnostic pop
