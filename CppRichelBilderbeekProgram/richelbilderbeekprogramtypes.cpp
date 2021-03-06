//---------------------------------------------------------------------------
/*
RichelBilderbeek::ProgramType, enum for a program by Richel Bilderbeek
Copyright (C) 2013-2015 Richel Bilderbeek

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
//From http://www.richelbilderbeek.nl/CppProjectRichelBilderbeek.htm
//---------------------------------------------------------------------------
#include "richelbilderbeekprogramtypes.h"

#include "richelbilderbeekprogramtype.h"

#include <cassert>

#include "testtimer.h"
#include "trace.h"

//Lazy initializion
boost::bimap<ribi::ProgramType,std::string> ribi::ProgramTypes::m_map_to_enumname {};
//Lazy initializion
boost::bimap<ribi::ProgramType,std::string> ribi::ProgramTypes::m_map_to_screenname {};

ribi::ProgramTypes::ProgramTypes()
{
  #ifndef NDEBUG
  Test();
  #endif
}

boost::bimap<ribi::ProgramType,std::string> ribi::ProgramTypes::CreateEnumNameMap()
{
  #ifndef NDEBUG
  Test();
  #endif

  boost::bimap<ProgramType,std::string> m;
  typedef boost::bimap<ProgramType,std::string>::value_type Pair;
  ///Generated with Gnumeric:
  /// =CONCATENATE("m.insert(Pair(ProgramType::",A1,",\"",A1,"\"));")
  ///where A1 to Ax contain the programType enum values
  m.insert(Pair(ProgramType::asciiArter,"asciiArter"));
  m.insert(Pair(ProgramType::beerWanter,"beerWanter"));
  m.insert(Pair(ProgramType::boenken,"boenken"));
  m.insert(Pair(ProgramType::brainweaver,"brainweaver"));
  m.insert(Pair(ProgramType::caesarCipher,"caesarCipher"));
  m.insert(Pair(ProgramType::codeToHtml,"codeToHtml"));
  m.insert(Pair(ProgramType::connectThree,"connectThree"));
  m.insert(Pair(ProgramType::corridor,"corridor"));
  m.insert(Pair(ProgramType::createGlossary,"createGlossary"));
  m.insert(Pair(ProgramType::createQtProjectZipFile,"createQtProjectZipFile"));
  m.insert(Pair(ProgramType::dasWahreSchlagerfest,"dasWahreSchlagerfest"));
  m.insert(Pair(ProgramType::dotMatrix,"dotMatrix"));
  m.insert(Pair(ProgramType::encranger,"encranger"));
  m.insert(Pair(ProgramType::filterOperationer,"filterOperationer"));
  m.insert(Pair(ProgramType::fisherWrighter,"fisherWrighter"));
  m.insert(Pair(ProgramType::functionPlotter,"functionPlotter"));
  m.insert(Pair(ProgramType::gaborFilter,"gaborFilter"));
  m.insert(Pair(ProgramType::grayCoder,"grayCoder"));
  m.insert(Pair(ProgramType::gtst,"gtst"));
  m.insert(Pair(ProgramType::histogramEqualizationer,"histogramEqualizationer"));
  m.insert(Pair(ProgramType::hometrainer,"hometrainer"));
  m.insert(Pair(ProgramType::imageRotater,"imageRotater"));
  m.insert(Pair(ProgramType::k3OpEenRij,"k3OpEenRij"));
  m.insert(Pair(ProgramType::kalmanFilterer,"kalmanFilterer"));
  m.insert(Pair(ProgramType::keySender,"keySender"));
  m.insert(Pair(ProgramType::kTouchCppLessonsCreator,"kTouchLectureCreator"));
  m.insert(Pair(ProgramType::lizardPaperRockScissorsSpock,"lizardPaperRockScissorsSpock"));
  m.insert(Pair(ProgramType::logisticGrowthSimulator,"logisticGrowthSimulator"));
  m.insert(Pair(ProgramType::mazeCreator,"mazeCreator"));
  m.insert(Pair(ProgramType::maziak,"maziak"));
  m.insert(Pair(ProgramType::multiEncranger,"multiEncranger"));
  m.insert(Pair(ProgramType::mutualismBreakdowner,"mutualismBreakdowner"));
  m.insert(Pair(ProgramType::ndsmake,"ndsmake"));
  m.insert(Pair(ProgramType::paperRockScissors,"paperRockScissors"));
  m.insert(Pair(ProgramType::paperRockScissorsWithTrait,"paperRockScissorsWithTrait"));
  m.insert(Pair(ProgramType::pause,"pause"));
  m.insert(Pair(ProgramType::perfectElasticCollision,"perfectElasticCollision"));
  m.insert(Pair(ProgramType::picToCode,"picToCode"));
  m.insert(Pair(ProgramType::pixelator,"pixelator"));
  m.insert(Pair(ProgramType::preDickAdvocaTor,"preDickAdvocaTor"));
  m.insert(Pair(ProgramType::primeExpert,"primeExpert"));
  m.insert(Pair(ProgramType::projectRichelBilderbeek,"projectRichelBilderbeek"));
  m.insert(Pair(ProgramType::pylos,"pylos"));
  m.insert(Pair(ProgramType::qmakeWatcher,"qmakeWatcher"));
  m.insert(Pair(ProgramType::quadraticSolver,"quadraticSolver"));
  m.insert(Pair(ProgramType::rampalEtienne,"rampalEtienne"));
  m.insert(Pair(ProgramType::randomCode,"randomCode"));
  m.insert(Pair(ProgramType::regexTester,"regexTester"));
  m.insert(Pair(ProgramType::reversi,"reversi"));
  m.insert(Pair(ProgramType::richelBilderbeekGallery,"richelBilderbeekGallery"));
  m.insert(Pair(ProgramType::richelbilderbeekNlSitemapGenerator,"richelbilderbeekNlSitemapGenerator"));
  m.insert(Pair(ProgramType::rubiksClock,"rubiksClock"));
  m.insert(Pair(ProgramType::searchAndDestroyChess,"searchAndDestroyChess"));
  m.insert(Pair(ProgramType::secretMessage,"secretMessage"));
  m.insert(Pair(ProgramType::simMysteryMachine,"simMysteryMachine"));
  m.insert(Pair(ProgramType::simplifyNewick,"simplifyNewick"));
  m.insert(Pair(ProgramType::simTopPredatorPrey,"simTopPredatorPrey"));
  m.insert(Pair(ProgramType::solvePuzzleX,"solvePuzzleX"));
  m.insert(Pair(ProgramType::stateObserver,"stateObserver"));
  m.insert(Pair(ProgramType::stochasticityInspector,"stochasticityInspector"));
  m.insert(Pair(ProgramType::styleSheetSetter,"styleSheetSetter"));
  m.insert(Pair(ProgramType::surfacePlotter,"surfacePlotter"));
  m.insert(Pair(ProgramType::tankBattalion,"tankBattalion"));
  m.insert(Pair(ProgramType::testAbout,"testAbout"));
  m.insert(Pair(ProgramType::testApproximator,"testApproximator"));
  m.insert(Pair(ProgramType::testBinaryNewickVector,"testBinaryNewickVector"));
  m.insert(Pair(ProgramType::testBouncingBallsWidget,"testBouncingBallsWidget"));
  m.insert(Pair(ProgramType::testBouncingRectsWidget,"testBouncingRectsWidget"));
  m.insert(Pair(ProgramType::testBroadcastServer,"testBroadcastServer"));
  m.insert(Pair(ProgramType::testChess,"testChess"));
  m.insert(Pair(ProgramType::testDial,"testDial"));
  m.insert(Pair(ProgramType::testDrawCanvas,"testDrawCanvas"));
  m.insert(Pair(ProgramType::testEntrance,"testEntrance"));
  m.insert(Pair(ProgramType::testExercise,"testExercise"));
  m.insert(Pair(ProgramType::testFunctionParser,"testFunctionParser"));
  m.insert(Pair(ProgramType::testGnuplotInterface,"testGnuplotInterface"));
  m.insert(Pair(ProgramType::testGraphicsProxyWidget,"testGraphicsProxyWidget"));
  m.insert(Pair(ProgramType::testGravityWidget,"testGravityWidget"));
  m.insert(Pair(ProgramType::testGroupWidget,"testGroupWidget"));
  m.insert(Pair(ProgramType::testHugeVector,"testHugeVector"));
  m.insert(Pair(ProgramType::testImageCanvas,"testImageCanvas"));
  m.insert(Pair(ProgramType::testLazy_init,"testLazy_init"));
  m.insert(Pair(ProgramType::testLed,"testLed"));
  m.insert(Pair(ProgramType::testManyDigitNewick,"testManyDigitNewick"));
  m.insert(Pair(ProgramType::testMultiApproximator,"testMultiApproximator"));
  m.insert(Pair(ProgramType::testMultiCanvas,"testMultiCanvas"));
  m.insert(Pair(ProgramType::testMultipleChoiceQuestion,"testMultipleChoiceQuestion"));
  m.insert(Pair(ProgramType::testMultiVector,"testMultiVector"));
  m.insert(Pair(ProgramType::testNdsmake,"testNdsmake"));
  m.insert(Pair(ProgramType::testNeuralNet,"testNeuralNet"));
  m.insert(Pair(ProgramType::testNewick,"testNewick"));
  m.insert(Pair(ProgramType::testNewickVector,"testNewickVector"));
  m.insert(Pair(ProgramType::testOpenQuestion,"testOpenQuestion"));
  m.insert(Pair(ProgramType::testPlane,"testPlane"));
  m.insert(Pair(ProgramType::testPolyFile,"testPolyFile"));
  m.insert(Pair(ProgramType::testPolyFileFromPolygons,"testPolyFileFromPolygons"));
  m.insert(Pair(ProgramType::testPylos,"testPylos"));
  m.insert(Pair(ProgramType::testQrcFile,"testQrcFile"));
  m.insert(Pair(ProgramType::testQtArrowItems,"testQtArrowItems"));
  m.insert(Pair(ProgramType::testQtCreatorProFile,"testQtCreatorProFile"));
  m.insert(Pair(ProgramType::testQtHideAndShowDialog,"testQtHideAndShowDialog"));
  m.insert(Pair(ProgramType::testQtKeyboardFriendlyGraphicsView,"testQtKeyboardFriendlyGraphicsView"));
  m.insert(Pair(ProgramType::testQtModels,"testQtModels"));
  m.insert(Pair(ProgramType::testQtOcrWidget,"testQtOcrWidget"));
  m.insert(Pair(ProgramType::testQtRoundedEditRectItem,"testQtRoundedEditRectItem"));
  m.insert(Pair(ProgramType::testQtRoundedRectItem,"testQtRoundedRectItem"));
  m.insert(Pair(ProgramType::testQtRoundedEditRectItem,"testQtRoundedEditRectItem"));
  m.insert(Pair(ProgramType::testQuestion,"testQuestion"));
  m.insert(Pair(ProgramType::testReversi,"testReversi"));
  m.insert(Pair(ProgramType::testSelectFileDialog,"testSelectFileDialog"));
  m.insert(Pair(ProgramType::testServerPusher,"testServerPusher"));
  m.insert(Pair(ProgramType::testShape,"testShape"));
  m.insert(Pair(ProgramType::testShinyButton,"testShinyButton"));
  m.insert(Pair(ProgramType::testSimpleLinearRegression,"testSimpleLinearRegression"));
  m.insert(Pair(ProgramType::testStopwatch,"testStopwatch"));
  m.insert(Pair(ProgramType::testTextCanvas,"testTextCanvas"));
  m.insert(Pair(ProgramType::testTextPositionWidget,"testTextPositionWidget"));
  m.insert(Pair(ProgramType::testTicTacToe,"testTicTacToe"));
  m.insert(Pair(ProgramType::testTimedServerPusher,"testTimedServerPusher"));
  m.insert(Pair(ProgramType::testToggleButton,"testToggleButton"));
  m.insert(Pair(ProgramType::testTriangle,"testTriangle"));
  m.insert(Pair(ProgramType::testTwoDigitNewick,"testTwoDigitNewick"));
  m.insert(Pair(ProgramType::thresholdFilterer,"thresholdFilterer"));
  m.insert(Pair(ProgramType::ticTacToe,"ticTacToe"));
  m.insert(Pair(ProgramType::timePoll,"timePoll"));
  m.insert(Pair(ProgramType::triangleMeshCreator,"triangleMeshCreator"));
  m.insert(Pair(ProgramType::tronCollection,"tronCollection"));
  m.insert(Pair(ProgramType::valentineCardDecrypter,"valentineCardDecrypter"));
  m.insert(Pair(ProgramType::vigenereCipher,"vigenereCipher"));
  m.insert(Pair(ProgramType::visualAbc,"visualAbc"));
  m.insert(Pair(ProgramType::wktToSvg,"wktToSvg"));
  m.insert(Pair(ProgramType::xeNonZero,"xeNonZero"));
  m.insert(Pair(ProgramType::zork,"zork"));
  assert(m.left.size() == m.right.size());
  #ifndef NDEBUG
  if (static_cast<int>(m.left.size()) != static_cast<int>(ProgramType::n_types))
  {
    TRACE(m.left.size());
    TRACE(static_cast<int>(ProgramType::n_types));
    TRACE("Copy the contents of ProgramType (except n_types) to above here");
  }
  #endif
  assert(static_cast<int>(m.left.size()) == static_cast<int>(ProgramType::n_types));
  return m;
}

boost::bimap<ribi::ProgramType,std::string> ribi::ProgramTypes::CreateScreenNameMap()
{
  #ifndef NDEBUG
  Test();
  #endif

  boost::bimap<ProgramType,std::string> m;
  typedef boost::bimap<ProgramType,std::string>::value_type Pair;
  ///Generated with Gnumeric:
  /// =CONCATENATE("m.insert(Pair(ProgramType::",A1,",\"",A1,"\"));")
  ///where A1 to Ax contain the programType enum values
  m.insert(Pair(ProgramType::asciiArter,"AsciiArter"));
  m.insert(Pair(ProgramType::beerWanter,"BeerWanter"));
  m.insert(Pair(ProgramType::boenken,"Boenken"));
  m.insert(Pair(ProgramType::brainweaver,"Brainweaver"));
  m.insert(Pair(ProgramType::caesarCipher,"CaesarCipher"));
  m.insert(Pair(ProgramType::codeToHtml,"CodeToHtml"));
  m.insert(Pair(ProgramType::connectThree,"ConnectThree"));
  m.insert(Pair(ProgramType::corridor,"Corridor"));
  m.insert(Pair(ProgramType::createGlossary,"CreateGlossary"));
  m.insert(Pair(ProgramType::createQtProjectZipFile,"CreateQtProjectZipFile"));
  m.insert(Pair(ProgramType::dasWahreSchlagerfest,"Das Wahre Schlagerfest"));
  m.insert(Pair(ProgramType::dotMatrix,"DotMatrix"));
  m.insert(Pair(ProgramType::encranger,"Encranger"));
  m.insert(Pair(ProgramType::filterOperationer,"FilterOperationer"));
  m.insert(Pair(ProgramType::fisherWrighter,"FisherWrighter"));
  m.insert(Pair(ProgramType::functionPlotter,"FunctionPlotter"));
  m.insert(Pair(ProgramType::gaborFilter,"GaborFilter"));
  m.insert(Pair(ProgramType::grayCoder,"GrayCoder"));
  m.insert(Pair(ProgramType::gtst,"GTST"));
  m.insert(Pair(ProgramType::histogramEqualizationer,"HistogramEqualizationer"));
  m.insert(Pair(ProgramType::hometrainer,"Hometrainer"));
  m.insert(Pair(ProgramType::imageRotater,"ImageRotater"));
  m.insert(Pair(ProgramType::k3OpEenRij,"K3 Op Een Rij"));
  m.insert(Pair(ProgramType::kalmanFilterer,"KalmanFilterer"));
  m.insert(Pair(ProgramType::keySender,"KeySender"));
  m.insert(Pair(ProgramType::kTouchCppLessonsCreator,"KTouch Lecture Creator"));
  m.insert(Pair(ProgramType::lizardPaperRockScissorsSpock,"LizardPaperRockScissorsSpock"));
  m.insert(Pair(ProgramType::logisticGrowthSimulator,"LogisticGrowthSimulator"));
  m.insert(Pair(ProgramType::mazeCreator,"MazeCreator"));
  m.insert(Pair(ProgramType::maziak,"Maziak"));
  m.insert(Pair(ProgramType::morpher,"Morpher"));
  m.insert(Pair(ProgramType::multiEncranger,"MultiEncranger"));
  m.insert(Pair(ProgramType::musicTheory,"MusicTheory"));
  m.insert(Pair(ProgramType::mutualismBreakdowner,"MutualismBreakdowner"));
  m.insert(Pair(ProgramType::ndsmake,"ndsmake"));
  m.insert(Pair(ProgramType::paperRockScissors,"PaperRockScissors"));
  m.insert(Pair(ProgramType::paperRockScissorsWithTrait,"PaperRockScissorsWithTrait"));
  m.insert(Pair(ProgramType::pause,"Pause"));
  m.insert(Pair(ProgramType::perfectElasticCollision,"PerfectElasticCollision"));
  m.insert(Pair(ProgramType::picToCode,"PicToCode"));
  m.insert(Pair(ProgramType::pixelator,"Pixelator"));
  m.insert(Pair(ProgramType::preDickAdvocaTor,"PreDickAdvocaTor"));
  m.insert(Pair(ProgramType::primeExpert,"PrimeExpert"));
  m.insert(Pair(ProgramType::projectRichelBilderbeek,"ProjectRichelBilderbeek"));
  m.insert(Pair(ProgramType::pylos,"Pylos"));
  m.insert(Pair(ProgramType::qmakeWatcher,"qmake Watcher"));
  m.insert(Pair(ProgramType::quadraticSolver,"QuadraticSolver"));
  m.insert(Pair(ProgramType::rampalEtienne,"RampalEtienne"));
  m.insert(Pair(ProgramType::randomCode,"RandomCode"));
  m.insert(Pair(ProgramType::regexTester,"RegexTester"));
  m.insert(Pair(ProgramType::reversi,"Reversi"));
  m.insert(Pair(ProgramType::richelBilderbeekGallery,"RichelBilderbeekGallery"));
  m.insert(Pair(ProgramType::richelbilderbeekNlSitemapGenerator,"RichelbilderbeekNlSitemapGenerator"));
  m.insert(Pair(ProgramType::rubiksClock,"Rubik's Clock"));
  m.insert(Pair(ProgramType::searchAndDestroyChess,"SearchAndDestroyChess"));
  m.insert(Pair(ProgramType::secretMessage,"SecretMessage"));
  m.insert(Pair(ProgramType::simMysteryMachine,"SimMysteryMachine"));
  m.insert(Pair(ProgramType::simplifyNewick,"SimplifyNewick"));
  m.insert(Pair(ProgramType::simTopPredatorPrey,"SimTopPredatorPrey"));
  m.insert(Pair(ProgramType::solvePuzzleX,"SolvePuzzleX"));
  m.insert(Pair(ProgramType::stateObserver,"StateObserver"));
  m.insert(Pair(ProgramType::stochasticityInspector,"StochasticityInspector"));
  m.insert(Pair(ProgramType::styleSheetSetter,"StyleSheetSetter"));
  m.insert(Pair(ProgramType::surfacePlotter,"SurfacePlotter"));
  m.insert(Pair(ProgramType::tankBattalion,"Tank Battalion"));
  m.insert(Pair(ProgramType::testAbout,"Test About"));
  m.insert(Pair(ProgramType::testApproximator,"Test Approximator"));
  m.insert(Pair(ProgramType::testBinaryNewickVector,"Test BinaryNewickVector"));
  m.insert(Pair(ProgramType::testBouncingBallsWidget,"Test BouncingBallsWidget"));
  m.insert(Pair(ProgramType::testBouncingRectsWidget,"Test BouncingRectsWidget"));
  m.insert(Pair(ProgramType::testBroadcastServer,"Test BroadcastServer"));
  m.insert(Pair(ProgramType::testChess,"Test Chess"));
  m.insert(Pair(ProgramType::testDial,"Test Dial"));
  m.insert(Pair(ProgramType::testDrawCanvas,"Test DrawCanvas"));
  m.insert(Pair(ProgramType::testEntrance,"Test Entrance"));
  m.insert(Pair(ProgramType::testExercise,"Test Exercise"));
  m.insert(Pair(ProgramType::testFunctionParser,"Test FunctionParser"));
  m.insert(Pair(ProgramType::testGnuplotInterface,"Test GnuplotInterface"));
  m.insert(Pair(ProgramType::testGraphicsProxyWidget,"Test GraphicsProxyWidget"));
  m.insert(Pair(ProgramType::testGravityWidget,"Test GravityWidget"));
  m.insert(Pair(ProgramType::testGroupWidget,"Test GroupWidget"));
  m.insert(Pair(ProgramType::testHugeVector,"Test HugeVector"));
  m.insert(Pair(ProgramType::testImageCanvas,"Test ImageCanvas"));
  m.insert(Pair(ProgramType::testLazy_init,"Test Lazy_init"));
  m.insert(Pair(ProgramType::testLed,"Test Led"));
  m.insert(Pair(ProgramType::testManyDigitNewick,"Test ManyDigitNewick"));
  m.insert(Pair(ProgramType::testMultiApproximator,"Test MultiApproximator"));
  m.insert(Pair(ProgramType::testMultiCanvas,"Test MultiCanvas"));
  m.insert(Pair(ProgramType::testMultipleChoiceQuestion,"Test MultipleChoiceQuestion"));
  m.insert(Pair(ProgramType::testMultiVector,"Test MultiVector"));
  m.insert(Pair(ProgramType::testNdsmake,"Test ndsmake"));
  m.insert(Pair(ProgramType::testNeuralNet,"Test NeuralNet"));
  m.insert(Pair(ProgramType::testNewick,"Test Newick"));
  m.insert(Pair(ProgramType::testNewickVector,"Test NewickVector"));
  m.insert(Pair(ProgramType::testOpenQuestion,"Test OpenQuestion"));
  m.insert(Pair(ProgramType::testPlane,"Test Plane"));
  m.insert(Pair(ProgramType::testPolyFile,"Test PolyFile"));
  m.insert(Pair(ProgramType::testPolyFileFromPolygons,"Test PolyFileFromPolygons"));
  m.insert(Pair(ProgramType::testPylos,"Test Pylos"));
  m.insert(Pair(ProgramType::testQrcFile,"Test QrcFile"));
  m.insert(Pair(ProgramType::testQtArrowItems,"Test QtArrowItems"));
  m.insert(Pair(ProgramType::testQtCreatorProFile,"Test QtCreatorProFile"));
  m.insert(Pair(ProgramType::testQtHideAndShowDialog,"Test QtHideAndShowDialog"));
  m.insert(Pair(ProgramType::testQtKeyboardFriendlyGraphicsView,"Test QtKeyboardFriendlyGraphicsView"));
  m.insert(Pair(ProgramType::testQtModels,"Test QtModels"));
  m.insert(Pair(ProgramType::testQtOcrWidget,"Test QtOcrWidget"));
  m.insert(Pair(ProgramType::testQtRoundedEditRectItem,"Test QtRoundedEditRectItem"));
  m.insert(Pair(ProgramType::testQtRoundedRectItem,"Test QtRoundedRectItem"));
  m.insert(Pair(ProgramType::testQtRoundedEditRectItem,"Test QtRoundedEditRectItem"));
  m.insert(Pair(ProgramType::testQuestion,"Test Question"));
  m.insert(Pair(ProgramType::testReversi,"Test Reversi"));
  m.insert(Pair(ProgramType::testSelectFileDialog,"Test SelectFileDialog"));
  m.insert(Pair(ProgramType::testServerPusher,"Test ServerPusher"));
  m.insert(Pair(ProgramType::testShape,"Test Shape"));
  m.insert(Pair(ProgramType::testShinyButton,"Test ShinyButton"));
  m.insert(Pair(ProgramType::testSimpleLinearRegression,"Test SimpleLinearRegression"));
  m.insert(Pair(ProgramType::testStopwatch,"Test Stopwatch"));
  m.insert(Pair(ProgramType::testTextCanvas,"Test TextCanvas"));
  m.insert(Pair(ProgramType::testTextPositionWidget,"Test TextPositionWidget"));
  m.insert(Pair(ProgramType::testTicTacToe,"Test TicTacToe"));
  m.insert(Pair(ProgramType::testTimedServerPusher,"Test TimedServerPusher"));
  m.insert(Pair(ProgramType::testToggleButton,"Test ToggleButton"));
  m.insert(Pair(ProgramType::testTriangle,"Test Triangle"));
  m.insert(Pair(ProgramType::testTwoDigitNewick,"Test TwoDigitNewick"));
  m.insert(Pair(ProgramType::thresholdFilterer,"ThresholdFilterer"));
  m.insert(Pair(ProgramType::ticTacToe,"TicTacToe"));
  m.insert(Pair(ProgramType::timePoll,"TimePoll"));
  m.insert(Pair(ProgramType::triangleMeshCreator,"TriangleMeshCreator"));
  m.insert(Pair(ProgramType::tronCollection,"Tron Collection"));
  m.insert(Pair(ProgramType::valentineCardDecrypter,"ValentineCardDecrypter"));
  m.insert(Pair(ProgramType::vigenereCipher,"VigenereCipher"));
  m.insert(Pair(ProgramType::visualAbc,"Visual ABC"));
  m.insert(Pair(ProgramType::wktToSvg,"WktToSvg"));
  m.insert(Pair(ProgramType::xeNonZero,"XeNonZero"));
  m.insert(Pair(ProgramType::zork,"Zork"));
  assert(m.left.size() == m.right.size());
  #ifndef NDEBUG
  if (static_cast<int>(m.left.size()) != static_cast<int>(ProgramType::n_types))
  {
    TRACE(m.left.size());
    TRACE(static_cast<int>(ProgramType::n_types));
    TRACE("Copy the contents of ProgramType (except n_types) to above here");
  }
  #endif
  assert(static_cast<int>(m.left.size()) == static_cast<int>(ProgramType::n_types));
  return m;
}

std::vector<std::string> ribi::ProgramTypes::GetAllEnumNames() const noexcept
{
  if (m_map_to_enumname.right.empty()) { m_map_to_enumname = CreateEnumNameMap(); }
  assert(!m_map_to_enumname.right.empty());

  //Copy right map of bimap
  std::vector<std::string> v;
  for (const auto& t: m_map_to_enumname.right) { v.push_back(t.first); }
  assert(!v.empty());
  assert(m_map_to_enumname.right.size() == v.size());
  return v;
}

std::vector<std::string> ribi::ProgramTypes::GetAllScreenNames() const noexcept
{
  if (m_map_to_screenname.right.empty()) { m_map_to_screenname = CreateScreenNameMap(); }
  assert(!m_map_to_screenname.right.empty());

  //Copy right map of bimap
  std::vector<std::string> v;
  for (const auto& t: m_map_to_screenname.right) { v.push_back(t.first); }
  assert(!v.empty());
  assert(m_map_to_screenname.right.size() == v.size());
  return v;
}

std::vector<ribi::ProgramType> ribi::ProgramTypes::GetAll() const noexcept
{
  if (m_map_to_enumname.left.empty()) { m_map_to_enumname = CreateEnumNameMap(); }
  assert(!m_map_to_enumname.left.empty());

  //Copy left map of bimap
  std::vector<ProgramType> v;
  for (const auto& t: m_map_to_enumname.left) { v.push_back(t.first); }
  assert(!v.empty());
  assert(m_map_to_enumname.left.size() == v.size());
  return v;
}

std::string ribi::ProgramTypes::ProgramTypeToEnumName(const ProgramType t) const noexcept
{
  if (m_map_to_enumname.left.empty()) { m_map_to_enumname = CreateEnumNameMap(); }
  assert(m_map_to_enumname.left.find(t) != m_map_to_enumname.left.end());
  return m_map_to_enumname.left.find(t)->second;
}

std::string ribi::ProgramTypes::ProgramTypeToScreenName(const ProgramType t) const noexcept
{
  if (m_map_to_screenname.left.empty()) { m_map_to_screenname = CreateEnumNameMap(); }
  assert(m_map_to_screenname.left.find(t) != m_map_to_screenname.left.end());
  return m_map_to_screenname.left.find(t)->second;
}

#ifndef NDEBUG
void ribi::ProgramTypes::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  const TestTimer test_timer(__func__,__FILE__,1.0);
  const ProgramTypes p;
  assert(p.GetAll().size() == p.GetAllEnumNames().size());
  assert(p.GetAll().size() == p.GetAllScreenNames().size());
}
#endif
