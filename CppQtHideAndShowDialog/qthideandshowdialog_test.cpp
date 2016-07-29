#include "qthideandshowdialog_test.h"

#include "qthideandshowdialog.h"

void ribi::qthideandshowdialog_test::default_construct()
{
  QtHideAndShowDialog d;
  QVERIFY(!d.GetVersion().empty());
  QVERIFY(!d.GetVersionHistory().empty());
}
