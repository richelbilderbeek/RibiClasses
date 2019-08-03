


#include "qtcreatorprofile.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "fileio.h"





ribi::QtCreatorProFile::QtCreatorProFile(const std::string& filename)
  :
    m_config{},
    m_defines{},
    m_forms{},
    m_headers{},
    m_includepath{},
    m_libs{},
    m_other_files{},
    m_pri_files{},
    m_pro_filename{fileio::FileIo().ConvertPathToUnix(filename)},
    m_qmake_cxxflags{},
    m_qt{},
    m_resources{},
    m_target{},
    m_template{},
    m_sources{}
{
  if (!ribi::fileio::FileIo().IsRegularFile(m_pro_filename))
  {
    std::stringstream s;
    s << "Filename '" << m_pro_filename << "' must be a regular file";
    throw std::invalid_argument(s.str().c_str());
  }
  assert(fileio::FileIo().IsUnixPath(m_pro_filename));
  if (!fileio::FileIo().IsUnixPath(m_pro_filename))
  {
    std::stringstream s;
    s << "Filename '" << m_pro_filename << "' must have a Linux-stylE path "
      << " (that is, folders must be seperated by a slash, instead of a backslash)"
    ;
    throw std::invalid_argument(s.str().c_str());
  }

  std::vector<std::string> v{ribi::fileio::FileIo().FileToVector(m_pro_filename)};
  RemoveComments(v);
  DoReplacements(v);
  std::stringstream data;
  std::copy(std::begin(v),std::end(v),std::ostream_iterator<std::string>(data," "));
  Parse(data);
}

void ribi::QtCreatorProFile::DoReplacements(std::vector<std::string>& v)
{
  for (std::string& s: v)
  {
    boost::algorithm::replace_all(s,"include (","include(");
  }
}

ribi::About ribi::QtCreatorProFile::GetAbout() noexcept
{
  ribi::About a(
    "Richel Bilderbeek",
    "QtCreatorProFile",
    "class to parse Qt Project files",
    "the 19th of August 2013",
    "2010-2015",
    "http://www.richelbilderbeek.nl/CppQtCreatorProFile.htm",
    GetVersion(),
    GetVersionHistory());
  return a;
}

std::string ribi::QtCreatorProFile::GetVersion() noexcept
{
  return "3.0";
}

std::vector<std::string> ribi::QtCreatorProFile::GetVersionHistory() noexcept
{
  return {
    "2010-12-19: version 1.0: initial version",
    "2011-01-06: version 1.1: added GetCommonRoot and GetLibs member functions, added operator<<",
    "2011-09-11: version 1.2: fixed bug",
    "2012-02-25: version 1.3: added GetAbout member function",
    "2012-02-28: version 1.4: added support for INCLUDEPATH, FORMS, OTHER_FILES, RESOURCES and QMAKE_CXXFLAGS",
    "2012-05-30: version 1.5: added SimplifyPath",
    "2012-08-13: version 1.6: modifiers like win32 and unix are ignored, instead of yielding an error",
    "2012-12-23: version 1.7: set destructor to private, except for boost::checked_delete",
    "2012-12-23: version 1.8: renamed to QtCreatorProFile due to naming conflicts when cross-compiling",
    "2013-05-18: version 2.0: simplified architecture by removing file I/O",
    "2013-08-19: version 2.1: replaced Boost.Regex by Boost.Xpressive, removed Boost.Filesystem",
    "2014-01-27: version 2.2: removes all comments, can detect includes of .pri files",
    "2014-05-02: version 3.0: use UNIX path seperators only"
  };
}

void ribi::QtCreatorProFile::Parse(std::stringstream& data)
{
  const bool verbose{false};
  std::set<std::string> * p = nullptr; //A set to write to
  enum class Prefix { none, plus, minus };
  Prefix prefix = Prefix::none;
  while (data)
  {
    std::string s;
    data >> s;
    if (verbose) { std::clog << s; }
    assert(s[0] != '#' && "Comments are already removed");
    if (s[0] == '{') continue;
    if (s[0] == '}') continue;
    if (s[0] == '\\') continue;
    if (s.size() > 7 && s.substr(0,7) == "include")
    {
      std::string t = s.substr(8,s.size() - 8 - 1);
      if (verbose) { std::clog << t; }
      while (t[0] == ' ' || t[0] == '(') t = t.substr(1,t.size()-1);
      if (verbose) { std::clog << t; }
      while (t.back() == ' ' || t.back() == ')') t.pop_back();
      if (verbose) { std::clog << t; }
      assert(t.find('(') == std::string::npos);
      assert(t.find(')') == std::string::npos);
      m_pri_files.insert(t);
      continue;
    }
    const std::vector<std::string> conditional_sections {
      "unix", "win32", "static", "debug", "release"
    };
    if (std::count(conditional_sections.begin(),conditional_sections.end(),s))
    {
      p = nullptr;
      continue;
    }
    const std::map<std::string,std::set<std::string>* > m {
      { "CONFIG"        ,&m_config },
      { "DEFINES"       ,&m_defines },
      { "FORMS"         ,&m_forms },
      { "HEADERS"       ,&m_headers },
      { "INCLUDEPATH"   ,&m_includepath },
      { "LIBS"          ,&m_libs },
      { "OTHER_FILES"   ,&m_other_files },
      { "QMAKE_CXXFLAGS",&m_qmake_cxxflags },
      { "QT"            ,&m_qt },
      { "RESOURCES"     ,&m_resources },
      { "SOURCES"       ,&m_sources },
      { "TARGET"        ,&m_target },
      { "TEMPLATE"      ,&m_template }
    };
    const std::map<std::string,std::set<std::string>* >::const_iterator iter {
      std::find_if(std::begin(m),std::end(m),
        [s](const std::pair<std::string,std::set<std::string>* > sub_pair)
        {
          return sub_pair.first == s;
        }
      )
    };
    if (iter != std::end(m))
    {
      if (verbose) { const std::string msg = "Set pointer to " + iter->first; std::clog << msg; }
      p = iter->second;
      prefix = Prefix::none;
      continue;
    }
    //Determine prefixes
    bool has_prefixes = true;
    while (has_prefixes)
    {
      has_prefixes = false;
      if (!s.empty() && s[0] == '+')
      {
        prefix = Prefix::plus;
        s = s.substr(1,s.size() - 1);
        has_prefixes = true;
      }
      else if (!s.empty() && s[0] == '-')
      {
        prefix = Prefix::minus;
        s = s.substr(1,s.size() - 1);
        has_prefixes = true;
      }
      else if (!s.empty() && s[0] == '\\')
      {
        s = s.substr(1,s.size() - 1);
        has_prefixes = true;
      }
      else if (!s.empty() && s[0] == '=')
      {
        s = s.substr(1,s.size() - 1);
        has_prefixes = true;
      }
    }
    //Remove possible postfix
    if (!s.empty())
    {
      if (s[ s.size() - 1] == '\\') s.resize(s.size() - 1);

    }
    if (p && !s.empty())
    {
      if (verbose) { const std::string msg = "Added " + s; std::clog << msg; }
      p->insert(
        (prefix == Prefix::minus ? "-" : "") + s);
    }
  }
}

