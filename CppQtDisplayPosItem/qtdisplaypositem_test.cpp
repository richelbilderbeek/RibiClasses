#include "qtdisplaypositem_test.h"
#include "qtdisplaypositem.h"

void ribi::QtDisplayPosItemTest::default_construct()
{
  QtDisplayPosItem i;
  QVERIFY(!i.GetVersion().empty());
  QVERIFY(!i.GetVersionHistory().empty());
}

