#ifndef XXX_TEST_H
#define XXX_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class qtconceptmapnodedialog_test : public QObject
{
    Q_OBJECT

private slots:
  void can_default_construct();
};

} //~namespace cmap
} //~namespace ribi

#endif // XXX_TEST_H
