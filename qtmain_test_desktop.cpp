#include <cassert>
#include <iostream>
#include <QtTest/QtTest>
#include "grabber_test.h"
#include "qtarrowitem_test.h"
#include "qtkeyboardfriendlygraphicsview_test.h"
#include "qtdisplaypositem_test.h"
#include "qthideandshowdialog_test.h"
#include "qtscopeddisable_test.h"
#include "qtgraphics_test.h"
#include "qtimage_test.h"
#include "qtquadbezierarrowdialog_test.h"
#include "qtquadbezierarrowitem_test.h"
#include "qtroundededitrectitem_test.h"
#include "qtroundedrectitem_test.h"
#include "qtroundedrectitemdialog_test.h"

using namespace ribi;

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  int error = 0;
  //I am most interested in:
  //{ QtQuadBezierArrowItemTest t; error |= QTest::qExec(&t, argc, argv); }
  //assert(!"Don't forget to let Travis test everything");

  //These are all tests in alphabetical order
  { GrabberTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtArrowItemTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtDisplayPosItemTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtGraphicsTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtHideAndShowDialogTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtImageTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtKeyboardFriendlyGraphicsViewTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtQuadBezierArrowDialogTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtQuadBezierArrowItemTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtRoundedEditRectItemTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtRoundedRectItemDialogTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtRoundedRectItemTest t; error |= QTest::qExec(&t, argc, argv); }
  { QtScopedDisableTest t; error |= QTest::qExec(&t, argc, argv); }


  if (error == 0) { std::cout << "Pass\n"; }
  if (error != 0) { std::cout << "Fail\n"; }
  return error;
}
