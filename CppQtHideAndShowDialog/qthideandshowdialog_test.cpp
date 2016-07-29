#include "qthideandshowdialog_test.h"

#include "qthideandshowdialog.h"

void ribi::qthideandshowdialog_test::default_construct()
{
  QtHideAndShowDialog d;
  QVERIFY(!d.GetVersion().empty());
  QVERIFY(!d.GetVersionHistory().empty());
}

void ribi::qthideandshowdialog_test::parent_can_show_child()
{
  QtHideAndShowDialog parent;
  QtHideAndShowDialog kid;
  parent.show();
  QVERIFY(parent.isVisible());
  QVERIFY(!kid.isVisible());
  QTimer * const timer{new QTimer};
  timer->setInterval(1000);
  QObject::connect(timer, SIGNAL(timeout()), &kid, SLOT(close()));
  timer->start();
  parent.ShowChild(&kid);

  //Close
  //QTest::keyClick(&kid, Qt::Key_Escape, Qt::NoModifier, 100);
}
