#include "beast.h"

#include <cassert>
#include <chrono>

#include <QFile>


#include "fileio.h"
#include "ribi_rinside.h"

#include <boost/test/unit_test.hpp>

#include "RInside.h"

using namespace ribi;

BOOST_AUTO_TEST_CASE(beast_constructor)
{
  BOOST_CHECK_NO_THROW(Beast());
}

BOOST_AUTO_TEST_CASE(beast)
{
  Beast b;
  ribi::fileio::FileIo f;

  const std::string log_file_bd{"CppBeastLogFileBirthDeath.log"};
  const std::string log_file_coa{"CppBeastLogFileCoalescent.log"};
  const std::vector<std::string> resource_filenames
    =
    {
      log_file_bd, log_file_coa
    };
  //Create resources
  for (const auto& s: resource_filenames)
  {
    if (!f.IsRegularFile(s))
    {
      QFile((std::string(":/CppBeast/") + s).c_str()).copy(s.c_str());
    }
    BOOST_CHECK(f.IsRegularFile(s));
  }

  if (!"Speed comparison AnalyzeBirthDeath")
  {
    const std::string png_filename{f.GetTempFileName(".png")};

    const auto t2 = std::chrono::system_clock::now();
    b.AnalyzeBirthDeathRinside(
      log_file_bd,
      png_filename
    );
    const auto d2 = std::chrono::system_clock::now() - t2;

    const auto t1 = std::chrono::system_clock::now();
    b.AnalyzeBirthDeathRscript(
      log_file_bd,
      png_filename
    );

    const auto d1 = std::chrono::system_clock::now() - t1;
    BOOST_CHECK(d2 < d1 / 10);
  }

  if (!"Speed comparison AnalyzeCoalescent")
  {
    const std::string png_filename1{f.GetTempFileName(".png")};
    const std::string png_filename2{f.GetTempFileName(".png")};

    const auto t2 = std::chrono::system_clock::now();
    b.AnalyzeCoalescentRinside(
      log_file_coa,
      png_filename1,
      png_filename2
    );
    const auto d2 = std::chrono::system_clock::now() - t2;

    const auto t1 = std::chrono::system_clock::now();
    b.AnalyzeCoalescentRscript(
      log_file_coa,
      png_filename1,
      png_filename2
    );

    const auto d1 = std::chrono::system_clock::now() - t1;
    BOOST_CHECK(d2 < d1 / 10);
  }

  //Cleanup resources
  for (const auto& s: resource_filenames)
  {
    assert(f.IsRegularFile(s));
    f.DeleteFile(s);
    assert(!f.IsRegularFile(s));
  }
}
