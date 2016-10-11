#include "ribi_time_unit.h"

#include <fstream>
#include <sstream>
#include <boost/units/io.hpp>
#include "fileio.h"


// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace ribi::units;

BOOST_AUTO_TEST_CASE(ribi_units_ribi_time_unit)
{
  using ribi::fileio::FileIo;
  using boost::units::si::seconds;

  //Time is in seconds
  {
    const Time d{1.0 * seconds};
    std::stringstream s;
    s << d;
    const std::string t{s.str()};
    BOOST_CHECK(t.substr(t.size() - 1,1) == "s");
  }
  //Time uses a dot as a seperator
  {
    const Time d{12.34 * seconds};
    std::stringstream s;
    s << d;
    const std::string t{s.str()};
    BOOST_CHECK(t == "12.34 s");
  }
  //Time file I/O, one Time
  {
    const Time d{12.34 * seconds};
    const std::string filename{FileIo().GetTempFileName(".txt")};
    {
      std::ofstream f{filename};
      f << d;
    }
    std::ifstream f{filename};
    Time d_too;
    f >> d_too;
    if (d != d_too)
    {
      std::cerr << d << '\n'
        << d_too << '\n'
      ;
    }
    BOOST_CHECK(d == d_too);
  }
  //Time file I/O, two Time
  {
    const Time a{12.34 * seconds};
    const Time b{23.45 * seconds};
    const std::string filename{FileIo().GetTempFileName(".txt")};
    {
      std::ofstream f{filename};
      f << a << " " << b;
    }
    std::ifstream f{filename};
    Time a_too;
    Time b_too;
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
}

#pragma GCC diagnostic pop
