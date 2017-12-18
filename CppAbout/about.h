#ifndef RIBI_ABOUT_H
#define RIBI_ABOUT_H

#include <iosfwd>
#include <string>
#include <vector>

namespace ribi {

///About is a class to contain 'about' information
struct About
{
  explicit About(
    const std::string& author,
    const std::string& file_title,
    const std::string& file_description,
    const std::string& programmed_on,
    const std::string& years,
    const std::string& url,
    const std::string& version,
    const std::vector<std::string>& version_history
  ) noexcept;

  ///Add a library name and version
  void AddLibrary(const std::string& s) noexcept;

  ///Create the 'about' text
  std::vector<std::string> CreateAboutText() const noexcept;

  ///Create a version history
  std::vector<std::string> CreateVersionHistory() const noexcept;

  ///Create a text with all libraries used
  std::vector<std::string> CreateLibrariesUsedText() const noexcept;

  ///CreateLicence creates a GPL 3.0 licence.
  std::vector<std::string> CreateLicenceText() const noexcept;

  ///Obtain the version of this About class itself
  static std::string GetAboutVersion() noexcept;

  ///Obtain the version history of this About class itself
  static std::vector<std::string> GetAboutVersionHistory() noexcept;

  ///Get the author of this class
  const std::string& GetAuthor() const noexcept { return m_author; }

  ///GetBoostVersion returns the version of the current Boost library.
  ///From http://www.richelbilderbeek.nl/CppGetBoostVersion.htm
  static std::string GetBoostVersion() noexcept;

  ///Get the author of my classes
  static std::string GetDefaultAuthor() noexcept { return "Richel Bilderbeek"; }

  ///Get the file/class/program its description
  const std::string& GetFileDescription() const noexcept { return m_file_description; }

  ///Get the file/class/program its title
  const std::string& GetFileTitle() const noexcept { return m_file_title; }

  ///Get the date this class has been programmed on
  const std::string& GetProgrammedAt() const noexcept { return m_programmed_on; }

  ///GetQtCreatorVersion returns the version number of the Qt Creator currently installed.
  ///From http://www.richelbilderbeek.nl/CppGetQtCreatorVersion.htm
  static std::string GetQtCreatorVersion() noexcept;

  ///GetStlVersion returns the version number of the GCC STL currently installed.
  ///From http://www.richelbilderbeek.nl/CppGetStlVersion.htm
  static std::string GetStlVersion() noexcept;

  ///Get the URL this class can be downloaded from
  const std::string& GetUrl() const noexcept { return m_url; }

  ///Obtain the years this class has been worked on
  const std::string& GetYears() const noexcept { return m_years; }

  ///Obtain the version of this class
  const std::string& GetVersion() const noexcept { return m_version; }

  ///Obtain the version history of this class
  const std::vector<std::string>& GetVersionHistory() const noexcept { return m_version_history; }

  private:

  ///The author of this class
  const std::string m_author;

  ///The title or name of this class
  const std::string m_file_title;

  ///The description of this class
  const std::string m_file_description;

  ///The libraries and/or classes used by this class
  std::vector<std::string> m_libraries;

  ///The date this class was programmed
  const std::string m_programmed_on;

  ///The years this class has been worked on
  const std::string m_years;

  ///The URL this class can be downloaded from
  const std::string m_url;

  ///The version of this class
  const std::string m_version;

  ///The version history of this class
  const std::vector<std::string> m_version_history;

};

std::ostream& operator<<(std::ostream& os,const About& a) noexcept;

} //~namespace ribi

#endif // RIBI_ABOUT_H


