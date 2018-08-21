#ifndef QTIMAGE_TEST_H
#define QTIMAGE_TEST_H

#include <QtTest/QtTest>

namespace ribi {

class QtImageTest : public QObject
{
    Q_OBJECT

private slots:
  void all_tests();
};

} //~namespace ribi
#endif // QTIMAGE_TEST_H
