#include "qtcreatorprofile.h"

#include <fstream>
#include "fileio.h"

#include <boost/test/unit_test.hpp>

using namespace ribi;

BOOST_AUTO_TEST_CASE(ribi_qtcreatorprofile_tests)
{
  {
    const std::string mypath { fileio::FileIo().GetTempFileName() };
    {
      std::ofstream f(mypath);
      f << "SOURCES += qtmain.cpp";
    }
    //Check the project file
    const QtCreatorProFile p(mypath);
    assert(p.GetSources().size() == 1);
    assert(p.GetSources().count("qtmain.cpp"));
    fileio::FileIo().DeleteFile(mypath.c_str());
  }
  {
    const std::string mypath { fileio::FileIo().GetTempFileName() };
    {
      std::ofstream f(mypath);
      f << "include(something.pri)";
    }
    //Check the project file
    const QtCreatorProFile p(mypath);
    assert(p.GetPriFiles().size() == 1);
    assert(p.GetPriFiles().count("something.pri"));
    fileio::FileIo().DeleteFile(mypath.c_str());
  }
  {
    const std::string mypath { fileio::FileIo().GetTempFileName() };
    {
      std::ofstream f(mypath);
      f << "include (something.pri)";
    }
    //Check the project file
    const QtCreatorProFile p(mypath);
    assert(p.GetPriFiles().size() == 1);
    assert(p.GetPriFiles().count("something.pri"));
    fileio::FileIo().DeleteFile(mypath.c_str());
  }
  {
    const std::string mypath { fileio::FileIo().GetTempFileName() };
    {
      std::ofstream f(mypath);
      f << "HEADERS += header.h #Must remove this comment";
    }
    //Check the project file
    const QtCreatorProFile p(mypath);
    assert(p.GetHeaders().size() == 1);
    assert(p.GetHeaders().count("header.h"));
    fileio::FileIo().DeleteFile(mypath.c_str());
  }
  {
    const std::string mypath { fileio::FileIo().GetTempFileName() };
    {
      std::ofstream f(mypath);
      f << "#-------------------------------------------------\n"
        << "#\n"
        << "# Project created by QtCreator 2010-12-19T16:24:53\n"
        << "#\n"
        << "#-------------------------------------------------\n"
        << "QT       += core\n"
        << "QT       -= gui\n"
        << "LIBS     += -lwt -lboost_regex\n"
        << "TARGET = ToolTestQtCreatorProFile\n"
        << "CONFIG   += console\n"
        << "CONFIG   -= app_bundle\n"
        << "TEMPLATE = app\n"
        << "SOURCES += main.cpp \\\n"
        << "    profile.cpp\n"
        << "HEADERS += \\\n"
        << "    profile.h";
    }
    //Check the project file
    const QtCreatorProFile p(mypath);
    assert(p.GetConfig().size() == 2);
    assert(p.GetConfig().count("console"));
    assert(p.GetConfig().count("-app_bundle"));
    assert(p.GetHeaders().size() == 1);
    assert(p.GetHeaders().count("profile.h"));
    assert(p.GetLibs().size() == 2);
    assert(p.GetLibs().count("-lwt"));
    assert(p.GetLibs().count("-lboost_regex"));
    assert(p.GetQtCreatorProFilename() == mypath);
    assert(p.GetQt().size() == 2);
    assert(p.GetQt().count("core"));
    assert(p.GetQt().count("-gui"));
    assert(p.GetSources().size() == 2);
    assert(p.GetSources().count("main.cpp"));
    assert(p.GetSources().count("profile.cpp"));
    assert(p.GetTarget().count("ToolTestQtCreatorProFile") == 1);
    assert(p.GetTemplate().size() == 1);
    assert(p.GetTemplate().count("app"));
    {
      std::stringstream ss;
      ss << p << '\n';
    }
    //std::clog << "Test QtCreatorProFile::operator==");
    {
      QtCreatorProFile q(mypath);
      assert(p == q);
    }
    fileio::FileIo().DeleteFile(mypath.c_str());
  }
  //std::clog << "Test QtCreatorProFile::Merge");
  {
    const std::string mypath1 { fileio::FileIo().GetTempFileName() };
    const std::string mypath2 { fileio::FileIo().GetTempFileName() };
    {
      std::ofstream f(mypath1);
      f << "#-------------------------------------------------\n"
        << "#\n"
        << "# Project created by QtCreator 2010-12-19T16:24:53\n"
        << "#\n"
        << "#-------------------------------------------------\n"
        << "QT       += core\n"
        << "QT       -= gui\n"
        << "LIBS     += -lwt -lboost_regex\n"
        << "TARGET = ToolTestQtCreatorProFileWebsite\n"
        << "CONFIG   += console\n"
        << "CONFIG   -= app_bundle\n"
        << "TEMPLATE = app\n"
        << "SOURCES += wtmain.cpp \\\n"
        << "    profile.cpp\n"
        << "HEADERS += \\\n"
        << "    profile.h"
      ;
    }
    {
      std::ofstream f(mypath2);
      f << "#-------------------------------------------------\n"
        << "#\n"
        << "# Project created by QtCreator 2010-12-19T16:24:53\n"
        << "#\n"
        << "#-------------------------------------------------\n"
        << "QT       -= core\n"
        << "QT       -= gui\n"
        << "LIBS     += -lboost_regex\n"
        << "TARGET = ToolTestQtCreatorProFileConsole\n"
        << "CONFIG   += console\n"
        << "CONFIG   -= app_bundle\n"
        << "TEMPLATE = app\n"
        << "SOURCES += main.cpp \\\n"
        << "    profile.cpp\n"
        << "HEADERS += \\\n"
        << "    profile.h";
    }
    //Check the project file
    const QtCreatorProFile p1(mypath1);
    const QtCreatorProFile p2(mypath2);
    fileio::FileIo().DeleteFile(mypath1.c_str());
    fileio::FileIo().DeleteFile(mypath2.c_str());
  }
  //Test conditionals
  {
    //Create a project file
    const std::string mypath { fileio::FileIo().GetTempFileName() };
    {
      std::ofstream f(mypath);
      f
        << "QT       += core\n"
        << "unix {\n"
        << "SOURCES += \\\n"
        << "    unix_main.cpp \\\n"
        << "}\n"
        << "win32 {\n"
        << "TARGET = TestTarget\n"
        << "SOURCES += win_main.cpp\n"
        << "}\n"
        << "SOURCES += profile.cpp\n"
        << "HEADERS += \\\n"
        << "    profile.h";
    }
    //Check the project file
    const QtCreatorProFile p(mypath);
    assert(p.GetQt().size() == 1);
    assert(p.GetQt().count("core"));
    assert(p.GetSources().size() == 3);
    assert(p.GetSources().count("unix_main.cpp"));
    assert(p.GetSources().count("win_main.cpp"));
    assert(p.GetSources().count("profile.cpp"));
    assert(p.GetHeaders().size() == 1);
    assert(p.GetHeaders().count("profile.h"));

    //Test operator<<
    {
      std::stringstream ss;
      ss << p << '\n';
    }
    //Test operator==
    {
      QtCreatorProFile q(mypath);
      assert(p == q);
    }
    fileio::FileIo().DeleteFile(mypath.c_str());
  }
}
