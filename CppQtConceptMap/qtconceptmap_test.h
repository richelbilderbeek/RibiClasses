#ifndef qtconceptmap_test_TEST_H
#define qtconceptmap_test_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class qtconceptmap_test : public QObject
{
    Q_OBJECT

private slots:
  void can_default_construct();
};

} //~namespace cmap
} //~namespace ribi

#endif // qtconceptmap_test_TEST_H