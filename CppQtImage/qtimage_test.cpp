#include "qtimage.h"
#include <boost/test/unit_test.hpp>
#include <QGraphicsSimpleTextItem>

BOOST_AUTO_TEST_CASE(ribi_qtimage_test)
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
    BOOST_CHECK(q.pixel(0,0) == qRgb(1,2,3));
  }
  //if (verbose) { TRACE("QGraphicsItem rendering"); }
  {
    QGraphicsSimpleTextItem item;
    item.setText(__func__);
    QImage image{QtImage().Paint(item)};
    BOOST_CHECK(image.width() > 1);
    BOOST_CHECK(image.height() > 1);
    image.save("tmp_ribi_QtImage_Test.png");
  }
}
