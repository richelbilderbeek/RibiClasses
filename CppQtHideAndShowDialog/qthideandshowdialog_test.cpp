#include "qthideandshowdialog_test.h"

#include <memory>
#include "qthideandshowdialog.h"
#include "qthideandshowandthrowdialog.h"

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
  std::unique_ptr<QTimer> timer(new QTimer);
  timer->setInterval(1000);
  QObject::connect(timer.get(), SIGNAL(timeout()), &kid, SLOT(close()));
  timer->start();
  parent.ShowChild(&kid);
}

void ribi::qthideandshowdialog_test::parent_can_show_potentially_throwing_child()
{
  QtHideAndShowDialog parent;
  QtHideAndShowAndThrowDialog kid;
  parent.show();
  QVERIFY(parent.isVisible());
  QVERIFY(!kid.isVisible());
  std::unique_ptr<QTimer> timer(new QTimer);
  timer->setInterval(1000);
  QObject::connect(timer.get(), SIGNAL(timeout()), &kid, SLOT(close()));
  timer->start();
  parent.ShowChild(&kid);
}

void ribi::qthideandshowdialog_test::destroy_cleanly_upon_exception_in_derived_class()
{
  if ("this will freeze") return;
  QtHideAndShowDialog parent;
  QtHideAndShowAndThrowDialog kid;
  parent.show();
  QVERIFY(parent.isVisible());
  QVERIFY(!kid.isVisible());

  //Make the kid throw an exception after one second
  std::unique_ptr<QTimer> timer(new QTimer);
  timer->setInterval(1000);
  QObject::connect(timer.get(), SIGNAL(timeout()), &kid, SLOT(do_throw()));
  timer->start();

  try
  {
    parent.ShowChild(&kid);
    QVERIFY(!"Should not get here");
  }
  catch (std::exception& )
  {
    //OK
    QVERIFY("Should get here");
  }
}
