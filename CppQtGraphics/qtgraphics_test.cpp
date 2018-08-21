#include "qtgraphics_test.h"
#include "qtgraphics.h"
#include "stopwatch.h"

#include "trace.h"

void ribi::QtGraphicsTest::all_tests()
{
  using namespace ribi;
  const bool verbose{false};
  if (verbose) { TRACE("Default-construction of QtGraphics"); }
  {
    const QtGraphics q;
  }
  //CreateImage
  {
    const QImage a = QtGraphics().CreateImage(256,256,64);
    QVERIFY(!a.isNull());
  }
  {
    QImage target = QtGraphics().CreateImage(256,256,64);
    QVERIFY(!target.isNull());
    const QImage source = QtGraphics().CreateImage(196,156,196);
    QVERIFY(!source.isNull());
    QtGraphics().DrawImage(target,source,32,64);
  }
  {
    QImage target_slowest = QtGraphics().CreateImage(256,256,64);
    QImage target_slow    = QtGraphics().CreateImage(256,256,64);
    QImage target_fast    = QtGraphics().CreateImage(256,256,64);
    const QImage source   = QtGraphics().CreateImage(196,156,196);
    QtGraphics().DrawImage(       target_fast   ,source,32,64);
    QtGraphics().DrawImageSlow   (target_slow   ,source,32,64);
    QtGraphics().DrawImageSlowest(target_slowest,source,32,64);
    QVERIFY(target_fast == target_slow);
    QVERIFY(target_fast == target_slowest);
  }
  const bool do_timing{false};
  if (do_timing)
  {
    //TRACE 't_fast' line 188 in file '../../Classes/CppQtGraphics/qtgraphics.cpp': '0,032000000000000001'
    //TRACE 't_slow' line 189 in file '../../Classes/CppQtGraphics/qtgraphics.cpp': '0,51700000000000002'
    //TRACE 't_slowest' line 190 in file '../../Classes/CppQtGraphics/qtgraphics.cpp': '1,169'
    const int sz{5000};
    QImage target_fast    = QtGraphics().CreateImage(sz,sz);
    QImage target_slow    = QtGraphics().CreateImage(sz,sz);
    QImage target_slowest = QtGraphics().CreateImage(sz,sz);
    const QImage source   = QtGraphics().CreateImage(sz,sz);
    Stopwatch s_fast;
    QtGraphics().DrawImage(target_fast,source,0,0);
    const double t_fast{s_fast.GetElapsedSecs()};
    Stopwatch s_slow;
    QtGraphics().DrawImageSlow(target_slow,source,0,0);
    const double t_slow{s_slow.GetElapsedSecs()};
    Stopwatch s_slowest;
    QtGraphics().DrawImageSlowest(target_slowest,source,0,0);
    const double t_slowest{s_slowest.GetElapsedSecs()};
    TRACE(t_fast);
    TRACE(t_slow);
    TRACE(t_slowest);
    QVERIFY(t_fast < t_slow);
    QVERIFY(t_slow < t_slowest);
    QVERIFY(t_fast * 10.0 < t_slow);
  }
}
