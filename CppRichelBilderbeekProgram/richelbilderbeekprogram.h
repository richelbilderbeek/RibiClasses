//---------------------------------------------------------------------------
/*
Program, class for a program by Richel Bilderbeek
Copyright (C) 2012-2015 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/ProjectRichelBilderbeekProgram.htm
//---------------------------------------------------------------------------
#ifndef RIBI_RICHELBILDERBEEKPROGRAM_H
#define RIBI_RICHELBILDERBEEKPROGRAM_H

#include <string>





#include <boost/shared_ptr.hpp>
#include "richelbilderbeekprogramstatus.h"
#include "richelbilderbeekprogramtype.h"


namespace ribi {

///RichelBilderbeek::Program is the base class for program information
struct Program
{
  virtual ~Program() noexcept {}

  ///Create a std::vector with all different Program instances in it
  static boost::shared_ptr<Program> CreateProgram(const ProgramType type) noexcept;

  ///Create a std::vector with all different Program instances in it
  static std::vector<boost::shared_ptr<Program>> GetAllPrograms() noexcept;

  ///Get the filename of a console version screenshot
  virtual std::string GetFilenameConsole() const noexcept = 0;

  ///Get the filename of a desktop platform-independent version screenshot
  virtual std::string GetFilenameDesktop() const noexcept = 0;

  ///Get the filename of a desktop windows-only version screenshot
  virtual std::string GetFilenameDesktopWindowsOnly() const noexcept = 0;

  ///Get the filename of a web application version screenshot
  virtual std::string GetFilenameWeb() const noexcept = 0;

  ///Status of the console-only version
  virtual ProgramStatus GetStatusConsole() const noexcept = 0;

  ///Status of the Windows-only desktop version
  virtual ProgramStatus GetStatusDesktopWindowsOnly() const noexcept = 0;

  ///Status of the platform-independent desktop version
  virtual ProgramStatus GetStatusDesktop() const noexcept = 0;

  ///Status of the web application version
  virtual ProgramStatus GetStatusWebApplication() const noexcept = 0;

  ///The item its enum name, e.g. 'k3OpEenRij'
  std::string GetName() const noexcept;

  ///The item its on-screen name, e.g. 'K3-Op-Een-Rij'
  virtual std::string GetScreenName() const noexcept;

  ///The program, e.g. 'RichelBilderbeekProgramType::boenken'
  virtual ProgramType GetType() const noexcept = 0;

  ///The type of program its name, e.g. 'Game'
  virtual std::string GetTypeName() const noexcept = 0;

  ///The item its URL (at http://ww.richelbilderbeek.nl), e.g. 'ToolSimMysteryMachine.htm'
  virtual std::string GetUrl() const noexcept = 0;

  ///Obtain the version of this class
  static std::string GetVersion() noexcept;

  ///Obtain the version history of this class
  static std::vector<std::string> GetVersionHistory() noexcept;

  private:
  ///Get the base of a filename
  std::string GetFilenameBase() const noexcept;
};

///ProgramClass is the base class for all class demonstration programs
struct ProgramClass : public Program
{
  virtual ~ProgramClass() noexcept {}

  ///The type of program its name, e.g. 'game'
  std::string GetTypeName() const noexcept override { return "class"; }
};

struct ProgramGame : public Program
{
  virtual ~ProgramGame() noexcept {}

  ///The type of program its name, e.g. 'game'
  std::string GetTypeName() const noexcept override { return "game"; }
};

struct ProgramProject : public Program
{
  virtual ~ProgramProject() noexcept {}

  ///The type of program its name, e.g. 'game'
  std::string GetTypeName() const noexcept override { return "project"; }
};

struct ProgramTool : public Program
{
  virtual ~ProgramTool() noexcept {}

  ///The type of program its name, e.g. 'game'
  std::string GetTypeName() const noexcept override { return "tool"; }
};

struct ProgramAsciiArter : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolAsciiArter_6_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return "ToolAsciiArter_5_2.png"; }
  std::string GetScreenName() const noexcept override final { return "AsciiArter"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::asciiArter; }
  std::string GetUrl() const noexcept override final { return "ToolAsciiArter.htm"; }
};

struct ProgramBeerWanter : public ProgramGame
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "GameBeerWanter5.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "BeerWanter"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::beerWanter; }
  std::string GetUrl() const noexcept override final { return "GameBeerWanter.htm"; }
};

struct ProgramBoenken : public ProgramGame
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "GameBoenken_4_1.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "Boenken"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::boenken; }
  std::string GetUrl() const noexcept override final { return "GameBoenken.htm"; }
};

struct ProgramBrainweaver : public ProgramProject
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "Brainweaver"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::brainweaver; }
  std::string GetUrl() const noexcept override final { return "ProjectBrainweaver.htm"; }
};

struct ProgramCaesarCipher : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolCaesarCipher_1_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "CaesarCipher"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::wip; }
  ProgramType GetType() const noexcept override final { return ProgramType::caesarCipher; }
  std::string GetUrl() const noexcept override final { return "ToolCaesarCipher.htm"; }
};

struct ProgramCodeToHtml : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolCodeToHtml_3_1.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return "ToolCodeToHtml_2_0.png"; }
  std::string GetScreenName() const noexcept override final { return "CodeToHtml"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::codeToHtml; }
  std::string GetUrl() const noexcept override final { return "ToolCodeToHtml.htm"; }
};

struct ProgramConnectThree : public ProgramGame
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "GameConnectThree_5_2.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return "GameConnectThree_6_0.png"; }
  std::string GetScreenName() const noexcept override final { return "ConnectThree"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::connectThree; }
  std::string GetUrl() const noexcept override final { return "GameConnectThree.htm"; }
};

struct ProgramCorridor : public ProgramGame
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "GameCorridor.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "Corridor"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::nvr; }
  ProgramType GetType() const noexcept override final { return ProgramType::corridor; }
  std::string GetUrl() const noexcept override final { return "GameCorridor.htm"; }
};

struct ProgramCreateGlossary : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolCreateGlossary_1_2.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "CreateGlossary"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::createGlossary; }
  std::string GetUrl() const noexcept override final { return "ToolCreateGlossary.htm"; }
};

struct ProgramCreateQtProjectZipFile : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolCreateQtProjectZipFile_2_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "CreateQtProjectZipFile"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::createQtProjectZipFile; }
  std::string GetUrl() const noexcept override final { return "ToolCreateQtProjectZipFile.htm"; }
};

struct ProgramDasWahreSchlagerfest : public ProgramGame
{
  std::string GetFilenameConsole() const noexcept override final { return "GameDasWahreSchlagerfest_2_5.png"; }
  std::string GetFilenameDesktop() const noexcept override final { return "GameDasWahreSchlagerfest_2_2.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "Das Wahre Schlagerfest"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::dasWahreSchlagerfest; }
  std::string GetUrl() const noexcept override final { return "GameDasWahreSchlagerfest.htm"; }
};

struct ProgramDotMatrix : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolDotMatrix_2_1.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "DotMatrix"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::dotMatrix; }
  std::string GetUrl() const noexcept override final { return "ToolDotMatrix.htm"; }
};

struct ProgramEncranger : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolEncranger_2_5.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return "ToolEncranger_1_0.png"; }
  std::string GetFilenameWeb() const noexcept override final { return "ToolEncranger_2_1.png"; }
  std::string GetScreenName() const noexcept override final { return "Encranger"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::encranger; }
  std::string GetUrl() const noexcept override final { return "ToolEncranger.htm"; }
};

struct ProgramFilterOperationer : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolFilterOperationer_2_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "FilterOperationer"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::filterOperationer; }
  std::string GetUrl() const noexcept override final { return "ToolFilterOperationer.htm"; }
};

struct ProgramFisherWrighter : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "FisherWrighter"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::fisherWrighter; }
  std::string GetUrl() const noexcept override final { return "ToolFisherWrighter.htm"; }
};

struct ProgramFunctionPlotter : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolFunctionPlotter_2_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "FunctionPlotter"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::functionPlotter; }
  std::string GetUrl() const noexcept override final { return "ToolFunctionPlotter.htm"; }
};

struct ProgramGaborFilter : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolGaborFilter_2_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "GaborFilter"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::gaborFilter; }
  std::string GetUrl() const noexcept override final { return "ToolGaborFilter.htm"; }
};

struct ProgramGrayCoder : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolGrayCoder_2_1.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "GrayCoder"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::grayCoder; }
  std::string GetUrl() const noexcept override final { return "ToolGrayCoder.htm"; }
};

struct ProgramGtst : public ProgramProject
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return "ProjectGtst_1_2.png"; }
  std::string GetScreenName() const noexcept override final { return "GTST"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::gtst; }
  std::string GetUrl() const noexcept override final { return "ProjectGtst.htm"; }
};

struct ProgramHistogramEqualizationer : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolHistogramEqualizationer_2_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "HistogramEqualizationer"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::unk; }
  ProgramType GetType() const noexcept override final { return ProgramType::histogramEqualizationer; }
  std::string GetUrl() const noexcept override final { return "ToolHistogramEqualizationer.htm"; }
};

struct ProgramHometrainer : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return "ToolHometrainer_2_0.png"; }
  std::string GetScreenName() const noexcept override final { return "Hometrainer"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::hometrainer; }
  std::string GetUrl() const noexcept override final { return "ToolHometrainer.htm"; }
};

struct ProgramImageRotater : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolImageRotater_2_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "ImageRotater"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::unk; }
  ProgramType GetType() const noexcept override final { return ProgramType::imageRotater; }
  std::string GetUrl() const noexcept override final { return "ToolImageRotater.htm"; }
};

struct ProgramK3OpEenRij : public ProgramGame
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "GameK3OpEenRij_6_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return "GameK3OpEenRij_5_0.png"; }
  std::string GetScreenName() const noexcept override final { return "K3 Op Een Rij"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::k3OpEenRij; }
  std::string GetUrl() const noexcept override final { return "GameK3OpEenRij.htm"; }
};

struct ProgramKalmanFilterer : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolKalmanFilterer_1_15.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "KalmanFilterer"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::kalmanFilterer; }
  std::string GetUrl() const noexcept override final { return "ToolKalmanFilterer.htm"; }
};

struct ProgramKeySender : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolKeySender_1_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "KeySender"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::keySender; }
  std::string GetUrl() const noexcept override final { return "ToolKeySender.htm"; }
};

struct ProgramKTouchCppLessonsCreator : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "KTouchCppLessonsCreator"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::kTouchCppLessonsCreator; }
  std::string GetUrl() const noexcept override final { return "ToolKTouchCppLessonsCreator.htm"; }
};

struct ProgramLizardPaperRockScissorsSpock : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "LizardPaperRockScissorsSpock"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::lizardPaperRockScissorsSpock; }
  std::string GetUrl() const noexcept override final { return "ToolLizardPaperRockScissorsSpock.htm"; }
};

struct ProgramLogisticGrowthSimulator : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return "ToolLogisticGrowthSimulator_1_0.png"; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "LogisticGrowthSimulator"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::logisticGrowthSimulator; }
  std::string GetUrl() const noexcept override final { return "ToolLogisticGrowthSimulator.htm"; }
};

struct ProgramMazeCreator : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolMazeCreator_2_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "MazeCreator"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::mazeCreator; }
  std::string GetUrl() const noexcept override final { return "ToolMazeCreator.htm"; }
};

struct ProgramMaziak : public ProgramGame
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "GameMaziak_1_3.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "Maziak"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::maziak; }
  std::string GetUrl() const noexcept override final { return "GameMaziak.htm"; }
};

struct ProgramMorpher : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return "ToolMorpher_1_0.png"; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "Morpher"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::morpher; }
  std::string GetUrl() const noexcept override final { return "ToolMorpher.htm"; }
};

struct ProgramMultiEncranger : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolMultiEncranger_2_3.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "MultiEncranger"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::multiEncranger; }
  std::string GetUrl() const noexcept override final { return "ToolMultiEncranger.htm"; }
};

struct ProgramMusicTheory : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolMusicTheory_1_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "MusicTheory"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::musicTheory; }
  std::string GetUrl() const noexcept override final { return "ToolMusicTheory.htm"; }
};

struct ProgramMutualismBreakdowner : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "MutualismBreakdowner"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::mutualismBreakdowner; }
  std::string GetUrl() const noexcept override final { return "ToolMutualismBreakdowner.htm"; }
};

struct ProgramNdsmake : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "Ndsmake"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::ndsmake; }
  std::string GetUrl() const noexcept override final { return "ToolNdsmake.htm"; }
};


struct ProgramPaperRockScissors : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "PaperRockScissors"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::paperRockScissors; }
  std::string GetUrl() const noexcept override final { return "ToolPaperRockScissors.htm"; }
};

struct ProgramPaperRockScissorsWithTrait : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "PaperRockScissorsWithTrait"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::paperRockScissorsWithTrait; }
  std::string GetUrl() const noexcept override final { return "ToolPaperRockScissorsWithTrait.htm"; }
};

struct ProgramPause : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "Pause"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::pause; }
  std::string GetUrl() const noexcept override final { return "ToolPause.htm"; }
};

struct ProgramPerfectElasticCollision : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolPerfectElasticCollision_1_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "PerfectElasticCollision"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::perfectElasticCollision; }
  std::string GetUrl() const noexcept override final { return "ToolPerfectElasticCollision.htm"; }
};

struct ProgramPicToCode : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolPicToCode_1_2.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return "ToolPicToCode_1_3.png"; }
  std::string GetScreenName() const noexcept override final { return "PicToCode"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::picToCode; }
  std::string GetUrl() const noexcept override final { return "ToolPicToCode.htm"; }
};

struct ProgramPixelator : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "Pixelator"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::pixelator; }
  std::string GetUrl() const noexcept override final { return "ToolPixelator.htm"; }
};

struct ProgramPreDickAdvocaTor : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolPreDickAdvocaTor_2_1.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "PreDickAdvocaTor"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::preDickAdvocaTor; }
  std::string GetUrl() const noexcept override final { return "ToolPreDickAdvocaTor.htm"; }
};

struct ProgramPrimeExpert : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolPrimeExpert_2_1.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return "ToolTestPrimeExpert_1_0.png"; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "PrimeExpert"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::primeExpert; }
  std::string GetUrl() const noexcept override final { return "ToolPrimeExpert.htm"; }
};

struct ProgramProjectRichelBilderbeek : public ProgramProject
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ProjectRichelBilderbeek_1_14.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "ProjectRichelBilderbeek"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::projectRichelBilderbeek; }
  std::string GetUrl() const noexcept override final { return "ProjectRichelBilderbeek.htm"; }
};


struct ProgramPylos : public ProgramGame
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "GamePylos_2_1.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "Pylos"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::wip; }
  ProgramType GetType() const noexcept override final { return ProgramType::pylos; }
  std::string GetUrl() const noexcept override final { return "GamePylos.htm"; }
};

struct ProgramQmakeWatcher : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolQmakeWatcher_1_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "QmakeWatcher"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::qmakeWatcher; }
  std::string GetUrl() const noexcept override final { return "ToolQmakeWatcher.htm"; }
};

struct ProgramQuadraticSolver : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return "ToolQuadraticSolver_1_0.png"; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolQuadraticSolver_2_1.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "QuadraticSolver"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::quadraticSolver; }
  std::string GetUrl() const noexcept override final { return "ToolQuadraticSolver.htm"; }
};

struct ProgramRampalEtienne : public ProgramProject
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "ProjectRampalEtienne"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::rampalEtienne; }
  std::string GetUrl() const noexcept override final { return "ProjectRampalEtienne.htm"; }
};

struct ProgramRandomCode : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolRandomCode_4_2.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return "ToolRandomCode_3_0.png"; }
  std::string GetScreenName() const noexcept override final { return "RandomCode"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::randomCode; }
  std::string GetUrl() const noexcept override final { return "ToolRandomCode.htm"; }
};

struct ProgramRegexTester : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolRegexTester_1_1.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "RegexTester"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::regexTester; }
  std::string GetUrl() const noexcept override final { return "ToolRegexTester.htm"; }
};

struct ProgramReversi : public ProgramGame
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return "GameReversi_0_1.png"; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "Reversi"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::wip; }
  ProgramType GetType() const noexcept override final { return ProgramType::reversi; }
  std::string GetUrl() const noexcept override final { return "GameReversi.htm"; }
};

struct ProgramRichelBilderbeekGallery : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolRichelBilderbeekGallery_1_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "RichelBilderbeekGallery"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::richelBilderbeekGallery; }
  std::string GetUrl() const noexcept override final { return "ToolRichelBilderbeekGallery.htm"; }
};

struct ProgramRichelBilderbeekNlSitemapGenerator : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "RichelBilderbeekNlSitemapGenerator"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::richelbilderbeekNlSitemapGenerator; }
  std::string GetUrl() const noexcept override final { return "ToolRichelbilderbeekNlSitemapGenerator.htm"; }
};

struct ProgramRubiksClock : public ProgramGame
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "GameRubiksClock_2_3.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return "GameRubiksClock_2_1.png"; }
  std::string GetScreenName() const noexcept override final { return "Rubik's Clock"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::rubiksClock; }
  std::string GetUrl() const noexcept override final { return "GameRubiksClock.htm"; }
};

struct ProgramSearchAndDestroyChess : public ProgramGame
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "Search And Destroy Chess"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::searchAndDestroyChess; }
  std::string GetUrl() const noexcept override final { return "GameSearchAndDestroyChess.htm"; }
};

struct ProgramSecretMessage : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolSecretMessage_2_1.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "SecretMessage"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::wip; }
  ProgramType GetType() const noexcept override final { return ProgramType::secretMessage; }
  std::string GetUrl() const noexcept override final { return "ToolSecretMessage.htm"; }
};

struct ProgramSimMysteryMachine : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolSimMysteryMachine_1_2.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return "ToolSimMysteryMachine_1_1_web.png"; }
  std::string GetScreenName() const noexcept override final { return "SimMysteryMachine"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::simMysteryMachine; }
  std::string GetUrl() const noexcept override final { return "ToolSimMysteryMachine.htm"; }
};

struct ProgramSimplifyNewick : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolSimplifyNewick_1_2.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "SimplifyNewick"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::simplifyNewick; }
  std::string GetUrl() const noexcept override final { return "ToolSimplifyNewick.htm"; }
};

struct ProgramSimTopPredatorPrey : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "SimTopPredatorPrey"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::simTopPredatorPrey; }
  std::string GetUrl() const noexcept override final { return "ToolSimTopPredatorPrey"; }
};

struct ProgramSolvePuzzleX : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "SolvePuzzleX"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::solvePuzzleX; }
  std::string GetUrl() const noexcept override final { return "ToolSolvePuzzleX.htm"; }
};

struct ProgramStateObserver : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolStateObserver_1_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "StateObserver"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::stateObserver; }
  std::string GetUrl() const noexcept override final { return "ToolStateObserver.htm"; }
};


struct ProgramStochasticityInspector : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "StochasticityInspector"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::stochasticityInspector; }
  std::string GetUrl() const noexcept override final { return "ToolStochasticityInspector.htm"; }
};

struct ProgramStyleSheetSetter : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "StyleSheetSetter"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::styleSheetSetter; }
  std::string GetUrl() const noexcept override final { return "ToolStyleSheetSetter.htm"; }
};

struct ProgramSurfacePlotter : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolSurfacePlotter_2_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "SurfacePlotter"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::surfacePlotter; }
  std::string GetUrl() const noexcept override final { return "ToolSurfacePlotter.htm"; }
};

struct ProgramTankBattalion : public ProgramGame
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TankBattalion"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::tankBattalion; }
  std::string GetUrl() const noexcept override final { return "GameTankBattalion.htm"; }
};

struct ProgramTestAbout : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestAbout_1_1.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestAbout"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testAbout; }
  std::string GetUrl() const noexcept override final { return "ToolTestAbout.htm"; }
};

struct ProgramTestApproximator : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestApproximator_1_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestApproximator"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testApproximator; }
  std::string GetUrl() const noexcept override final { return "ToolTestApproximator.htm"; }
};

struct ProgramTestBinaryNewickVector : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestBinaryNewickVector_1_5.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestBinaryNewickVector"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testBinaryNewickVector; }
  std::string GetUrl() const noexcept override final { return "ToolTestBinaryNewickVector.htm"; }
};

struct ProgramTestBouncingBallsWidget : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestBouncingBallsWidget_1_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestBouncingBallsWidget"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testBouncingBallsWidget; }
  std::string GetUrl() const noexcept override final { return "ToolTestBouncingBallsWidget.htm"; }
};

struct ProgramTestBouncingRectsWidget : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestBouncingRectsWidget_1_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestBouncingRectsWidget"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testBouncingRectsWidget; }
  std::string GetUrl() const noexcept override final { return "ToolTestBouncingRectsWidget.htm"; }
};

struct ProgramTestBroadcastServer : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return "ToolTestBroadcastServer_1_0.png"; }
  std::string GetScreenName() const noexcept override final { return "TestBroadcastServer"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::n_a; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::n_a; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::testBroadcastServer; }
  std::string GetUrl() const noexcept override final { return "ToolTestBroadcastServer.htm"; }
};


struct ProgramTestChess : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestChess20120209.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return "ToolTestChess20120207.png"; }
  std::string GetScreenName() const noexcept override final { return "TestChess"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::wip; }
  ProgramType GetType() const noexcept override final { return ProgramType::testChess; }
  std::string GetUrl() const noexcept override final { return "ToolTestChess.htm"; }
};

struct ProgramTestDial : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestDial_2_3.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return "ToolTestDial_1_1.png"; }
  std::string GetScreenName() const noexcept override final { return "TestDial"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::n_a; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::testDial; }
  std::string GetUrl() const noexcept override final { return "ToolTestDial.htm"; }
};

struct ProgramTestDrawCanvas : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestDrawCanvas_1_2.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestDrawCanvas"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testDrawCanvas; }
  std::string GetUrl() const noexcept override final { return "ToolTestDrawCanvas.htm"; }
};

struct ProgramTestEntrance : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return "ToolTestEntrance_1_0.png"; }
  std::string GetScreenName() const noexcept override final { return "TestEntrance"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::n_a; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::n_a; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::testEntrance; }
  std::string GetUrl() const noexcept override final { return "ToolTestEntrance.htm"; }
};

struct ProgramTestExercise : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return "ToolTestExercise_1_0.png"; }
  std::string GetScreenName() const noexcept override final { return "TestExercise"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::unk; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::testExercise; }
  std::string GetUrl() const noexcept override final { return "ToolTestExercise.htm"; }
};

struct ProgramTestFunctionParser : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestFunctionParser_2_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return "ToolTestFunctionParser_1_0.png"; }
  std::string GetFilenameWeb() const noexcept override final { return "ToolTestFunctionParser_2_1.png"; }
  std::string GetScreenName() const noexcept override final { return "TestFunctionParser"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::testFunctionParser; }
  std::string GetUrl() const noexcept override final { return "ToolTestFunctionParser.htm"; }
};

struct ProgramTestGnuplotInterface : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestGnuplotInterface"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testGnuplotInterface; }
  std::string GetUrl() const noexcept override final { return "ToolTestGnuplotInterface.htm"; }
};

struct ProgramTestGraphicsProxyWidget : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestGraphicsProxyWidget_0_1.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestGraphicsProxyWidget"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testGraphicsProxyWidget; }
  std::string GetUrl() const noexcept override final { return "ToolTestGraphicsProxyWidget.htm"; }
};

struct ProgramTestGravityWidget : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestGravityWidget_0_1.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestGravityWidget"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testGravityWidget; }
  std::string GetUrl() const noexcept override final { return "ToolTestGravityWidget.htm"; }
};

struct ProgramTestGroupWidget : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return "ToolTestGroupWidget_1_0.png"; }
  std::string GetScreenName() const noexcept override final { return "TestGroupWidget"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::testGroupWidget; }
  std::string GetUrl() const noexcept override final { return "ToolTestGroupWidget.htm"; }
};

struct ProgramTestHugeVector : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestHugeVector"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testHugeVector; }
  std::string GetUrl() const noexcept override final { return "ToolTestHugeVector.htm"; }
};

struct ProgramTestImageCanvas : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestImageCanvas_1_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestImageCanvas"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testImageCanvas; }
  std::string GetUrl() const noexcept override final { return "ToolTestImageCanvas.htm"; }
};

struct ProgramTestLazy_init : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestLazy_init"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testLazy_init; }
  std::string GetUrl() const noexcept override final { return "ToolTestLazy_init.htm"; }
};

struct ProgramTestLed : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestLed_1_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return "ToolTestLed_1_1.png"; }
  std::string GetScreenName() const noexcept override final { return "TestLed"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::testLed; }
  std::string GetUrl() const noexcept override final { return "ToolTestLed.htm"; }
};

struct ProgramTestManyDigitNewick : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestManyDigitNewick_1_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestManyDigitNewick"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testManyDigitNewick; }
  std::string GetUrl() const noexcept override final { return "ToolTestManyDigitNewick.htm"; }
};

struct ProgramTestMultiApproximator : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestMultiApproximator_1_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestMultiApproximator"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testMultiApproximator; }
  std::string GetUrl() const noexcept override final { return "ToolTestMultiApproximator.htm"; }
};

struct ProgramTestMultiCanvas : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestMultiCanvas_1_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestMultiCanvas"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testMultiCanvas; }
  std::string GetUrl() const noexcept override final { return "ToolTestMultiCanvas.htm"; }
};

struct ProgramTestMultipleChoiceQuestion : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestMultipleChoiceQuestionDialog_1_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestMultipleChoiceQuestion"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testMultipleChoiceQuestion; }
  std::string GetUrl() const noexcept override final { return "ToolTestMultipleChoiceQuestion.htm"; }
};

struct ProgramTestMultiVector : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestMultiVector"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testMultiVector; }
  std::string GetUrl() const noexcept override final { return "ToolTestMultiVector.htm"; }
};

struct ProgramTestNdsmake : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestNdsmake_1_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestNdsmake"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testNdsmake; }
  std::string GetUrl() const noexcept override final { return "ToolTestNdsmake.htm"; }
};

struct ProgramTestNeuralNet : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestNeuralNet_0_1.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestNeuralNet"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testNeuralNet; }
  std::string GetUrl() const noexcept override final { return "ToolTestNeuralNet.htm"; }
};

struct ProgramTestNewick : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestNewick"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testNewick; }
  std::string GetUrl() const noexcept override final { return "ToolTestNewick.htm"; }
};

struct ProgramTestNewickVector : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestNewickVector_3_2.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestNewickVector"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testNewickVector; }
  std::string GetUrl() const noexcept override final { return "ToolTestNewickVector.htm"; }
};

struct ProgramTestOpenQuestion : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestOpenQuestion_1_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestOpenQuestion"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testOpenQuestion; }
  std::string GetUrl() const noexcept override final { return "ToolTestOpenQuestion.htm"; }
};

struct ProgramTestPolyFile : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestPolyFile"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testPolyFile; }
  std::string GetUrl() const noexcept override final { return "ToolTestPolyFile.htm"; }
};

struct ProgramTestPolyFileFromPolygons : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestPolyFileFromPolygons"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testPolyFileFromPolygons; }
  std::string GetUrl() const noexcept override final { return "ToolTestPolyFileFromPolygons.htm"; }
};

struct ProgramTestPylos : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestPylos_1_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestPylos"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testPylos; }
  std::string GetUrl() const noexcept override final { return "ToolTestPylos.htm"; }
};

struct ProgramTestQrcFile : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestQrcFile_1_3.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestQrcFile"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testQrcFile; }
  std::string GetUrl() const noexcept override final { return "ToolTestQrcFile.htm"; }
};

struct ProgramTestQtArrowItems : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestQtArrowItems_1_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestQtArrowItems"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testQtArrowItems; }
  std::string GetUrl() const noexcept override final { return "ToolTestQtArrowItems.htm"; }
};

struct ProgramTestQtCreatorProFile : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestQtCreatorProFile_1_4.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestQtCreatorProFile"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testQtCreatorProFile; }
  std::string GetUrl() const noexcept override final { return "ToolTestQtCreatorProFile.htm"; }
};

struct ProgramTestQtHideAndShowDialog : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestQtHideAndShowDialog_0_1.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestQtHideAndShowDialog"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testQtHideAndShowDialog; }
  std::string GetUrl() const noexcept override final { return "ToolTestQtHideAndShowDialog.htm"; }
};

struct ProgramTestQtKeyboardFriendlyGraphicsView : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestKeyboardFriendlyGraphicsView_0_4.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestQtKeyboardFriendlyGraphicsView"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testQtKeyboardFriendlyGraphicsView; }
  std::string GetUrl() const noexcept override final { return "ToolTestQtKeyboardFriendlyGraphicsView.htm"; }
};

struct ProgramTestQtModels : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestQtModels_1_2.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestQtModels"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testQtModels; }
  std::string GetUrl() const noexcept override final { return "ToolTestQtModels.htm"; }
};

struct ProgramTestQtOcrWidget : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestQtOcrWidget"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testQtOcrWidget; }
  std::string GetUrl() const noexcept override final { return "ToolTestQtOcrWidget.htm"; }
};

struct ProgramTestQtRoundedEditRectItem : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestQtRoundedEditRectItem_1_2.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestQtRoundedEditRectItem"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testQtRoundedEditRectItem; }
  std::string GetUrl() const noexcept override final { return "ToolTestQtRoundedEditRectItem.htm"; }
};

struct ProgramTestQtRoundedRectItem : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestQtRoundedRectItem_1_2.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestQtRoundedRectItem"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testQtRoundedRectItem; }
  std::string GetUrl() const noexcept override final { return "ToolTestQtRoundedRectItem.htm"; }
};

struct ProgramTestQuestion : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestQuestion_3_1.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestQuestion"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::unk; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::testQuestion; }
  std::string GetUrl() const noexcept override final { return "ToolTestQuestion.htm"; }
};

struct ProgramTestReversi : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestReversi_0_1.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestReversi"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testReversi; }
  std::string GetUrl() const noexcept override final { return "ToolTestReversi.htm"; }
};

struct ProgramTestSelectFileDialog : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return "ToolTestSelectFileDialog_1_1.png"; }
  std::string GetScreenName() const noexcept override final { return "TestSelectFileDialog"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::testSelectFileDialog; }
  std::string GetUrl() const noexcept override final { return "ToolTestSelectFileDialog.htm"; }
};

struct ProgramTestServerPusher : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestServerPusher"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::testServerPusher; }
  std::string GetUrl() const noexcept override final { return "ToolTestServerPusher.htm"; }
};

struct ProgramTestShape : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestShape_2_1.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return "ToolTestShape_1_0_web.png"; }
  std::string GetScreenName() const noexcept override final { return "TestShape"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::testShape; }
  std::string GetUrl() const noexcept override final { return "ToolTestShape.htm"; }
};

struct ProgramTestShinyButton : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestShinyButton_2_1.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return "ToolTestShinyButton_2_0_web.png"; }
  std::string GetScreenName() const noexcept override final { return "TestShinyButton"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::testShinyButton; }
  std::string GetUrl() const noexcept override final { return "ToolTestShinyButton.htm"; }
};

struct ProgramTestSimpleLinearRegression : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestSimpleLinearRegression_1_2.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestSimpleLinearRegression"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testSimpleLinearRegression; }
  std::string GetUrl() const noexcept override final { return "ToolTestSimpleLinearRegression.htm"; }
};

struct ProgramTestStopwatch : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestStopwatch"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testStopwatch; }
  std::string GetUrl() const noexcept override final { return "ToolTestStopwatch.htm"; }
};

struct ProgramTestTextCanvas : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestTextCanvas_1_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestTextCanvas"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testTextCanvas; }
  std::string GetUrl() const noexcept override final { return "ToolTestTextCanvas.htm"; }
};

struct ProgramTestTextPositionWidget : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestTextPositionWidget_1_4.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestTextPositionWidget"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testTextPositionWidget; }
  std::string GetUrl() const noexcept override final { return "ToolTestTextPositionWidget.htm"; }
};

struct ProgramTestTicTacToe : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestTicTacToe_1_1.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return "ToolTestTicTacToe_1_2.png"; }
  std::string GetScreenName() const noexcept override final { return "TestTicTacToe"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::testTicTacToe; }
  std::string GetUrl() const noexcept override final { return "ToolTestTicTacToe.htm"; }
};

struct ProgramTestTimedServerPusher : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestTimedServerPusher"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::testTimedServerPusher; }
  std::string GetUrl() const noexcept override final { return "ToolTestTimedServerPusher.htm"; }
};

struct ProgramTestToggleButton : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestToggleButton_1_4.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestToggleButton"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::testToggleButton; }
  std::string GetUrl() const noexcept override final { return "ToolTestToggleButton.htm"; }
};

struct ProgramTestTriangle : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestTriangle_1_1.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestTriangle"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testTriangle; }
  std::string GetUrl() const noexcept override final { return "ToolTestTriangle.htm"; }
};


struct ProgramTestTwoDigitNewick : public ProgramClass
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTestTwoDigitNewick_2_2.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TestTwoDigitNewick"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::testTwoDigitNewick; }
  std::string GetUrl() const noexcept override final { return "ToolTestTwoDigitNewick.htm"; }
};

struct ProgramThresholdFilterer : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolThresholdFilterer_2_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "ThresholdFilterer"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::thresholdFilterer; }
  std::string GetUrl() const noexcept override final { return "ToolThresholdFilterer.htm"; }
};

struct ProgramTicTacToe : public ProgramGame
{
  std::string GetFilenameConsole() const noexcept override final { return "GameTicTacToe_1_6.png"; }
  std::string GetFilenameDesktop() const noexcept override final { return "GameTicTacToe_1_5.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return "GameTicTacToe_1_4.png"; }
  std::string GetScreenName() const noexcept override final { return "TicTacToe"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::ticTacToe; }
  std::string GetUrl() const noexcept override final { return "GameTicTacToe.htm"; }
};

struct ProgramTimePoll : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return "ToolTimePoll_4_1.png"; }
  std::string GetScreenName() const noexcept override final { return "TimePoll"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::yes; }
  ProgramType GetType() const noexcept override final { return ProgramType::timePoll; }
  std::string GetUrl() const noexcept override final { return "ToolTimePoll.htm"; }
};

struct ProgramTriangleMeshCreator : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolTriangleMeshCreator_1_2.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TriangleMeshCreator"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::triangleMeshCreator; }
  std::string GetUrl() const noexcept override final { return "ToolTriangleMeshCreator.htm"; }
};

struct ProgramTronCollection : public ProgramGame
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "TronCollection"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::tronCollection; }
  std::string GetUrl() const noexcept override final { return "GameTronCollection.htm"; }
};

struct ProgramValentineCardDecrypter : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolValentineCardDecrypter_2_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "ValentineCardDecrypter"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::valentineCardDecrypter; }
  std::string GetUrl() const noexcept override final { return "ToolValentineCardDecrypter.htm"; }
};

struct ProgramVigenereCipher : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolVigenereCipher_1_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "VigenereCipher"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::wip; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::wip; }
  ProgramType GetType() const noexcept override final { return ProgramType::vigenereCipher; }
  std::string GetUrl() const noexcept override final { return "ToolVigenereCipher.htm"; }
};

struct ProgramVisualAbc : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolVisualAbc_1_0.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "VisualAbc"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::visualAbc; }
  std::string GetUrl() const noexcept override final { return "ToolVisualAbc.htm"; }
};

struct ProgramWktToSvg : public ProgramTool
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "ToolWktToSvg_1_3.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "WktToSvg"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::wktToSvg; }
  std::string GetUrl() const noexcept override final { return "ToolWktToSvg.htm"; }
};

struct ProgramXeNonZero : public ProgramGame
{
  std::string GetFilenameConsole() const noexcept override final { return ""; }
  std::string GetFilenameDesktop() const noexcept override final { return "GameXeNonZero_0_1.png"; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "XeNonZero"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::xeNonZero; }
  std::string GetUrl() const noexcept override final { return "GameXeNonZero.htm"; }
};

struct ProgramZork : public ProgramGame
{
  std::string GetFilenameConsole() const noexcept override final { return "GameZork.png"; }
  std::string GetFilenameDesktop() const noexcept override final { return ""; }
  std::string GetFilenameDesktopWindowsOnly() const noexcept override final { return ""; }
  std::string GetFilenameWeb() const noexcept override final { return ""; }
  std::string GetScreenName() const noexcept override final { return "Zork"; }
  ProgramStatus GetStatusConsole() const noexcept override final { return ProgramStatus::yes; }
  ProgramStatus GetStatusDesktopWindowsOnly() const noexcept override final { return ProgramStatus::nvr; }
  ProgramStatus GetStatusDesktop() const noexcept override final { return ProgramStatus::no; }
  ProgramStatus GetStatusWebApplication() const noexcept override final { return ProgramStatus::no; }
  ProgramType GetType() const noexcept override final { return ProgramType::zork; }
  std::string GetUrl() const noexcept override final { return "GameZork.htm"; }
};

std::ostream& operator<<(std::ostream& os, const Program& p) noexcept;
bool operator<(const Program& lhs, const Program& rhs) noexcept;

} //~namespace ribi

#endif // RIBI_RICHELBILDERBEEKPROGRAM_H
