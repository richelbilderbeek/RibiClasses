#include "grabber_test.h"
#include "grabber.h"

void ribi::grabber_test::can_default_construct()
{
  QVERIFY(!ribi::Grabber(0).GetVersion().empty());
}
