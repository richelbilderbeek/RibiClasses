#ifndef QTCREATORPROFILEZIPSCRIPT
#define QTCREATORPROFILEZIPSCRIPT

#include <set>
#include <string>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/checked_delete.hpp>
#include <boost/shared_ptr.hpp>
#include "qtcreatorprofile.h"
#pragma GCC diagnostic pop

namespace ribi {

///Create a shell script to zip a Qt Creator .pro file
struct QtCreatorProFileZipScript
{
  QtCreatorProFileZipScript(
    const boost::shared_ptr<const ribi::QtCreatorProFile> pro_file
  );
  QtCreatorProFileZipScript(const QtCreatorProFileZipScript&) = delete;
  QtCreatorProFileZipScript& operator=(const QtCreatorProFileZipScript&) = delete;

  ///Create a script to zip all .pro files (and all they refer to) in a folder
  static std::string CreateScript(const std::string& source_folder);

  ///Obtain this class its About information
  static About GetAbout() noexcept;

  ///Obtain all filenames
  const std::set<std::string>& GetFilenames() const { return m_filenames; }

  ///Obtain the .pro file
  //const boost::shared_ptr<const QtCreatorProFile> GetProFile() const { return m_pro_file; }
  const std::string& GetProFileName() const { return m_pro_file_name; }

  ///Obtain this class its version
  static std::string GetVersion() noexcept;

  ///Obtain this class its version history
  static std::vector<std::string> GetVersionHistory() noexcept;

  static boost::shared_ptr<QtCreatorProFileZipScript> Merge(
    const std::vector<boost::shared_ptr<const QtCreatorProFileZipScript> >& v);

  ///Set all filenames
  void SetFilenames(const std::set<std::string>& filenames) { m_filenames = filenames; }

  private:
  ///Used when merging
  QtCreatorProFileZipScript(
    const std::set<std::string>& filenames,
    const std::string& pro_file_name);

  ///Be sure the class is correctly deleted
  ~QtCreatorProFileZipScript() noexcept {}
  friend void boost::checked_delete<>(QtCreatorProFileZipScript* x);

  ///All the files used by the QtCreatorProFile
  std::set<std::string> m_filenames;

  ///The original .pro file its filename
  const std::string m_pro_file_name;

  ///Create a QtCreatorProFile from every filename
  static std::vector<boost::shared_ptr<QtCreatorProFile> > CreateProFiles(const std::vector<std::string>& pro_files);

  ///Extract a QtCreatorProFile its filenames
  const std::set<std::string> ExtractFilenames(
    const boost::shared_ptr<const QtCreatorProFile> & pro_file) const;

  friend std::ostream& operator<<(std::ostream& os,const QtCreatorProFileZipScript& script) noexcept;
};

///Get all the .pro and .pri files in a folder
//From http://www.richelbilderbeek.nl/CppGetProFilesInFolder.htm
std::vector<std::string> GetProAndPriFilesInFolder(const std::string& folder);

std::ostream& operator<<(std::ostream& os,const QtCreatorProFileZipScript& script) noexcept;

} //~namespace ribi

#endif // QTCREATORPROFILEZIPSCRIPT