"","x"
"1","    <state id=""state"" storeEvery=""5000"">"
"2","        <tree id=""Tree.t:anthus_aco"" name=""stateNode"">"
"3","            <taxonset id=""TaxonSet.anthus_aco"" spec=""TaxonSet"">"
"4","                <alignment idref=""anthus_aco""/>"
"5","            </taxonset>"
"6","        </tree>"
"7","        <tree id=""Tree.t:anthus_nd2"" name=""stateNode"">"
"8","            <taxonset id=""TaxonSet.anthus_nd2"" spec=""TaxonSet"">"
"9","                <alignment idref=""anthus_nd2""/>"
"10","            </taxonset>"
"11","        </tree>"
"12","        <tree id=""Tree.t:anthus_nd3"" name=""stateNode"">"
"13","            <taxonset id=""TaxonSet.anthus_nd3"" spec=""TaxonSet"">"
"14","                <alignment idref=""anthus_nd3""/>"
"15","            </taxonset>"
"16","        </tree>"
"17","        <tree id=""Tree.t:anthus_nd4"" name=""stateNode"">"
"18","            <taxonset id=""TaxonSet.anthus_nd4"" spec=""TaxonSet"">"
"19","                <alignment idref=""anthus_nd4""/>"
"20","            </taxonset>"
"21","        </tree>"
"22","        <parameter id=""gammaShape.s:anthus_aco"" name=""stateNode"">0.1</parameter>"
"23","        <parameter id=""kappa.s:anthus_nd2"" lower=""0.0"" name=""stateNode"">2.1</parameter>"
"24","        <parameter id=""freqParameter.s:anthus_nd2"" dimension=""4"" lower=""0.0"" name=""stateNode"" upper=""1.0"">0.25</parameter>"
"25","        <parameter id=""gammaShape.s:anthus_nd2"" name=""stateNode"">0.2</parameter>"
"26","        <parameter id=""kappa1.s:anthus_nd3"" lower=""0.0"" name=""stateNode"">2.2</parameter>"
"27","        <parameter id=""kappa2.s:anthus_nd3"" lower=""0.0"" name=""stateNode"">2.3</parameter>"
"28","        <parameter id=""freqParameter.s:anthus_nd3"" dimension=""4"" lower=""0.0"" name=""stateNode"" upper=""1.0"">0.25</parameter>"
"29","        <parameter id=""gammaShape.s:anthus_nd3"" name=""stateNode"">0.4</parameter>"
"30","        <parameter id=""rateAC.s:anthus_nd4"" lower=""0.0"" name=""stateNode"">1.1</parameter>"
"31","        <parameter id=""rateAG.s:anthus_nd4"" lower=""0.0"" name=""stateNode"">1.2</parameter>"
"32","        <parameter id=""rateAT.s:anthus_nd4"" lower=""0.0"" name=""stateNode"">1.3</parameter>"
"33","        <parameter id=""rateCG.s:anthus_nd4"" lower=""0.0"" name=""stateNode"">1.4</parameter>"
"34","        <parameter id=""rateGT.s:anthus_nd4"" lower=""0.0"" name=""stateNode"">1.6</parameter>"
"35","        <parameter id=""freqParameter.s:anthus_nd4"" dimension=""4"" lower=""0.0"" name=""stateNode"" upper=""1.0"">0.25</parameter>"
"36","        <parameter id=""ucldMean.c:anthus_nd2"" name=""stateNode"">1.1</parameter>"
"37","        <parameter id=""ucldStdev.c:anthus_nd2"" lower=""0.0"" name=""stateNode"">0.1</parameter>"
"38","        <stateNode id=""rateCategories.c:anthus_nd2"" spec=""parameter.IntegerParameter"" dimension=""42"">1</stateNode>"
"39","        <parameter id=""ucldMean.c:anthus_nd3"" name=""stateNode"">1.2</parameter>"
"40","        <parameter id=""ucldStdev.c:anthus_nd3"" lower=""0.0"" name=""stateNode"">0.1</parameter>"
"41","        <stateNode id=""rateCategories.c:anthus_nd3"" spec=""parameter.IntegerParameter"" dimension=""42"">1</stateNode>"
"42","        <parameter id=""ucldMean.c:anthus_nd4"" name=""stateNode"">1.3</parameter>"
"43","        <parameter id=""ucldStdev.c:anthus_nd4"" lower=""0.0"" name=""stateNode"">0.1</parameter>"
"44","        <stateNode id=""rateCategories.c:anthus_nd4"" spec=""parameter.IntegerParameter"" dimension=""42"">1</stateNode>"
"45","        <parameter id=""BDBirthRate.t:anthus_aco"" lower=""0.0"" name=""stateNode"" upper=""10000.0"">1.0</parameter>"
"46","        <parameter id=""BDDeathRate.t:anthus_aco"" lower=""0.0"" name=""stateNode"" upper=""1.0"">0.5</parameter>"
"47","        <parameter id=""popSize.t:anthus_nd2"" name=""stateNode"">0.3</parameter>"
"48","        <parameter id=""ePopSize.t:anthus_nd3"" name=""stateNode"">0.3</parameter>"
"49","        <parameter id=""growthRate.t:anthus_nd3"" name=""stateNode"">3.0E-4</parameter>"
"50","        <parameter id=""birthRate.t:anthus_nd4"" name=""stateNode"">1.0</parameter>"
"51","    </state>"
