#include "qtscopeddisable_test.h"
#include "qtscopeddisable.h"
#include <QDialog>

void ribi::QtScopedDisableTest::default_construct()
{
  QDialog * const dialog{new QDialog};
  QVERIFY(dialog->isEnabled());
  {
    QtScopedDisable<QDialog> disabler(dialog);
    QVERIFY(!disabler.GetVersion().empty());
    QVERIFY(!disabler.GetVersionHistory().empty());
    QVERIFY(!dialog->isEnabled());
  }
  QVERIFY(dialog->isEnabled());
}
