#include "qtdisplaypositem_test.h"
#include "qtdisplaypositem.h"

void ribi::qtdisplaypositem_test::default_construct()
{
  QtDisplayPosItem i;
  QVERIFY(!i.GetVersion().empty());
  QVERIFY(!i.GetVersionHistory().empty());
}

