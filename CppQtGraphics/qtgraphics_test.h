#ifndef QTGRAPHICS_TEST_H
#define QTGRAPHICS_TEST_H

#include <QtTest/QtTest>

namespace ribi {

class qtgraphics_test : public QObject
{
    Q_OBJECT

private slots:
  void all_tests();
};

} //~namespace ribi

#endif // QTGRAPHICS_TEST_H
