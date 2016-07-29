#ifndef QTQTSCOPEDDISABLE_TEST_H
#define QTQTSCOPEDDISABLE_TEST_H

#include <QtTest/QtTest>

namespace ribi {

class qtscopeddisable_test : public QObject
{
    Q_OBJECT

private slots:
  void default_construct();
};

} //~namespace ribi

#endif // QTQTSCOPEDDISABLE_TEST_H
