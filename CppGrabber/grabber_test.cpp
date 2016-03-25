#include "grabber.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(ribi_grabber_default_construction)
{
  BOOST_CHECK_NO_THROW(ribi::Grabber(0));
}

BOOST_AUTO_TEST_CASE(ribi_grabber_grab_screenshot)
{
  //Too bad, cannot
}
