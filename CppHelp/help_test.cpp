#include "help.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(ribi_help_test)
{
  const std::string program_name{"program name"};
  const std::string program_description{"program description"};
  const std::vector<ribi::Help::Option> options{
    ribi::Help::Option('b',"abs","take absolute"),
    ribi::Help::Option('r',"rel","take relative")
  };
  const std::vector<std::string> example_uses{
   "./my_program -a",
   "./my_program --rel"
  };

  const ribi::Help help(
    program_name,
    program_description,
    options,
    example_uses
  );
  BOOST_CHECK(help.GetProgramName() == program_name);
  BOOST_CHECK(help.GetProgramDescription() == program_description);
  BOOST_CHECK(help.GetOptions() == ribi::Help::AddDefaultOptions(options));
  BOOST_CHECK(help.GetExampleUses() == example_uses);
}
