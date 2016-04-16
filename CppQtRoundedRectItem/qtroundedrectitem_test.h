#ifndef QTROUNDEDRECTITEM_TEST_H
#define QTROUNDEDRECTITEM_TEST_H

#include <QtTest/QtTest>

namespace ribi {

class qtroundedrectitem_test : public QObject
{
    Q_OBJECT

private slots:
  void all_tests();
  void default_construction();
  void set_and_get_outer_x();
  void set_and_get_outer_y();
};

} //~namespace ribi

#endif // QTROUNDEDRECTITEM_TEST_H
