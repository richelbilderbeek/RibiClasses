#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "menudialog.h"
#include <cassert>
#include <iterator>
#include <iostream>

#pragma GCC diagnostic pop

std::vector<std::string> ribi::MenuDialog::ConvertArguments(
  const int argc, const char * const argv[]) noexcept
{
   std::vector<std::string> v;
   v.reserve(argc);
   for (int i=0; i!=argc; ++i)
   {
     v.push_back(std::string(argv[i]));
   }
   assert(argc == static_cast<int>(v.size()));
   return v;
}

int ribi::MenuDialog::Execute(
  const std::vector<std::string>& argv
) noexcept
{
  const int argc = static_cast<int>(argv.size());
  if (argc <= 1)
  {
    return ExecuteSpecific(argv);
  }
  const std::string s = argv[1];
  if (s == "--about" || s == "-a")
  {
    const std::vector<std::string> v { GetAbout().CreateAboutText() };
    std::copy(
      std::begin(v),
      std::end(v),
      std::ostream_iterator<std::string>(std::cout,"\n")
    );
    std::cout
      << "\n"
      << "Source code built on "
      << __DATE__
      << " "
      << __TIME__
      << " ("
    #ifdef NDEBUG
      << "release"
    #else
      << "debug"
    #endif
      << " version)"
      << '\n';
    return 0;
  }
  else if (s == "--help" || s == "-h")
  {
    std::cout << GetHelp() << '\n';
    return 0;
  }
  else if (s == "--history" || s == "-i")
  {
    const std::vector<std::string> v { GetVersionHistory() };
    std::copy(
      std::begin(v),
      std::end(v),
      std::ostream_iterator<std::string>(std::cout,"\n")
    );
    return 0;
  }
  else if (s == "--license" || s == "--licence" || s == "-l")
  {
    const std::vector<std::string> v { GetAbout().CreateLicenceText() };
    std::copy(
      std::begin(v),
      std::end(v),
      std::ostream_iterator<std::string>(std::cout,"\n")
    );
    return 0;
  }
  else if (s == "--version" || s == "-v")
  {
    const std::vector<std::string> v = { GetAbout().CreateLibrariesUsedText() };
    std::copy(
      std::begin(v),
      std::end(v),
      std::ostream_iterator<std::string>(std::cout,"\n")
    );
    return 0;
  }
  return ExecuteSpecific(argv);
}
