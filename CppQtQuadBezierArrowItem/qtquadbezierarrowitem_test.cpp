#include "qtquadbezierarrowitem_test.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "grabber.h"
#include "qtroundededitrectitem.h"
#include "qtquadbezierarrowitem.h"

void ribi::qtquadbezierarrowitem_test::all_tests()
{
  using namespace ribi;
  //Render one QtQuadBezierArrowItem
  {
    QGraphicsScene * const my_scene{new QGraphicsScene};
    std::vector<QGraphicsRectItem *> rects;
    //From
    {
      const double x1{50.0};
      const double y1{0.0};
      QtRoundedEditRectItem * const rect = new QtRoundedEditRectItem;
      rect->SetCenterPos(x1,y1);
      my_scene->addItem(rect);
      rects.push_back(rect);
    }
    //Center
    {
      const double x1{0.0};
      const double y1{0.0};
      QtRoundedEditRectItem * const rect = new QtRoundedEditRectItem;
      rect->SetCenterPos(x1,y1);
      my_scene->addItem(rect);
      rects.push_back(rect);
    }
    //To
    {
      const double x1{ 0.0};
      const double y1{50.0};
      QtRoundedEditRectItem * const rect = new QtRoundedEditRectItem;
      rect->SetCenterPos(x1,y1);
      my_scene->addItem(rect);
      rects.push_back(rect);
    }
    //Arrow
    QtQuadBezierArrowItem * arrow{new QtQuadBezierArrowItem(
        rects[0],
        false,
        rects[1],
        true,
        rects[2]
      )
    };
    my_scene->addItem(arrow);
    //Scales
    {
      QGraphicsLineItem * const xaxis{new QGraphicsLineItem(-100.0,0.0,100.0,0.0)};
      QGraphicsLineItem * const yaxis{new QGraphicsLineItem(0.0,-100.0,0.0,100.0)};
      my_scene->addItem(xaxis);
      my_scene->addItem(yaxis);
    }

    QGraphicsView * const view{new QGraphicsView};
    view->setScene(my_scene);
    ribi::Grabber grabber(view->winId(),"QtQuadBezierArrowItemTest1.png");
    view->show();
    view->setGeometry(0,0,300,300);
    arrow->SetVerbosity(false);
    //Increase max value of i if screenshot does not show the QGraphicsView
    for (int i=0; i!=10; ++i) { qApp->processEvents(); }
    view->show();
    grabber.Grab();

    //Same general tests
    QVERIFY(arrow->GetCenter().x() == 25.0);
    QVERIFY(arrow->GetCenter().y() == 25.0);
    QVERIFY(arrow->zValue() < rects[0]->zValue());
    QVERIFY(arrow->zValue() < rects[1]->zValue());
    QVERIFY(arrow->zValue() < arrow->GetMidItem()->zValue());
    delete my_scene;
    delete view;
  }
}
