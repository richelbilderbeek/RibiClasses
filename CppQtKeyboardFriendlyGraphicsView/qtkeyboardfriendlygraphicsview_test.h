#ifndef QTKEYBOARDFRIENDLYGRAPHICSVIEW_TEST_H
#define QTKEYBOARDFRIENDLYGRAPHICSVIEW_TEST_H

#include <QtTest/QtTest>
#include "qtkeyboardfriendlygraphicsview.h"
namespace ribi {

class QtKeyboardFriendlyGraphicsViewTest : public QObject
{
    Q_OBJECT

private slots:
  void SelectNothing();
  void SendRandomKeys();
  void SpaceSelectsOneRandomItemWhenTwoAreSelected();
  void SpaceSelectsOneRandomItemWhenZeroAreSelected();
  void TappingShiftMustKeepFocus();


public slots:
  void cleanup();
  void init();

private:
  QtKeyboardFriendlyGraphicsView * m_view;
  QGraphicsRectItem * m_item1;
  QGraphicsRectItem * m_item2;
};

QtKeyboardFriendlyGraphicsView CreateTestView() noexcept;

} //~namespace ribi


#endif // QTKEYBOARDFRIENDLYGRAPHICSVIEW_TEST_H
