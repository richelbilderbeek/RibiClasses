#include "qtroundedrectitem_test.h"
#include "qtroundedrectitem.h"

#include <cmath>
#include <QCoreApplication>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QImage>
#include <QPainter>
#include "counter.h"
#include "trace.h"

void ribi::qtroundedrectitem_test::default_construction()
{
  const QtRoundedRectItem i;
  QVERIFY(std::abs(i.GetCenterX() - 0.0) < 0.0001);
  QVERIFY(std::abs(i.GetCenterY() - 0.0) < 0.0001);
}

void ribi::qtroundedrectitem_test::set_and_get_outer_x()
{
  QtRoundedRectItem i;
  const auto old_x = i.GetCenterX();
  const auto new_x = old_x + 10.0;
  i.SetCenterX(new_x);
  QVERIFY(std::abs(i.GetCenterX() - new_x) < 4.0);
}

void ribi::qtroundedrectitem_test::set_and_get_outer_y()
{
  QtRoundedRectItem i;
  const auto old_y = i.GetCenterY();
  const auto new_y = old_y + 10.0;
  i.SetCenterY(new_y);
  QVERIFY(std::abs(i.GetCenterY() - new_y) < 4.0);
}

void ribi::qtroundedrectitem_test::all_tests()
{
  using namespace ribi;
  const bool verbose{false};
  QtRoundedRectItem i;
  if (verbose) { TRACE("SetOuterPos and GetOuterPos must be symmetric"); }
  {
    const auto old_pos = i.GetCenterPos();
    const auto new_pos = old_pos + QPointF(10.0,10.0);
    i.SetCenterPos(new_pos);
    QVERIFY(std::abs(i.GetCenterPos().x() - new_pos.x()) < 2.0);
    QVERIFY(std::abs(i.GetCenterPos().y() - new_pos.y()) < 2.0);
  }
  if (verbose) { TRACE("SetOuterWidth and GetOuterWidth must be symmetric"); }
  {
    const auto old_width = i.GetOuterWidth();
    const auto new_width = old_width + 10.0;
    i.SetOuterWidth(new_width);
    QVERIFY(std::abs(i.GetOuterWidth() - new_width) < 2.0);
  }
  if (verbose) { TRACE("SetOuterHeight and GetOuterHeight must be symmetric"); }
  {
    const auto old_height = i.GetOuterHeight();
    const auto new_height = old_height + 10.0;
    i.SetOuterHeight(new_height);
    QVERIFY(std::abs(i.GetOuterHeight() - new_height) < 2.0);
  }
  if (verbose) { TRACE("SetInnerX and GetInnerX must be symmetric"); }
  {
    const auto old_x = i.GetCenterX();
    const auto new_x = old_x + 10.0;
    i.SetCenterX(new_x);
    QVERIFY(std::abs(i.GetCenterX() - new_x) < 2.0);
  }
  if (verbose) { TRACE("SetInnerY and GetInnerY must be symmetric"); }
  {
    const auto old_y = i.GetCenterY();
    const auto new_y = old_y + 10.0;
    i.SetCenterY(new_y);
    QVERIFY(std::abs(i.GetCenterY() - new_y) < 2.0);
  }
  if (verbose) { TRACE("SetInnerPos and GetInnerPos must be symmetric"); }
  {
    const auto old_pos = i.GetCenterPos();
    const auto new_pos = old_pos + QPointF(10.0,10.0);
    i.SetCenterPos(new_pos);
    QVERIFY(std::abs(i.GetCenterPos().x() - new_pos.x()) < 2.0);
    QVERIFY(std::abs(i.GetCenterPos().y() - new_pos.y()) < 2.0);
  }
  if (verbose) { TRACE("SetInnerHeight and GetInnerHeight must be symmetric"); }
  {
    const auto old_height = i.GetInnerHeight();
    const auto new_height = old_height + 10.0;
    i.SetInnerHeight(new_height);
    QVERIFY(std::abs(i.GetInnerHeight() - new_height) < 2.0);
  }
  if (verbose) { TRACE("SetInnerWidth and GetInnerWidth must be symmetric"); }
  {
    const auto old_width = i.GetInnerWidth();
    const auto new_width = old_width + 10.0;
    i.SetInnerWidth(new_width);
    //Difference has been measured to be 3.0
    QVERIFY(std::abs(i.GetInnerWidth() - new_width) < 4.0);
  }
  if (verbose) { TRACE("SetRadiusX and GetRadiusX must be symmetric"); }
  {
    const auto old_radius_x = i.GetRadiusX();
    const auto new_radius_x = old_radius_x + 10.0;
    i.SetRadiusX(new_radius_x);
    QVERIFY(std::abs(i.GetRadiusX() - new_radius_x) < 2.0);
  }
  if (verbose) { TRACE("SetRadiusY and GetRadiusY must be symmetric"); }
  {
    const auto old_radius_y = i.GetRadiusY();
    const auto new_radius_y = old_radius_y + 10.0;
    i.SetRadiusY(new_radius_y);
    QVERIFY(std::abs(i.GetRadiusY() - new_radius_y) < 2.0);
  }
  if (verbose) { TRACE("Position must be in GetOuterRect"); }
  {
    QVERIFY(i.GetOuterRect().contains(i.GetCenterPos()));
  }
  if (verbose) { TRACE("Position must be in GetInnerRect"); }
  {
    QVERIFY(i.GetInnerRect().contains(i.GetCenterPos()));
  }
  if (verbose) { TRACE("After changing the outer width and position, the Position must be in GetOuterRect"); }
  {
    i.SetOuterWidth(3.0);
    i.SetOuterHeight(3.0);
    i.SetCenterX(i.GetCenterX() + 10.0);
    i.SetCenterY(i.GetCenterY() + 10.0);
    QVERIFY(i.GetOuterRect().contains(i.GetCenterPos()));
  }
  if (verbose) { TRACE("After changing the inner width and position, the Position must be in GetInnerRect"); }
  {
    i.SetInnerWidth(3.0);
    i.SetInnerHeight(3.0);
    i.SetCenterX(i.GetCenterX() + 10.0);
    i.SetCenterY(i.GetCenterY() + 10.0);
    QVERIFY(i.GetInnerRect().contains(i.GetCenterPos()));
  }
  if (verbose) { TRACE("GetInnerRect should not change if width of contour pen changes"); }
  {
    QGraphicsScene scene;
    scene.addItem(&i);
    i.SetSelected(false);
    i.clearFocus();
    QVERIFY(!i.hasFocus() && !i.isSelected()
      && "Assume no focus, otherwise this test has no use");
    const auto before = i.GetInnerRect();
    const QPen old_pen{i.GetContourPen()};
    QPen new_pen{old_pen};
    new_pen.setWidth((old_pen.width() * 10) + 10);
    i.SetContourPen(new_pen);
    const auto after = i.GetInnerRect();
    QVERIFY(before == after);
    scene.removeItem(&i); //Otherwise i gets deleted
  }
  if (verbose) { TRACE("GetInnerRect should not change if width of focus pen changes"); }
  {
    QGraphicsScene scene;
    scene.addItem(&i);
    i.SetSelected(false);
    i.clearFocus();
    QVERIFY(!i.hasFocus() && !i.isSelected()
      && "Assume no focus, otherwise this test has no use");
    const auto before = i.GetInnerRect();
    const QPen old_pen{i.GetContourPen()};
    QPen new_pen{old_pen};
    new_pen.setWidth((old_pen.width() * 10) + 10);
    i.SetFocusPen(new_pen);
    const auto after = i.GetInnerRect();
    QVERIFY(before == after);
    scene.removeItem(&i); //Otherwise i gets deleted
  }
  #ifdef ISSUE_261_GIVE_UP_ON_TESTING_FOCUS
  if (verbose) { TRACE("GetInnerRect should not change if width of focus pen changes and given focus"); }
  {
    QGraphicsScene scene;
    scene.addItem(&i);
    i.setSelected(false);
    i.clearFocus();
    QVERIFY(!i.hasFocus() && !i.isSelected()
      && "Assume no focus, otherwise this test has no use");
    const auto before = i.GetInnerRect();
    const QPen old_pen{i.GetContourPen()};
    QPen new_pen{old_pen};
    new_pen.setWidth((old_pen.width() * 10) + 10);

    i.SetFocusPen(new_pen);
    i.setFocus();
    i.setSelected(true);
    // Force repaint
    {
      //Create an image of the item
      QImage image(scene.sceneRect().size().toSize(), QImage::Format_ARGB32);
      // Start all pixels transparent
      image.fill(Qt::transparent);
      QPainter painter(&image);
      scene.render(&painter);
      image.save("tmp_bug244_pre.png");
    }
    QVERIFY(i.hasFocus());
    QVERIFY(i.isSelected());
    QVERIFY(i.hasFocus() && i.isSelected()
      && "Assume focus, otherwise this test has no use");
    const auto after = i.GetInnerRect();
    QVERIFY(before == after);
    scene.removeItem(&i); //Otherwise i gets deleted
  }
  if (verbose) { TRACE("Bug #244: Changing pens does not resize the raw QRect"); }
  // The GetInnerRect should always remain the same
  // Test procedure:
  // 1) Set the contour pen to wide
  // 2) Give the item focus
  // Now, the inner rectangle has gotten bigger. This should not be.

  {

    QGraphicsScene scene;
    scene.addItem(&i);

    //Set contour pen to width 20
    {
      auto contour_pen = i.GetContourPen();
      contour_pen.setWidth(20);
      i.SetContourPen(contour_pen);
    }
    //Set focus pen to width 1
    {
      auto focus_pen = i.GetFocusPen();
      focus_pen.setWidth(1);
      i.SetFocusPen(focus_pen);
      QVERIFY(i.GetFocusPen().width() == 1);
    }
    QVERIFY(i.GetContourPen().width() == 20);
    QVERIFY(i.GetCurrentPen().width() == 20);
    QVERIFY(i.GetFocusPen().width() == 1);

    //OK: inner width + 2 * contour pen width = outer width
    {
      const auto expected_outer_width
        = i.GetInnerWidth() + (2 * i.GetContourPen().width());
      const auto real_outer_width = i.GetOuterWidth();
      QVERIFY(std::abs(expected_outer_width - real_outer_width) < 4
        && "inner_width + 2 * contour_pen_width should be about the outer width"
      );
    }
    //big_rect_width is the outer rectangle of the QItem, as it is drawn
    const auto big_rect_width = i.GetOuterRect().width();
    TRACE("PAY ATTENTION!");
    //Set focus to QtNode
    i.setSelected(true);
    QVERIFY(i.GetCurrentPen() == i.GetFocusPen()
      && "Be sure that QtNode has focus now");
    //Paint it to file, to force a redraw
    {
      //Create an image of before
      QImage image(scene.sceneRect().size().toSize(), QImage::Format_ARGB32);
      // Start all pixels transparent
      image.fill(Qt::transparent);
      QPainter painter(&image);
      scene.render(&painter);
      image.save("tmp_bug244.png");
    }

    //ERROR: inner width + 2 * focus pen width != outer width
    {
      const auto expected_outer_width
        = i.GetInnerWidth() + (2 * i.GetFocusPen().width());
      const auto real_outer_width = i.GetOuterWidth();
      QVERIFY(std::abs(expected_outer_width - real_outer_width) < 4
        && "inner_width + 2 * focus_pen_width should be about the outer width"
      );
    }
    QVERIFY(i.GetContourPen().width() == 20);
    QVERIFY(i.GetCurrentPen().width() == 1);
    QVERIFY(i.GetFocusPen().width() == 1);

    //small_rect_width is the outer rectangle of the QItem, as it is drawn
    //
    //
    //   v---v   = small_rect_width
    // v-------v = big_rect_width
    //
    // +-------+
    // |       |
    // | +---+ |
    // | | X | |
    // | +---+ |
    // |       |
    // +-------+

    const auto small_rect_width = i.GetInnerRect().width();
    QVERIFY(small_rect_width + 4 < big_rect_width);

    scene.removeItem(&i); //Otherwise i gets deleted

    QVERIFY(!"Fixed, refactor, then check in");
  }
  QVERIFY(1==2);
  #endif // ISSUE_261_GIVE_UP_ON_TESTING_FOCUS
}

