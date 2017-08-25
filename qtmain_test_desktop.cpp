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

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  int error = 0;
  //I am most interested in:
  //{ ribi::QtKeyboardFriendlyGraphicsViewTest t; error |= QTest::qExec(&t, argc, argv); }
  //{ ribi::QtRoundedRectItemTest t; error |= QTest::qExec(&t, argc, argv); }
  //assert(1==2);

  //These are all tests in alphabetical order
  { ribi::GrabberTest t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::QtArrowItemTest t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::QtDisplayPosItemTest t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::qtgraphics_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::qthideandshowdialog_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::qtimage_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::QtKeyboardFriendlyGraphicsViewTest t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::qtquadbezierarrowdialog_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::qtquadbezierarrowitem_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::qtroundededitrectitem_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::qtroundedrectitemdialog_test t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::QtRoundedRectItemTest t; error |= QTest::qExec(&t, argc, argv); }
  { ribi::qtscopeddisable_test t; error |= QTest::qExec(&t, argc, argv); }


  if (error == 0) { std::cout << "Pass\n"; }
  if (error != 0) { std::cout << "Fail\n"; }
  return error;
}