void ribi::QtCreatorProFile::RemoveComments(std::vector<std::string>& v)
{
  for (std::string& s: v)
  {
    const std::string t = boost::algorithm::trim_copy(s);
    if (t[0] == '#')
    {
      s = "";
      continue;
    }
    if (t.size() >= 7 && t.substr(0,7) == "message")
    {
      s = "";
      continue;
    }

    const auto iter = std::find(std::begin(t),std::end(t),'#');
    if (iter != std::end(t))
    {
      s.assign(std::begin(t),iter);
    }

    assert(std::find(std::begin(s),std::end(s),'#') == std::end(s)
      && "Every comment is removed");
  }

}

std::ostream& ribi::operator<<(std::ostream& os, const QtCreatorProFile& p)
{
  os << "\n";
  for (const std::string& pri: p.GetPriFiles())
  {
    os << "include(" << pri << ")" << '\n';
  }
  {
    const std::vector<
      std::pair<
        std::string, std::function<const std::set<std::string>& (const QtCreatorProFile&)>
        >
      > v = {
        { "CONFIG", &QtCreatorProFile::GetConfig },
        { "DEFINES", &QtCreatorProFile::GetDefines },
        { "FORMS", &QtCreatorProFile::GetForms },
        { "HEADERS", &QtCreatorProFile::GetHeaders },
        { "INCLUDEPATH", &QtCreatorProFile::GetIncludepath },
        { "LIBS", &QtCreatorProFile::GetLibs },
        { "OTHER_FILES", &QtCreatorProFile::GetOtherFiles },
        { "QMAKE_CXXFLAGS", &QtCreatorProFile::GetQmakeCxxflags },
        { "QT", &QtCreatorProFile::GetQt},
        { "RESOURCES", &QtCreatorProFile::GetResources },
        { "SOURCES", &QtCreatorProFile::GetSources },
        { "TARGET", &QtCreatorProFile::GetTarget },
        { "TEMPLATE", &QtCreatorProFile::GetTemplate }
      };

    std::for_each(v.begin(),v.end(),
      [&os,&p](const std::pair<std::string, std::function<const std::set<std::string>& (const QtCreatorProFile&)> >& pair)
      {
        const std::set<std::string>& w = pair.second(p);
        if (!w.empty())
        {
          os << pair.first << " += \\\n";
          if (w.size() > 1)
          {
            std::transform(w.begin(),--w.end(),std::ostream_iterator<std::string>(os," \\\n"),
              [](const std::string& s) { return "    " + s; } );
          }
          os << "    " + (*(--w.end())) + '\n';
          os << "\n";
        }
      }
    );
  }
  os << "\n";
  os << "#--------------------------------------------------------------------------\n";
  os << "# This file was created by:\n";
  os << "#\n";
  {
    const std::vector<std::string> v = p.GetAbout().CreateAboutText();
    std::transform(v.begin(),v.end(),std::ostream_iterator<std::string>(os,"\n"),
      [](const std::string& s) { return "# " + s; } );
  }
  os << "#\n";
  os << "#\n";
  os << "#\n";
  {
    const std::vector<std::string> v = p.GetAbout().CreateLicenceText();
    std::transform(v.begin(),v.end(),std::ostream_iterator<std::string>(os,"\n"),
      [](const std::string& s) { return "# " + s; } );
  }
  os << "#--------------------------------------------------------------------------";
  return os;
}

bool ribi::operator==(const QtCreatorProFile& lhs, const QtCreatorProFile& rhs)
{
  return
       lhs.m_config == rhs.m_config
    && lhs.m_defines == rhs.m_defines
    && lhs.m_forms == rhs.m_forms
    && lhs.m_headers == rhs.m_headers
    && lhs.m_includepath == rhs.m_includepath
    && lhs.m_libs == rhs.m_libs
    && lhs.m_other_files == rhs.m_other_files
    && lhs.m_pro_filename == rhs.m_pro_filename
    && lhs.m_qmake_cxxflags == rhs.m_qmake_cxxflags
    && lhs.m_qt == rhs.m_qt
    && lhs.m_resources == rhs.m_resources
    && lhs.m_target == rhs.m_target
    && lhs.m_template == rhs.m_template
    && lhs.m_sources == rhs.m_sources;
}
