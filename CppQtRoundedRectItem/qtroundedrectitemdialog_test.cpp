#include "qtroundedrectitemdialog_test.h"

#include "qtroundedrectitem.h"
#include "qtroundedrectitemdialog.h"
#include "ui_qtroundedrectitemdialog.h"
#include "trace.h"

void ribi::qtroundedrectitemdialog_test::all_tests()
{
  using namespace ribi;
  const bool verbose{false};
  QtRoundedRectItemDialog dialog;
  boost::shared_ptr<QtRoundedRectItem> item{new QtRoundedRectItem};
  dialog.SetItem(item);
  //Change all, sorted by UI component name
  if (verbose) { TRACE("Set/Get Contour pen must be symmetric"); }
  {
    dialog.GetUi()->box_contour_pen_width->setValue( item->GetContourPen().widthF() + 10.0 );
    QPen new_pen = item->GetContourPen();
    new_pen.setWidthF(dialog.GetUi()->box_contour_pen_width->value() + 10.0);
    item->SetContourPen(new_pen);
    QVERIFY(item->GetContourPen() == new_pen);
  }
  if (verbose) { TRACE("Set/Get Focus pen width must be symmetric"); }
  {
    dialog.GetUi()->box_focus_pen_width->setValue( item->GetFocusPen().widthF() + 10.0);
    QPen new_pen = item->GetFocusPen();
    new_pen.setWidthF(dialog.GetUi()->box_focus_pen_width->value() + 10.0);
    item->SetFocusPen(new_pen);
    QVERIFY(item->GetFocusPen() == new_pen);
  }
  if (verbose) { TRACE("Set/Get InnerHeight must be symmetric"); }
  {
    dialog.GetUi()->box_height->setValue(item->GetInnerHeight() + 10.0);
    const auto new_height = dialog.GetUi()->box_height->value() + 10.0;
    item->SetInnerHeight(new_height);
    QVERIFY(std::abs(item->GetInnerHeight() - new_height) < 2.0);
  }
  if (verbose) { TRACE("Set/Get OuterHeight must be symmetric"); }
  {
    dialog.GetUi()->box_height_including_pen->setValue(item->GetOuterHeight() + 10.0);
    const auto new_height = dialog.GetUi()->box_height_including_pen->value() + 10.0;
    item->SetOuterHeight(new_height);
    QVERIFY(std::abs(item->GetOuterHeight() - new_height) < 2.0);
  }
  if (verbose) { TRACE("Set/Get Radius X must be symmetric"); }
  {
    dialog.GetUi()->box_radius_x->setValue(item->GetRadiusX() + 10.0);
    const auto new_x = dialog.GetUi()->box_radius_x->value() + 10.0;
    item->SetRadiusX(new_x);
    QVERIFY(std::abs(item->GetRadiusX() - new_x) < 2.0);
  }
  if (verbose) { TRACE("Set/Get Radius Y must be symmetric"); }
  {
    dialog.GetUi()->box_radius_y->setValue(item->GetRadiusY() + 10.0);
    const auto new_y = dialog.GetUi()->box_radius_y->value() + 10.0;
    item->SetRadiusY(new_y);
    QVERIFY(std::abs(item->GetRadiusY() - new_y) < 2.0);
  }
  if (verbose) { TRACE("Set/Get InnerWidth must be symmetric"); }
  {
    const double old_width{item->GetInnerWidth()};
    const double new_width{old_width + 10.0};
    item->SetInnerWidth(new_width);
    QVERIFY(std::abs(item->GetInnerWidth() - new_width) < 2.0);
  }
  if (verbose) { TRACE("Set/Get OuterWidth must be symmetric"); }
  {
    dialog.GetUi()->box_width_including_pen->setValue(item->GetOuterWidth() + 10.0);
    const auto new_width = dialog.GetUi()->box_width_including_pen->value() + 10.0;
    item->SetOuterWidth(new_width);
    QVERIFY(std::abs(item->GetOuterWidth() - new_width) < 2.0);
  }
  if (verbose) { TRACE("Set/Get X must be symmetric"); }
  {
    dialog.GetUi()->box_x->setValue(item->GetCenterX() + 10.0);
    const auto new_x = dialog.GetUi()->box_x->value() + 10.0;
    item->SetCenterX(new_x);
    QVERIFY(std::abs(item->GetCenterX() - new_x) < 2.0);
  }
  if (verbose) { TRACE("Set/Get Y must be symmetric"); }
  {
    dialog.GetUi()->box_y->setValue(item->GetCenterY() + 10.0);
    const auto new_y = dialog.GetUi()->box_y->value() + 10.0;
    item->SetCenterY(new_y);
    QVERIFY(std::abs(item->GetCenterY() - new_y) < 2.0);
  }
  if (verbose) { TRACE("SetItem with other item"); }
  {
    boost::shared_ptr<QtRoundedRectItem> new_item{new QtRoundedRectItem};
    dialog.SetItem(new_item);
    QVERIFY(dialog.GetItem() == new_item);
  }
  if (verbose) { TRACE("SetItem with original item"); }
  {
    dialog.SetItem(item);
    QVERIFY(dialog.GetItem() == item);
  }
  if (verbose) { TRACE("Changing a contour pen does not change the inner height"); }
  {
    QVERIFY(!item->GetIsSelected() && "Work with contour pen");
    const double old_height{item->GetInnerHeight()};
    const double new_height{old_height + 10.0};
    item->SetInnerHeight(new_height);
    QPen pen(item->GetContourPen());
    pen.setWidthF(pen.widthF() + 5.0);
    item->SetContourPen(pen);
    QVERIFY(std::abs(item->GetInnerHeight() - new_height) < 4.0);
  }
  if (verbose) { TRACE("Changing a contour pen does not change the inner width"); }
  {
    QVERIFY(!item->GetIsSelected() && "Work with contour pen");
    const double old_width{item->GetInnerWidth()};
    const double new_width{old_width + 10.0};
    item->SetInnerWidth(new_width);
    QPen pen(item->GetContourPen());
    pen.setWidthF(pen.widthF() + 5.0);
    item->SetContourPen(pen);
    QVERIFY(std::abs(item->GetInnerWidth() - new_width) < 4.0);
  }
  if (verbose) { TRACE("Changing a contour pen does change the outer height"); }
  {
    QVERIFY(!item->GetIsSelected() && "Work with contour pen");
    const double old_height{item->GetInnerHeight()};
    const double new_height{old_height + 10.0};
    item->SetOuterHeight(new_height);
    QPen pen(item->GetContourPen());
    pen.setWidthF(pen.widthF() + 5.0);
    item->SetContourPen(pen);
    QVERIFY(std::abs(item->GetOuterHeight() - new_height) > 4.0);
  }
  if (verbose) { TRACE("Changing a contour pen does change the outer width"); }
  {
    QVERIFY(!item->GetIsSelected() && "Work with contour pen");
    const double old_width{item->GetInnerWidth()};
    const double new_width{old_width + 10.0};
    item->SetOuterWidth(new_width);
    QPen pen(item->GetContourPen());
    pen.setWidthF(pen.widthF() + 5.0);
    item->SetContourPen(pen);
    QVERIFY(std::abs(item->GetOuterWidth() - new_width) > 4.0);
  }
  if (verbose) { TRACE("Brute-force some random items"); }
  {
    for (int i{0}; i!=10; ++i) { dialog.DoSomethingRandom(); }
  }
}
