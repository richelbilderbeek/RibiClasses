#include "qtconceptmap_test.h"
#include "qtconceptmap.h"
#include "qtconceptmap.h"

#include <chrono>
#include <QApplication>
#include <QMouseEvent>
#include <boost/graph/isomorphism.hpp>

#include "counter.h"
#include "qtconceptmapcollect.h"
#include "conceptmapfactory.h"

#include "qtconceptmapcommandaddselectedrandom.h"
#include "qtconceptmapcommandcreatenewedge.h"
#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmapcommanddeleteselected.h"
#include "qtconceptmapcommandunselectrandom.h"
#include "conceptmap.h"
#include "conceptmapedge.h"
#include "conceptmapedgefactory.h"
#include "conceptmapnode.h"
#include "qtconceptmapqtnode.h"
#include "conceptmapnodefactory.h"
#include "ribi_system.h"

#include "trace.h"
#pragma GCC diagnostic pop

QKeyEvent CreateControlDown() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Down,Qt::ControlModifier); }
QKeyEvent CreateControlE() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_E,Qt::ControlModifier); }
QKeyEvent CreateControlN() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_N,Qt::ControlModifier); }
QKeyEvent CreateControlSpace() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Space,Qt::ControlModifier); }
QKeyEvent CreateControlZ() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Z,Qt::ControlModifier); }
QKeyEvent CreateDel() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Delete,Qt::NoModifier); }
QKeyEvent CreateDown() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Down,Qt::NoModifier); }
QKeyEvent CreateLeft() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Left,Qt::NoModifier); }
QKeyEvent CreateRight() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Right,Qt::NoModifier); }
QKeyEvent CreateSpace() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Space,Qt::NoModifier); }
QKeyEvent CreateUp() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Up,Qt::NoModifier); }

