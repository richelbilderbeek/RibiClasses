#ifndef RIBI_QTHIDEANDSHOWDIALOG_TEST_H
#define RIBI_QTHIDEANDSHOWDIALOG_TEST_H

#include <QtTest/QtTest>

namespace ribi {

class qthideandshowdialog_test : public QObject
{
    Q_OBJECT

private slots:
  void default_construct();
  void parent_can_show_child();
};

} //~namespace ribi

#endif // RIBI_QTHIDEANDSHOWDIALOG_TEST_H
