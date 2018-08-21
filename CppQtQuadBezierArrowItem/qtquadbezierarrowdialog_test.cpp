#include "qtquadbezierarrowdialog_test.h"

#include <cmath>
#include <QGraphicsSimpleTextItem>
#include "qtquadbezierarrowdialog.h"
#include "qtquadbezierarrowitem.h"

void ribi::QtQuadBezierArrowDialogTest::all_tests()
{
  using namespace ribi;
  const boost::shared_ptr<QGraphicsItem> from{new QGraphicsSimpleTextItem};
  const bool tail{false};
  const boost::shared_ptr<QGraphicsItem> mid{new QGraphicsSimpleTextItem};
  const bool head{true};
  const boost::shared_ptr<QGraphicsItem> to{new QGraphicsSimpleTextItem};
  const boost::shared_ptr<QtQuadBezierArrowItem> arrow{
    new QtQuadBezierArrowItem(from.get(),tail,mid.get(),head,to.get())
  };
  QtQuadBezierArrowDialog d;
  d.SetArrow(arrow);
  //if (verbose) { TRACE("Get/SetUiMidX must be symmetric"); }
  {
    const double old_x{d.GetUiMidX()};
    const double new_x{old_x + 10.0};
    d.SetUiMidX(new_x);
    QVERIFY(std::abs(d.GetUiMidX() - new_x) < 2.0);
  }
  //if (verbose) { TRACE("Get/SetUiMidY must be symmetric"); }
  {
    const double old_y{d.GetUiMidY()};
    const double new_y{old_y + 10.0};
    d.SetUiMidY(new_y);
    QVERIFY(std::abs(d.GetUiMidY() - new_y) < 2.0);
  }

  /*
  QVERIFY(std::abs(ui->box_focus_pen_width->value() - m_arrow->GetFocusPen().widthF()) < 2.0);
  QVERIFY(std::abs(ui->box_from_x->value() - m_arrow->GetFromItem()->x()) < 2.0);
  QVERIFY(std::abs(ui->box_from_y->value() - m_arrow->GetFromItem()->y()) < 2.0);
  QVERIFY(ui->box_has_head->isChecked() == m_arrow->HasHead());
  QVERIFY(ui->box_has_tail->isChecked() == m_arrow->HasTail());
  QVERIFY(std::abs(ui->box_mid_x->value() - m_arrow->GetMidItem()->x()) < 2.0);
  QVERIFY(std::abs(ui->box_mid_y->value() - m_arrow->GetMidItem()->y()) < 2.0);
  QVERIFY(std::abs(ui->box_normal_pen_width->value() - m_arrow->GetPen().widthF()) < 2.0);
  QVERIFY(std::abs(ui->box_to_x->value() - m_arrow->GetToItem()->x()) < 2.0);
  QVERIFY(std::abs(ui->box_to_y->value() - m_arrow->GetToItem()->y()) < 2.0);
  */

}