void ribi::cmap::qtconceptmap_test::all_tests()
{
  using namespace ribi::cmap;
  bool verbose{false};

  if (verbose) { TRACE("SetConceptMap, all tests"); }
  for (const auto conceptmap: ConceptMapFactory().GetAllTests())
  {
    QtConceptMap m;
    QVERIFY(CountQtNodes(m.GetScene()) == 0);
    m.SetConceptMap(conceptmap);
    const auto n_nodes = static_cast<int>(boost::num_vertices(conceptmap));
    const auto n_edges = static_cast<int>(boost::num_edges(conceptmap));
    const auto n_qtnodes = CountQtNodes(m.GetScene());
    const auto n_qtedges = CountQtEdges(m.GetScene());
    QVERIFY(n_nodes == n_qtnodes);
    QVERIFY(n_edges == n_qtedges);
  }
  if (verbose) { TRACE("A new command must be put in QUndoStack"); }
  {
    QtConceptMap qtconceptmap;

    CommandCreateNewNode * const command {
      new CommandCreateNewNode(
        qtconceptmap.GetConceptMap(),
        qtconceptmap.GetScene(),
        qtconceptmap.GetQtToolItem(),
        0.0,
        0.0
      )
    };
    QVERIFY(qtconceptmap.GetUndo().count() == 0);

    qtconceptmap.DoCommand(command);

    QVERIFY(qtconceptmap.GetUndo().count() == 1);
  }
  //---------------------------------------------------------------------------
  // CREATE NODES
  //---------------------------------------------------------------------------
  if (verbose) { TRACE("Create one node"); }
  {
    QtConceptMap qtconceptmap;
    QVERIFY(DoubleCheckEdgesAndNodes(qtconceptmap,0,0));
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,0,0));
    qtconceptmap.DoCommand(
      new CommandCreateNewNode(
        qtconceptmap.GetConceptMap(),
        qtconceptmap.GetScene(),
        qtconceptmap.GetQtToolItem(),
        0.0,
        0.0
      )
    );
    QVERIFY(DoubleCheckEdgesAndNodes(qtconceptmap,0,1));
      QVERIFY(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,0,1));

    qtconceptmap.Undo();
    QVERIFY(DoubleCheckEdgesAndNodes(qtconceptmap,0,0));
      QVERIFY(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,0,0));
  }
  if (verbose) { TRACE("CTRL-N creates a new QtNode"); }
  {
    QtConceptMap qtconceptmap;

    auto ctrl_n = CreateControlN();
    qtconceptmap.keyPressEvent(&ctrl_n);
    QVERIFY(DoubleCheckEdgesAndNodes(qtconceptmap,0,1));
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,0,1));
  }
  if (verbose) { TRACE("Create nodes"); }
  {
    QtConceptMap qtconceptmap;
    QVERIFY(DoubleCheckEdgesAndNodes(qtconceptmap,0,0));

    const int n{10};
    for (int i=0; i!=n; ++i) {
      qtconceptmap.DoCommand(
        new CommandCreateNewNode(
          qtconceptmap.GetConceptMap(),
          qtconceptmap.GetScene(),
          qtconceptmap.GetQtToolItem(),
          0.0,
          0.0
        )
      );
      QVERIFY(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,0,i + 1));
    }
    QVERIFY(DoubleCheckEdgesAndNodes(qtconceptmap,0,n));
    for (int i=0; i!=n; ++i) { qtconceptmap.Undo(); }
    QVERIFY(DoubleCheckEdgesAndNodes(qtconceptmap,0,0));
  }
  if (verbose) { TRACE("CTRL-N, CTRL-N creates two QtNodes"); }
  {
    QtConceptMap qtconceptmap;

    auto ctrl_n = CreateControlN();
    qtconceptmap.keyPressEvent(&ctrl_n);
    qtconceptmap.keyPressEvent(&ctrl_n);

    QVERIFY(DoubleCheckEdgesAndNodes(qtconceptmap,0,2));
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,0,2));
  }

  //---------------------------------------------------------------------------
  // DELETE NODE
  //---------------------------------------------------------------------------
  if (verbose) { TRACE("Delete nodes: create two Nodes, delete one Node from QtConceptMap"); }
  {
    QtConceptMap qtconceptmap;
    QVERIFY(DoubleCheckEdgesAndNodes(qtconceptmap,0,0));

    const int n{10};
    for (int i=0; i!=n; ++i) {
      qtconceptmap.DoCommand(
        new CommandCreateNewNode(
          qtconceptmap.GetConceptMap(),
          qtconceptmap.GetScene(),
          qtconceptmap.GetQtToolItem(),
          0.0,
          0.0
        )
      );
      QVERIFY(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,0,i + 1));
    }
    QVERIFY(DoubleCheckEdgesAndNodes(qtconceptmap,0,n));
    qtconceptmap.DoCommand(
      new CommandDeleteSelected(
        qtconceptmap.GetConceptMap(),
        qtconceptmap.GetScene(),
        qtconceptmap.GetQtToolItem()
      )
    );
    QVERIFY(DoubleCheckEdgesAndNodes(qtconceptmap,0,0));
    qtconceptmap.Undo();
    QVERIFY(DoubleCheckEdgesAndNodes(qtconceptmap,0,n));
  }
  if (verbose) { TRACE("Delete Edge"); }
  {
    QtConceptMap qtconceptmap;
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,0,0));
    const int n{2};
    for (int i=0; i!=n; ++i) {
      qtconceptmap.DoCommand(
        new CommandCreateNewNode(
          qtconceptmap.GetConceptMap(),
          qtconceptmap.GetScene(),
          qtconceptmap.GetQtToolItem(),
          0.0,
          0.0
        )
      );
    }
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,0,2));
    qtconceptmap.DoCommand(
      new CommandCreateNewEdgeBetweenTwoSelectedNodes(
        qtconceptmap.GetConceptMap(),
        qtconceptmap.GetScene(),
        qtconceptmap.GetQtToolItem()
      )
    );
    QVERIFY(DoubleCheckEdgesAndNodes(qtconceptmap,1,2));
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,1,0));
    qtconceptmap.DoCommand(
      new CommandDeleteSelected(
        qtconceptmap.GetConceptMap(),
        qtconceptmap.GetScene(),
        qtconceptmap.GetQtToolItem()
      )
    );
    QVERIFY(DoubleCheckEdgesAndNodes(qtconceptmap,0,2));
  }

  if (verbose) { TRACE("CTRL-N, CTRL-N, delete, creates two QtNodes and deletes two"); }
  {
    QtConceptMap qtconceptmap;

    auto ctrl_n = CreateControlN();
    qtconceptmap.keyPressEvent(&ctrl_n);
    qtconceptmap.keyPressEvent(&ctrl_n);
    auto del = CreateDel();
    qtconceptmap.keyPressEvent(&del);
    QVERIFY(DoubleCheckEdgesAndNodes(qtconceptmap,0,0));
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,0,0));
  }
  if (verbose) { TRACE("CTRL-N, CTRL-N, space, delete, creates two QtNodes, selects one and deletes one"); }
  {
    QtConceptMap qtconceptmap;
    auto ctrl_n = CreateControlN();
    qtconceptmap.keyPressEvent(&ctrl_n);
    qtconceptmap.keyPressEvent(&ctrl_n);
    auto space = CreateSpace();
    qtconceptmap.keyPressEvent(&space);
    QVERIFY(DoubleCheckEdgesAndNodes(qtconceptmap,0,2));
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,0,1));
    auto del = CreateDel();
    qtconceptmap.keyPressEvent(&del);
    QVERIFY(DoubleCheckEdgesAndNodes(qtconceptmap,0,1));
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,0,0));
  }
  //---------------------------------------------------------------------------
  // CREATE EDGES
  //---------------------------------------------------------------------------
  if (verbose) { TRACE("Create edge"); }
  {
    //When there are two selected nodes, an edge can be created
    //After adding the edges, only the edge will be selected
    //The edge its center concept will be between the two nodes
    QtConceptMap qtconceptmap;
    const int n{2};
    for (int i=0; i!=n; ++i) {
      qtconceptmap.DoCommand(
        new CommandCreateNewNode(
          qtconceptmap.GetConceptMap(),
          qtconceptmap.GetScene(),
          qtconceptmap.GetQtToolItem(),
          0.0,
          0.0
        )
      );
    }
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,0,2));
    qtconceptmap.DoCommand(
      new CommandCreateNewEdgeBetweenTwoSelectedNodes(
        qtconceptmap.GetConceptMap(),
        qtconceptmap.GetScene(),
        qtconceptmap.GetQtToolItem()
      )
    );
    QVERIFY(DoubleCheckEdgesAndNodes(qtconceptmap,1,2));
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,1,0));
  }
  if (verbose) { TRACE("CTRL-N, CTRL-N, CTRL-E: new Edge should be selected"); }
  {
    QtConceptMap qtconceptmap;
    auto ctrl_n = CreateControlN();
    qtconceptmap.keyPressEvent(&ctrl_n);
    qtconceptmap.keyPressEvent(&ctrl_n);
    auto ctrl_e = CreateControlE();
    qtconceptmap.keyPressEvent(&ctrl_e);
    QVERIFY(DoubleCheckEdgesAndNodes(qtconceptmap,1,2));
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,1,0));
  }
  //---------------------------------------------------------------------------
  // DELETE EDGES (also via source/target nodes)
  //---------------------------------------------------------------------------
  if (verbose) { TRACE("CTRL-N, CTRL-N, CTRL-E, del: new Edge should be deleted"); }
  {
    QtConceptMap qtconceptmap;
    auto ctrl_n = CreateControlN();
    qtconceptmap.keyPressEvent(&ctrl_n);
    qtconceptmap.keyPressEvent(&ctrl_n);
    auto ctrl_e = CreateControlE();
    qtconceptmap.keyPressEvent(&ctrl_e);
    auto del = CreateDel();
    qtconceptmap.keyPressEvent(&del);
    QVERIFY(DoubleCheckEdgesAndNodes(qtconceptmap,0,2));
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,0,0));
  }
  //#define FOR_LUCAS_20160130
  #ifdef FOR_LUCAS_20160130
  if (verbose) { TRACE("Delete Edge and Node-that-is-head-of-Edge: delete from QtConceptMap"); }
  {
    QtConceptMap qtconceptmap;
    auto ctrl_n = CreateControlN();
    qtconceptmap.keyPressEvent(&ctrl_n);
    qtconceptmap.keyPressEvent(&ctrl_n);
    auto ctrl_e = CreateControlE();
    qtconceptmap.keyPressEvent(&ctrl_e);
    QVERIFY(DoubleCheckEdgesAndNodes(qtconceptmap,1,2));
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,1,0));
    std::srand(42);
    //Select only one single QtNode
    //PROBLEM: This is an infinite loop. Why?
    while (CountSelectedQtNodes(qtconceptmap.GetScene()) != 1
      || CountSelectedQtEdges(qtconceptmap.GetScene()) != 0
    ) {
      auto ctrl_space = CreateControlSpace();
      qtconceptmap.keyPressEvent(&ctrl_space);
      std::cerr
        << CountSelectedQtNodes(qtconceptmap.GetScene())
        << ","
        << CountSelectedQtEdges(qtconceptmap.GetScene())
      ;
    }
    QVERIFY(!"You've fixed the problem halfways");
    QVERIFY(CountSelectedQtEdges(qtconceptmap.GetScene()) == 0);
    QVERIFY(DoubleCheckEdgesAndNodes(qtconceptmap,1,2));
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,0,1));
    //Deleting the QtNode should also delete the QtEdge that is connected to it
    auto del = CreateDel();
    qtconceptmap.keyPressEvent(&del);
    QVERIFY(DoubleCheckEdgesAndNodes(qtconceptmap,0,1));
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,0,0));
  }
  QVERIFY(!"You've fixed it!");
  #endif // FOR_LUCAS_20160130
  //#define FIX_ISSUE_1
  #ifdef  FIX_ISSUE_1
  verbose = true;
  if (verbose) { TRACE("FIX_ISSUE_1"); }
  if (verbose) { TRACE("CTRL-N, CTRL-N, CTRL-E, Left: should select one Node"); }
  {
    QtConceptMap qtconceptmap;
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    qtconceptmap.SetConceptMap(conceptmap);
    conceptmap.SetVerbosity(true);
    qtconceptmap.SetVerbosity(true);

    if (verbose) { TRACE("Create two nodes"); }
    auto ctrln = CreateControlN();
    qtconceptmap.keyPressEvent(&ctrln);
    qtconceptmap.keyPressEvent(&ctrln);

    if (verbose) { TRACE("Create an edge, edge is selected"); }
    auto ctrle = CreateControlE();
    qtconceptmap.keyPressEvent(&ctrle);

    QVERIFY(qtconceptmap.GetQtNodes().size() == 2);
    qtconceptmap.GetQtNodes()[0]->SetCenterPos(-100.0,0.0);
    qtconceptmap.GetQtNodes()[0]->setToolTip("QtNodes[0]");
    qtconceptmap.GetQtNodes()[1]->SetCenterPos( 100.0,0.0);
    qtconceptmap.GetQtNodes()[1]->setToolTip("QtNodes[1]");
    qtconceptmap.GetQtEdges()[0]->setToolTip("QtEdges[0]");
    qtconceptmap.GetQtEdges()[0]->GetQtNode()->setToolTip("QtEdges[0] its center QtNode");

    QVERIFY(boost::num_vertices(conceptmap) == 2);
    QVERIFY(boost::num_vertices(conceptmap) == qtconceptmap.GetQtNodes().size());
    QVERIFY(boost::num_edges(conceptmap) == 1);
    QVERIFY(boost::num_edges(conceptmap) == qtconceptmap.GetQtEdges().size());
    QVERIFY(conceptmap.GetSelectedNodes().size() == 0);
    QVERIFY(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
    QVERIFY(conceptmap.GetSelectedEdges().size() == 1);
    QVERIFY(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());

    //Fails if item is invisible
    qtconceptmap.showFullScreen();
    for (int i=0; i!=100; ++i) qApp->processEvents();
    QVERIFY(qtconceptmap.scene()->selectedItems().count() > 0);


    if (verbose) { TRACE("Unselect the edge, select the node by pressing an arrow key"); }
    auto up = CreateRight(); //Selects
    TRACE("START");
    conceptmap.SetVerbosity(true);
    qtconceptmap.SetVerbosity(true);
    qtconceptmap.keyPressEvent(&up);

    TRACE(conceptmap.GetSelectedNodes().size());
    TRACE(qtconceptmap.GetSelectedQtNodes().size());
    TRACE(conceptmap.GetSelectedEdges().size());
    TRACE(qtconceptmap.GetSelectedQtEdges().size());


    QVERIFY(boost::num_vertices(conceptmap) == 2);
    QVERIFY(boost::num_vertices(conceptmap) == qtconceptmap.GetQtNodes().size());
    QVERIFY(boost::num_edges(conceptmap) == 1);
    QVERIFY(boost::num_edges(conceptmap) == qtconceptmap.GetQtEdges().size());
    QVERIFY(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
    QVERIFY(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());
    QVERIFY(conceptmap.GetSelectedNodes().size() == 1);
    QVERIFY(conceptmap.GetSelectedEdges().size() == 0);
  }
  if (verbose) { TRACE("Delete Node-that-is-head-of-Edge, then Undo"); }
  {
    QtConceptMap qtconceptmap;
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    qtconceptmap.SetConceptMap(conceptmap);

    //Create two nodes
    auto ctrln = CreateControlN();
    qtconceptmap.keyPressEvent(&ctrln);
    qtconceptmap.keyPressEvent(&ctrln);

    //Create an edge
    auto ctrle = CreateControlE();
    qtconceptmap.keyPressEvent(&ctrle);

    QVERIFY(boost::num_vertices(conceptmap) == 2);
    QVERIFY(boost::num_vertices(conceptmap) == qtconceptmap.GetQtNodes().size());
    QVERIFY(boost::num_edges(conceptmap) == 1);
    QVERIFY(boost::num_edges(conceptmap) == qtconceptmap.GetQtEdges().size());
    QVERIFY(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
    QVERIFY(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());
    QVERIFY(conceptmap.GetSelectedNodes().size() == 0);
    QVERIFY(conceptmap.GetSelectedEdges().size() == 1);


    //Select a node
    auto left = CreateLeft();
    qtconceptmap.keyPressEvent(&left);

    TRACE(conceptmap.GetSelectedNodes().size());
    TRACE(qtconceptmap.GetSelectedQtNodes().size());
    TRACE(conceptmap.GetSelectedEdges().size());
    TRACE(qtconceptmap.GetSelectedQtEdges().size());

    QVERIFY(boost::num_vertices(conceptmap) == 2);
    QVERIFY(boost::num_vertices(conceptmap) == qtconceptmap.GetQtNodes().size());
    QVERIFY(boost::num_edges(conceptmap) == 1);
    QVERIFY(boost::num_edges(conceptmap) == qtconceptmap.GetQtEdges().size());
    QVERIFY(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
    QVERIFY(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());
    QVERIFY(conceptmap.GetSelectedNodes().size() == 1);
    QVERIFY(conceptmap.GetSelectedEdges().size() == 0);

    //Delete the node, edge will be deleted as well
    auto del = CreateDel();
    qtconceptmap.keyPressEvent(&del);

    //Undo the deletion, should bring back node and edge
    auto cntrlz = CreateControlZ();
    qtconceptmap.keyPressEvent(&cntrlz);

    QVERIFY(boost::num_vertices(conceptmap) == 2);
    QVERIFY(boost::num_vertices(conceptmap) == qtconceptmap.GetQtNodes().size());
    QVERIFY(boost::num_edges(conceptmap) == 1);
    QVERIFY(boost::num_edges(conceptmap) == qtconceptmap.GetQtEdges().size());
    QVERIFY(conceptmap.GetSelectedNodes().size() == 1);
    QVERIFY(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
    QVERIFY(conceptmap.GetSelectedEdges().size() == 0);
    QVERIFY(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());

  }
  QVERIFY(!"Fixed issue #1");
  #endif // FIX_ISSUE_1

  #ifdef NOT_NOW_20160226
  #if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
  if (verbose) { TRACE("MouseDoubleClick"); }
  for (int i=0; i!=5; ++i)
  {
    QtConceptMap m;
    m.SetConceptMap(ConceptMapFactory().GetHeteromorphousTestConceptMap(16));
    QMouseEvent e(
      QEvent::Type::MouseButtonDblClick,
      QPointF( std::sin(static_cast<double>(i)), std::cos(static_cast<double>(i) ) ), //localPos,
      Qt::MouseButton::LeftButton,
      Qt::MouseButtons(),
      Qt::NoModifier
    );
    m.mouseDoubleClickEvent(&e);
    const int n_nodes_in_scene{static_cast<int>(Collect<QtNode>(m.GetScene()).size())};
    const int n_nodes_in_conceptmap{static_cast<int>(m.GetConceptMap().GetNodes().size())};
    QVERIFY(n_nodes_in_scene == n_nodes_in_conceptmap);
  }
  #endif //NOT_NOW_20160226


  #endif
}
