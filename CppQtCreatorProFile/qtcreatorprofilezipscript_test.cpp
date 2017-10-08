#include "qtcreatorprofilezipscript.h"

#include <fstream>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include "fileio.h"
using namespace ribi;

BOOST_AUTO_TEST_CASE(ribi_qtcreatorprofilezipscript_basic)
{
  //Test basic functions on this project with going two folders down
  const std::vector<std::string> pro_file_names
    =
    {
      "../../Tools/ToolCreateQtProjectZipFile/ToolCreateQtProjectZipFileDesktop.pro",
      "../../Tools/ToolCodeToHtml/ToolCodeToHtmlDesktop.pro",
      "../../Tools/ToolKalmanFilterer/ToolKalmanFiltererDesktop.pro",
      "../../ProjectRichelBilderbeek/ProjectRichelBilderbeek/ProjectRichelBilderbeekDesktop.pro"
    };
  for (const std::string& pro_file_name: pro_file_names)
  {
    if (!ribi::fileio::FileIo().IsRegularFile(pro_file_name)) continue;

    assert(ribi::fileio::FileIo().IsRegularFile(pro_file_name));
    const boost::shared_ptr<const QtCreatorProFile> pro_file(
      new QtCreatorProFile(pro_file_name) );
    assert(pro_file);

    const boost::shared_ptr<const QtCreatorProFileZipScript> zip_script(
      new QtCreatorProFileZipScript(pro_file) );
    assert(zip_script);

    const std::set<std::string> filenames = zip_script->GetFilenames();
    for (const std::string& filename: filenames)
    {
      if (!ribi::fileio::FileIo().IsRegularFile(filename))
      {
        const std::string warning = "Warning: file \'" + filename + "\' not found";
        std::clog << warning;
        continue;
      }
      assert(ribi::fileio::FileIo().IsRegularFile(filename));
    }
  }
  //Test that GetProFilesInFolder detects an additional .pro file
  //being added to a folder
  {
    const std::string tmp_pro_filename { fileio::FileIo().GetTempFileName(".pro") };

    //Count the current number of .pro files
    const std::size_t n = GetProAndPriFilesInFolder("").size();

    //Add a .pro file
    {
      std::ofstream f(tmp_pro_filename.c_str());
      f.close();
      assert(ribi::fileio::FileIo().IsRegularFile(tmp_pro_filename));
    }

    //Count the current number of .pro and .pri files again
    const std::size_t p = GetProAndPriFilesInFolder("").size();
    assert(n == p - 1);
    fileio::FileIo().DeleteFile(tmp_pro_filename);
    const std::size_t q = GetProAndPriFilesInFolder("").size();
    assert(n == q);
  }

}
