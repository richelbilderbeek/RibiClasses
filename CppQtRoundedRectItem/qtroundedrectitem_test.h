#ifndef QTROUNDEDRECTITEM_TEST_H
#define QTROUNDEDRECTITEM_TEST_H

#include <QtTest/QtTest>

namespace ribi {

class QtRoundedRectItemTest : public QObject
{
    Q_OBJECT

private slots:

  void GetInnerRectIgnoresContourPenWidth();
  void GetInnerRectIgnoresFocusPenWidth();
  void SymmetricSetGetInnerHeight();
  void SymmetricSetGetInnerPos();
  void SymmetricSetGetInnerWidth();
  void SymmetricSetGetInnerX();
  void SymmetricSetGetInnerY();
  void SymmetricSetGetRadiusX();
  void SymmetricSetGetRadiusY();
  void all_tests();

  //To do/review
  void DefaultConstruction();
  void SetAndGetOuterX();
  void SetAndGetOuterY();
  void SetOuterHeightAndGetOuterHeightSymmetric();
  void SetOuterPosAndGetOuterPosSymmetric();
  void SetOuterWidthAndGetOuterWidthSymmetric();

  //Done
};

} //~namespace ribi

#endif // QTROUNDEDRECTITEM_TEST_H
