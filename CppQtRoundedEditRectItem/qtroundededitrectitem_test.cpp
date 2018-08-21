#include "qtroundededitrectitem_test.h"
#include "qtroundededitrectitem.h"
#include "counter.h"
#include "trace.h"

void ribi::QtRoundedEditRectItemTest::all_tests()
{
  using namespace ribi;
  //using namespace ribi::QtRoundedEditRectItem;
  const bool verbose{false};
  QtRoundedEditRectItem item;
  if (verbose) { TRACE("Set/Get Text must be symmetric"); }
  {
    const auto old_text = item.GetText();
    auto new_text = old_text;
    new_text[0] += " (modified)";
    item.SetText(new_text);
    QVERIFY(item.GetText() == new_text);
  }
  if (verbose) { TRACE("Setting two increasing-length single-line texts should increase the inner width"); }
  {
    item.SetText( {"A"} );
    const double old_width{item.GetInnerWidth()};
    item.SetText( {"AB"} );
    QVERIFY(old_width < item.GetInnerWidth());
  }
  if (verbose) { TRACE("Setting two decreasing-length single-line texts should decrease the inner width"); }
  {
    item.SetText( {"AB"} );
    const double old_width{item.GetInnerWidth()};
    item.SetText( {"A"} );
    QVERIFY(old_width > item.GetInnerWidth());
  }
  if (verbose) { TRACE("Setting two increasing-length single-line texts should keep the inner height the same"); }
  {
    item.SetText( {"A"} );
    const double old_height{item.GetInnerHeight()};
    item.SetText( {"AB"} );
    QVERIFY(old_height == item.GetInnerHeight());
  }
  if (verbose) { TRACE("A text rectangle of a single-line text must have its center at origin"); }
  {
    const auto center = QtRoundedEditRectItem::GetTextRectAtOrigin("ABCDEFG",QFont()).center();
    QVERIFY(center.x() >= -2.0);
    QVERIFY(center.x() <=  2.0);
    QVERIFY(center.y() >= -2.0);
    QVERIFY(center.y() <=  2.0);
  }
  if (verbose) { TRACE("A text rectangle of a multi-line text must have its center at origin"); }
  {
    const auto center = QtRoundedEditRectItem::GetTextRectAtOrigin(
      { "ABCDEFG", "ABCDEFG", "ABCDEFG" },
    QFont()).center();
    QVERIFY(center.x() >= -2.0);
    QVERIFY(center.x() <=  2.0);
    QVERIFY(center.y() >= -2.0);
    QVERIFY(center.y() <=  2.0);
  }
  if (verbose) { TRACE("A padded text rectangle of a multi-line text, line 1/3 must have its center above origin"); }
  {
    const auto center = QtRoundedEditRectItem::GetPaddedTextRectAtLine("ABCDEFG",QFont(),0,3).center();
    //TRACE(center.x());
    //TRACE(center.y());
    QVERIFY(center.x() >= -2.0);
    QVERIFY(center.x() <=  2.0);
    QVERIFY(center.y() <   0.0);
  }
  if (verbose) { TRACE("A padded text rectangle of a multi-line text, line 3/3 must have its center above origin"); }
  {
    const auto center = QtRoundedEditRectItem::GetPaddedTextRectAtLine("ABCDEFG",QFont(),2,3).center();
    //TRACE(center.x());
    //TRACE(center.y());
    QVERIFY(center.x() >= -2.0);
    QVERIFY(center.x() <=  2.0);
    QVERIFY(center.y() >   0.0);
  }
  if (verbose) { TRACE("A padded text rectangle must at least be as wide as a text rectangle"); }
  {
    QVERIFY(QtRoundedEditRectItem::GetTextRectAtOrigin("X",QFont()).width() > 0.0);
    QVERIFY(QtRoundedEditRectItem::GetPaddedTextRectAtOrigin("X",QFont()).width()
      >= QtRoundedEditRectItem::GetTextRectAtOrigin("X",QFont()).width());
  }
}
