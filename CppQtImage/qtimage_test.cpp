#include "qtimage_test.h"
#include "qtimage.h"

#include <QGraphicsSimpleTextItem>

void ribi::QtImageTest::all_tests()
{
  using namespace ribi;
  //if (verbose) { TRACE("Default-construction of QtImage"); }
  {
    const QtImage q;
  }
  //if (verbose) { TRACE("QImage pixel manipultion"); }
  {
    QImage q(1,1,QImage::Format_RGB32);
    q.setPixel(0,0,qRgb(1,2,3));
    QVERIFY(q.pixel(0,0) == qRgb(1,2,3));
  }
  //if (verbose) { TRACE("QGraphicsItem rendering"); }
  {
    QGraphicsSimpleTextItem item;
    item.setText(__func__);
    QImage image{QtImage().Paint(item)};
    QVERIFY(image.width() > 1);
    QVERIFY(image.height() > 1);
    image.save("tmp_ribi_QtImage_Test.png");
  }
}
