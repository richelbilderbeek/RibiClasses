#ifndef GRABBER_TEST_H
#define GRABBER_TEST_H

#include <QtTest/QtTest>

namespace ribi {

class grabber_test : public QObject
{
    Q_OBJECT

private slots:
  void can_default_construct();
};

} //~namespace ribi

#endif // GRABBER_TEST_H
