//---------------------------------------------------------------------------
/*
RichelBilderbeek::ProgramType, enum for a program by Richel Bilderbeek
Copyright (C 2013-2015 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/ProjectRichelBilderbeek.htm
//---------------------------------------------------------------------------
#ifndef RIBI_RICHELBILDERBEEKPROGRAMTYPE_H
#define RIBI_RICHELBILDERBEEKPROGRAMTYPE_H

#include <vector>

namespace ribi {

///ProgramTypes works with ProgramType
enum class ProgramType
{
  asciiArter,
  beerWanter,
  boenken,
  brainweaver,
  caesarCipher,
  codeToHtml,
  connectThree,
  corridor,
  createGlossary,
  createQtProjectZipFile,
  dasWahreSchlagerfest,
  dotMatrix,
  encranger,
  filterOperationer,
  fisherWrighter,
  functionPlotter,
  gaborFilter,
  grayCoder,
  gtst,
  histogramEqualizationer,
  hometrainer,
  imageRotater,
  k3OpEenRij,
  kalmanFilterer,
  keySender,
  kTouchCppLessonsCreator,
  lizardPaperRockScissorsSpock,
  logisticGrowthSimulator,
  mazeCreator,
  maziak,
  morpher,
  multiEncranger,
  musicTheory,
  mutualismBreakdowner,
  ndsmake,
  paperRockScissors,
  paperRockScissorsWithTrait,
  pause,
  perfectElasticCollision,
  picToCode,
  pixelator,
  preDickAdvocaTor,
  primeExpert,
  projectRichelBilderbeek,
  pylos,
  qmakeWatcher,
  quadraticSolver,
  rampalEtienne,
  randomCode,
  regexTester,
  reversi,
  richelBilderbeekGallery,
  richelbilderbeekNlSitemapGenerator,
  rubiksClock,
  searchAndDestroyChess,
  secretMessage,
  simMysteryMachine,
  simplifyNewick,
  simTopPredatorPrey,
  solvePuzzleX,
  stateObserver,
  stochasticityInspector,
  styleSheetSetter,
  surfacePlotter,
  tankBattalion,
  testAbout,
  testApproximator,
  testBinaryNewickVector,
  testBouncingBallsWidget,
  testBouncingRectsWidget,
  testBroadcastServer,
  testChess,
  testDial,
  testDrawCanvas,
  testEntrance,
  testExercise,
  testFunctionParser,
  testGnuplotInterface,
  testGraphicsProxyWidget,
  testGravityWidget,
  testGroupWidget,
  testHugeVector,
  testImageCanvas,
  testLazy_init,
  testLed,
  testManyDigitNewick,
  testMultiApproximator,
  testMultiCanvas,
  testMultipleChoiceQuestion,
  testMultiVector,
  testNdsmake,
  testNeuralNet,
  testNewick,
  testNewickVector,
  testOpenQuestion,
  testPlane,
  testPolyFile,
  testPolyFileFromPolygons,
  testPylos,
  testQrcFile,
  testQtArrowItems,
  testQtCreatorProFile,
  testQtHideAndShowDialog,
  testQtKeyboardFriendlyGraphicsView,
  testQtModels,
  testQtOcrWidget,
  testQtRoundedEditRectItem,
  testQtRoundedRectItem,
  testQuestion,
  testReversi,
  testSelectFileDialog,
  testServerPusher,
  testShape,
  testShinyButton,
  testSimpleLinearRegression,
  testStopwatch,
  testTextCanvas,
  testTextPositionWidget,
  testTicTacToe,
  testTimedServerPusher,
  testToggleButton,
  testTriangle,
  testTwoDigitNewick,
  thresholdFilterer,
  ticTacToe,
  timePoll,
  triangleMeshCreator,
  tronCollection,
  valentineCardDecrypter,
  vigenereCipher,
  visualAbc,
  wktToSvg,
  xeNonZero,
  zork,

  n_types //Used for debugging
};

} //~namespace ribi

#endif // RIBI_RICHELBILDERBEEKPROGRAMTYPE_H
