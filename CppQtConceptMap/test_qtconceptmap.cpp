//---------------------------------------------------------------------------
/*
QtConceptMap, Qt classes for display and interaction with ConceptMap
Copyright (C) 2013-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppQtConceptMap.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
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
#include "testtimer.h"
#include "trace.h"
#pragma GCC diagnostic pop

QKeyEvent CreateControlDown() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Down,Qt::ControlModifier); }
QKeyEvent CreateControlE() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_E,Qt::ControlModifier); }
QKeyEvent CreateControlN() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_N,Qt::ControlModifier); }
QKeyEvent CreateControlZ() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Z,Qt::ControlModifier); }
QKeyEvent CreateDel() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Delete,Qt::NoModifier); }
QKeyEvent CreateDown() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Down,Qt::NoModifier); }
QKeyEvent CreateLeft() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Left,Qt::NoModifier); }
QKeyEvent CreateRight() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Right,Qt::NoModifier); }
QKeyEvent CreateSpace() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Space,Qt::NoModifier); }
QKeyEvent CreateUp() noexcept { return QKeyEvent(QEvent::KeyPress,Qt::Key_Up,Qt::NoModifier); }

#ifndef NDEBUG
void ribi::cmap::QtConceptMap::Test() noexcept
{
  {
    static bool is_tested = false;
    if (is_tested) return;
    is_tested = true;
  }

  const TestTimer test_timer{__func__,__FILE__,2.0};
  TestTimer::SetMaxCnt(2); //Because the base class (QtKeyboardFriendlyGraphicsView)
                           //has to be tested as well
  bool verbose{false};

  if (verbose) { TRACE("SetConceptMap, all tests"); }
  for (const auto conceptmap: ConceptMapFactory().GetAllTests())
  {
    QtConceptMap m;
    assert(CountQtNodes(m.GetScene()) == 0);
    m.SetConceptMap(conceptmap);
    const auto n_nodes = static_cast<int>(boost::num_vertices(conceptmap));
    const auto n_edges = static_cast<int>(boost::num_edges(conceptmap));
    const auto n_qtnodes = CountQtNodes(m.GetScene());
    const auto n_qtedges = CountQtEdges(m.GetScene());
    assert(n_nodes == n_qtnodes);
    assert(n_edges == n_qtedges);
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
    assert(qtconceptmap.GetUndo().count() == 0);

    qtconceptmap.DoCommand(command);

    assert(qtconceptmap.GetUndo().count() == 1);
  }
  if (verbose) { TRACE("Create one node"); }
  {
    QtConceptMap qtconceptmap;
    assert(DoubleCheckEdgesAndNodes(qtconceptmap,0,0));
    assert(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,0,0));
    qtconceptmap.DoCommand(
      new CommandCreateNewNode(
        qtconceptmap.GetConceptMap(),
        qtconceptmap.GetScene(),
        qtconceptmap.GetQtToolItem(),
        0.0,
        0.0
      )
    );
    assert(DoubleCheckEdgesAndNodes(qtconceptmap,0,1));
      assert(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,0,1));

    qtconceptmap.Undo();
    assert(DoubleCheckEdgesAndNodes(qtconceptmap,0,0));
      assert(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,0,0));
  }
  if (verbose) { TRACE("Create nodes"); }
  {
    QtConceptMap qtconceptmap;
    assert(DoubleCheckEdgesAndNodes(qtconceptmap,0,0));

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
      assert(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,0,i + 1));
    }
    assert(DoubleCheckEdgesAndNodes(qtconceptmap,0,n));
    for (int i=0; i!=n; ++i) { qtconceptmap.Undo(); }
    assert(DoubleCheckEdgesAndNodes(qtconceptmap,0,0));
  }
  if (verbose) { TRACE("Create edge"); }
  {
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
    qtconceptmap.DoCommand(
      new CommandCreateNewEdgeBetweenTwoSelectedNodes(
        qtconceptmap.GetConceptMap(),
        qtconceptmap.GetScene(),
        qtconceptmap.GetQtToolItem()
      )
    );
    assert(DoubleCheckEdgesAndNodes(qtconceptmap,1,2));
  }
  if (verbose) { TRACE("Delete nodes: create two Nodes, delete one Node from QtConceptMap"); }
  {
    QtConceptMap qtconceptmap;
    assert(DoubleCheckEdgesAndNodes(qtconceptmap,0,0));

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
      assert(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,0,i + 1));
    }
    assert(DoubleCheckEdgesAndNodes(qtconceptmap,0,n));
    qtconceptmap.DoCommand(
      new CommandDeleteSelected(
        qtconceptmap.GetConceptMap(),
        qtconceptmap.GetScene(),
        qtconceptmap.GetQtToolItem()
      )
    );
    assert(DoubleCheckEdgesAndNodes(qtconceptmap,0,0));
    qtconceptmap.Undo();
    assert(DoubleCheckEdgesAndNodes(qtconceptmap,0,n));
  }
  if (verbose) { TRACE("Delete Edge"); }
  {
    QtConceptMap qtconceptmap;
    assert(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,0,0));
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
    assert(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,0,2));
    qtconceptmap.DoCommand(
      new CommandCreateNewEdgeBetweenTwoSelectedNodes(
        qtconceptmap.GetConceptMap(),
        qtconceptmap.GetScene(),
        qtconceptmap.GetQtToolItem()
      )
    );
    assert(DoubleCheckEdgesAndNodes(qtconceptmap,1,2));
    assert(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,1,0));
    qtconceptmap.DoCommand(
      new CommandDeleteSelected(
        qtconceptmap.GetConceptMap(),
        qtconceptmap.GetScene(),
        qtconceptmap.GetQtToolItem()
      )
    );
    assert(DoubleCheckEdgesAndNodes(qtconceptmap,0,2));
  }
  #ifdef NOT_NOW_20151230
  #ifdef FIX_ISSUE_10
  if (verbose) { TRACE("DeleteNode: delete a Node from ConceptMap using a Command"); }
  {
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    QtConceptMap qtconceptmap;
    qtconceptmap.SetConceptMap(conceptmap);
    const auto node1 = NodeFactory().GetTest(0);
    const auto node2 = NodeFactory().GetTest(0);
    const auto edge = EdgeFactory().GetTest(0,node1,node2);

    qtconceptmap.AddNode(node1);
    qtconceptmap.AddNode(node2);
    qtconceptmap.AddEdge(edge);

    assert(conceptmap.GetNodes().size() == 2);
    assert(conceptmap.GetEdges().size() == 1);
    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());
    assert(conceptmap.GetEdges().size() == qtconceptmap.GetQtEdges().size());

    const auto command = new CommandDeleteNode(conceptmap,node1);
    qtconceptmap.DoCommand(command);


    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());
    assert(conceptmap.GetEdges().size() == qtconceptmap.GetQtEdges().size());
    assert(conceptmap.GetNodes().size() == 1);
    assert(qtconceptmap.GetQtEdges().size() == 0);
  }
  if (verbose) { TRACE("DeleteNode: create two Nodes and Edge, delete one Node from ConceptMap"); }
  {
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    QtConceptMap qtconceptmap;
    qtconceptmap.SetConceptMap(conceptmap);
    const auto node1 = NodeFactory().GetTest(0);
    const auto node2 = NodeFactory().GetTest(0);
    const auto edge = EdgeFactory().GetTest(0,node1,node2);

    const auto qtnode1 = qtconceptmap.AddNode(node1);
    const auto qtnode2 = qtconceptmap.AddNode(node2);
    const auto qtedge = qtconceptmap.AddEdge(edge);

    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());
    assert(conceptmap.GetEdges().size() == qtconceptmap.GetQtEdges().size());
    assert(conceptmap.GetNodes().size() == 2);
    assert(qtconceptmap.GetQtEdges().size() == 1);

    qtconceptmap.DeleteNode(node1);

    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());
    assert(conceptmap.GetEdges().size() == qtconceptmap.GetQtEdges().size());
    assert(conceptmap.GetNodes().size() == 1);
    assert(qtconceptmap.GetQtEdges().size() == 0);
    assert(!qtnode1->scene());
    assert( qtnode2->scene()); //node2 is left over
    assert(!qtedge->scene());

  }
  if (verbose) { TRACE("AddEdge: an Edge added has to end up in both ConceptMap and QtConceptMap, by adding in QtConceptMap"); }
  {
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    QtConceptMap qtconceptmap;
    qtconceptmap.SetConceptMap(conceptmap);

    assert(conceptmap.GetEdges().size() == qtconceptmap.GetQtEdges().size());
    const int n_edges_before{static_cast<int>(qtconceptmap.GetQtEdges().size())};

    const auto from = NodeFactory().GetTest(0);
    const auto to = NodeFactory().GetTest(0);
    const auto edge = EdgeFactory().GetTest(0,from,to);
    qtconceptmap.AddEdge(edge);

    assert(conceptmap.GetEdges().size() == qtconceptmap.GetQtEdges().size());
    const int n_edges_after{static_cast<int>(qtconceptmap.GetQtEdges().size())};
    assert(n_edges_after == n_edges_before + 1);
  }
  if (verbose) { TRACE("AddEdge: an Edge added has to end up in both ConceptMap and QtConceptMap, by adding in ConceptMap"); }
  {
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    QtConceptMap qtconceptmap;
    qtconceptmap.SetConceptMap(conceptmap);

    assert(conceptmap.GetEdges().size() == qtconceptmap.GetQtEdges().size());
    const int n_edges_before{static_cast<int>(qtconceptmap.GetQtEdges().size())};

    const auto from = NodeFactory().GetTest(0);
    const auto to = NodeFactory().GetTest(0);
    const auto edge = EdgeFactory().GetTest(0,from,to);
    conceptmap.AddEdge(edge);

    assert(conceptmap.GetEdges().size() == qtconceptmap.GetQtEdges().size());
    const int n_edges_after{static_cast<int>(qtconceptmap.GetQtEdges().size())};
    assert(n_edges_after == n_edges_before + 1);
  }
  if (verbose) { TRACE("AddEdge: an Edge added has to end up in both ConceptMap and QtConceptMap, by adding in both places, ConceptMap first"); }
  {
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    QtConceptMap qtconceptmap;
    qtconceptmap.SetConceptMap(conceptmap);

    assert(conceptmap.GetEdges().size() == qtconceptmap.GetQtEdges().size());
    const int n_edges_before{static_cast<int>(qtconceptmap.GetQtEdges().size())};

    const auto from = NodeFactory().GetTest(0);
    const auto to = NodeFactory().GetTest(0);
    const auto edge = EdgeFactory().GetTest(0,from,to);
    conceptmap.AddEdge(edge);
    qtconceptmap.AddEdge(edge);

    assert(conceptmap.GetEdges().size() == qtconceptmap.GetQtEdges().size());
    const int n_edges_after{static_cast<int>(qtconceptmap.GetQtEdges().size())};
    assert(n_edges_after == n_edges_before + 1);
  }
  if (verbose) { TRACE("AddEdge: an Edge added has to end up in both ConceptMap and QtConceptMap, by adding in both places, QtConceptMap first"); }
  {
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    QtConceptMap qtconceptmap;
    qtconceptmap.SetConceptMap(conceptmap);

    assert(conceptmap.GetEdges().size() == qtconceptmap.GetQtEdges().size());
    const int n_edges_before{static_cast<int>(qtconceptmap.GetQtEdges().size())};

    const auto from = NodeFactory().GetTest(0);
    const auto to = NodeFactory().GetTest(0);
    const auto edge = EdgeFactory().GetTest(0,from,to);
    qtconceptmap.AddEdge(edge);
    //conceptmap.AddEdge(edge); //Correctly gives a warning

    assert(conceptmap.GetEdges().size() == qtconceptmap.GetQtEdges().size());
    const int n_edges_after{static_cast<int>(qtconceptmap.GetQtEdges().size())};
    assert(n_edges_after == n_edges_before + 1);
  }
  if (verbose) { TRACE("AddEdge: added QtEdge must get selected"); }
  {
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    QtConceptMap qtconceptmap;
    qtconceptmap.SetConceptMap(conceptmap);

    const auto from = NodeFactory().GetTest(0);
    const auto to = NodeFactory().GetTest(0);
    qtconceptmap.AddNode(from);
    qtconceptmap.AddNode(to);
    const auto edge = EdgeFactory().GetTest(0,from,to);
    const auto qtedge = qtconceptmap.AddEdge(edge);

    assert(conceptmap.GetSelectedEdges().size() == 1);
    assert(qtedge->isSelected() || qtedge->GetQtNode()->isSelected()); //Cannot choose which one is better
  }
  if (verbose) { TRACE("AddEdge: added QtEdge must unselect QtNodes"); }
  {
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    QtConceptMap qtconceptmap;
    qtconceptmap.SetConceptMap(conceptmap);

    const auto from = NodeFactory().GetTest(0);
    const auto to = NodeFactory().GetTest(0);
    const auto edge = EdgeFactory().GetTest(0,from,to);
    const auto qtfrom = qtconceptmap.AddNode(from);
    const auto qtto = qtconceptmap.AddNode(to);

    assert(qtfrom->isSelected());
    assert(qtto->isSelected());

    const auto qtedge = qtconceptmap.AddEdge(edge);

    assert(!qtfrom->isSelected());
    assert(!qtto->isSelected());
    assert(qtedge->isSelected());
    assert(conceptmap.GetSelectedNodes().size() == 0);
    assert(conceptmap.GetSelectedEdges().size() == 1);
  }
  if (verbose) { TRACE("AddEdge: selecting and unselect QtEdge must also select the Edge"); }
  {
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    QtConceptMap qtconceptmap;
    qtconceptmap.SetConceptMap(conceptmap);

    const auto from = NodeFactory().GetTest(0);
    const auto to = NodeFactory().GetTest(0);
    const auto edge = EdgeFactory().GetTest(0,from,to);
    const auto qtedge = qtconceptmap.AddEdge(edge);

    assert(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
    assert(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());

    assert(qtedge->isSelected());

    qtedge->SetSelected(false);

    assert(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
    assert(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());
    assert(!qtedge->isSelected());

    qtedge->SetSelected(true);

    assert(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
    assert(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());
    assert(qtedge->isSelected());
  }
  if (verbose) { TRACE("AddEdge: a selected QtEdge must also be detected by QScene::selectedItems"); }
  {
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    QtConceptMap qtconceptmap;
    qtconceptmap.SetConceptMap(conceptmap);

    const auto from = NodeFactory().GetTest(0);
    const auto to = NodeFactory().GetTest(0);
    const auto edge = EdgeFactory().GetTest(0,from,to);
    const auto qtedge = qtconceptmap.AddEdge(edge);

    //Fails if item is invisible
    qtconceptmap.showFullScreen();
    for (int i=0; i!=1000; ++i) qApp->processEvents();
    assert(qtconceptmap.scene()->selectedItems().size() == 1);

    qtedge->SetSelected(false);

    assert(qtconceptmap.scene()->selectedItems().size() == 0);

    qtedge->SetSelected(true);

    assert(qtconceptmap.scene()->selectedItems().size() == 1);
  }
  if (verbose) { TRACE("AddEdge: QtEdge its QtNode must be in between the QtNodes"); }
  {
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    QtConceptMap qtconceptmap;
    qtconceptmap.SetConceptMap(conceptmap);

    const auto from = NodeFactory().GetTest(0);
    const auto to = NodeFactory().GetTest(0);
    const auto qtfrom = qtconceptmap.AddNode(from);
    const auto qtto = qtconceptmap.AddNode(to);

    const double x1{100.0};
    const double y1{200.0};
    const double x2{300.0};
    const double y2{400.0};
    const double x3{(x1+x2)/2.0};
    const double y3{(y1+y2)/2.0};
    qtfrom->GetNode().SetX(x1);
    qtfrom->GetNode().SetY(y1);
    qtto->GetNode().SetX(x2);
    qtto->GetNode().SetY(y2);

    const auto edge = EdgeFactory().Create(from,to);

    assert(std::abs(edge->GetNode().GetX() - x3) < 1.0);
    assert(std::abs(edge->GetNode().GetY() - y3) < 1.0);

    const auto qtedge = qtconceptmap.AddEdge(edge);

    assert(std::abs(qtedge->GetQtNode()->GetNode().GetX() - x3) < 1.0);
    assert(std::abs(qtedge->GetQtNode()->GetNode().GetY() - y3) < 1.0);
  }
  if (verbose) { TRACE("QtNode and Node coordinats must be in sync"); }
  {
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    QtConceptMap qtconceptmap;
    qtconceptmap.SetConceptMap(conceptmap);

    Node node = NodeFactory().GetTest(0);
    const auto qtnode = qtconceptmap.AddNode(node);

    const double x1{100.0};
    const double y1{200.0};
    const double max_error{0.1};
    qtnode->GetNode().SetX(x1);
    qtnode->GetNode().SetY(y1);
    {
      const double new_x = 12.34;
      const double new_y = 43.21;

      //Change via node
      node.SetX(new_x);
      node.SetY(new_y);

      const double node_x = node.GetX();
      const double qtnode_x = qtnode->GetCenterX();

      if (std::abs(node_x - qtnode_x) >= max_error)
      {
        TRACE(node_x);
        TRACE(qtnode_x);
      }
      assert(std::abs(node_x - qtnode_x) < max_error
       && "X coordinat must be in sync");
      const double node_y = node.GetY();
      const double qtnode_y = qtnode->GetCenterY();

      assert(node_y == qtnode_y
       && "Y coordinat must be in sync");
    }
    //Change via Qt node
    {
      const double new_x = 123.456;
      const double new_y = 654.321;

      qtnode->SetCenterPos(new_x,new_y);

      const double node_x = node.GetX();
      const double qtnode_x = qtnode->GetCenterX();

      assert(std::abs(node_x - qtnode_x) < max_error
       && "X coordinat must be in sync");
      const double node_y = node.GetY();
      const double qtnode_y = qtnode->GetCenterY();

      assert(std::abs(node_y - qtnode_y) < max_error
       && "Y coordinat must be in sync");
    }

  }
  if (verbose) { TRACE("DeleteEdge: delete of QtEdge from QtConceptMap"); }
  {
    QtConceptMap qtconceptmap;
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetHeteromorphousTestConceptMap(10);
    qtconceptmap.SetConceptMap(conceptmap);

    assert(conceptmap.GetEdges().size() == 2);
    assert(conceptmap.GetEdges().size() == qtconceptmap.GetQtEdges().size());

    const auto qtedge1 = qtconceptmap.GetQtEdges().back();
    qtconceptmap.DeleteQtEdge(qtedge1);

    assert(conceptmap.GetEdges().size() == 1);
    assert(conceptmap.GetEdges().size() == qtconceptmap.GetQtEdges().size());

    const auto qtedge2 = qtconceptmap.GetQtEdges().back();
    qtconceptmap.DeleteQtEdge(qtedge2);

    assert(conceptmap.GetEdges().size() == 0);
    assert(conceptmap.GetEdges().size() == qtconceptmap.GetQtEdges().size());
  }
  //Test Undo functionality of commands
  if (verbose) { TRACE("Undo CommandCreateNewNode"); }
  {
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    QtConceptMap qtconceptmap;
    qtconceptmap.SetConceptMap(conceptmap);

    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());
    const int n_nodes_before{static_cast<int>(qtconceptmap.GetQtNodes().size())};

    const auto command = new CommandCreateNewNode(conceptmap);
    qtconceptmap.DoCommand(command);

    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());
    const int n_nodes_after{static_cast<int>(qtconceptmap.GetQtNodes().size())};
    assert(n_nodes_after == n_nodes_before + 1);

    command->undo();

    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());
    const int n_nodes_again{static_cast<int>(qtconceptmap.GetQtNodes().size())};
    assert(n_nodes_again == n_nodes_before);
  }
  //
  //
  //Selection by more direct/dirtier methods
  //
  //
  if (verbose) { TRACE("Selecting a QtNode also updates the ConceptMap::m_selected"); }
  {
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    QtConceptMap qtconceptmap;
    qtconceptmap.SetConceptMap(conceptmap);
    auto qtnode = qtconceptmap.AddNode(NodeFactory().GetTest(0));

    assert(conceptmap.GetSelectedNodes().size() == 1);
    assert(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());

    qtnode->SetSelected(false);

    assert(conceptmap.GetSelectedNodes().size() == 0);
    assert(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());

    qtnode->SetSelected(true);

    assert(conceptmap.GetSelectedNodes().size() == 1);
    assert(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
    assert(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());
  }
  if (verbose) { TRACE("Unselecting a QtNode also updates the ConceptMap::m_selected"); }
  {
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    QtConceptMap qtconceptmap;
    qtconceptmap.SetConceptMap(conceptmap);
    auto qtnode = qtconceptmap.AddNode(NodeFactory().GetTest(0));

    assert(conceptmap.GetSelectedNodes().size() == 1);
    assert(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());

    qtnode->SetSelected(true);

    assert(conceptmap.GetSelectedNodes().size() == 1);
    assert(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
    assert(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());

    qtnode->SetSelected(false);

    assert(conceptmap.GetSelectedNodes().size() == 0);
    assert(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
  }
  if (verbose) { TRACE("Seleting a QtNode also updates the ConceptMap::m_selected"); }
  {
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    QtConceptMap qtconceptmap;
    qtconceptmap.SetConceptMap(conceptmap);
    auto qtfrom = qtconceptmap.AddNode(NodeFactory().GetTest(0));
    auto qtto = qtconceptmap.AddNode(NodeFactory().GetTest(1));
    auto qtedge = qtconceptmap.AddEdge(EdgeFactory().GetTest(0,qtfrom->GetNode(),qtto->GetNode()));

    assert(conceptmap.GetSelectedNodes().size() == 0);
    assert(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
    assert(conceptmap.GetSelectedEdges().size() == 1);
    assert(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());

    qtedge->SetSelected(false);

    assert(conceptmap.GetSelectedNodes().size() == 0);
    assert(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
    assert(conceptmap.GetSelectedEdges().size() == 0);
    assert(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());

    qtedge->SetSelected(true);

    assert(conceptmap.GetSelectedNodes().size() == 0);
    assert(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
    assert(conceptmap.GetSelectedEdges().size() == 1);
    assert(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());
  }
  //
  //
  //QWidget Events; direct GUI responses. Note: these are done by Commands anyways,
  //so it may be hard to create a test that breaks here)
  //
  if (verbose) { TRACE("CTRL-N creates a new command in ConceptMap its QUndoStack"); }
  {
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    QtConceptMap qtconceptmap;
    qtconceptmap.SetConceptMap(conceptmap);

    auto ctrl_n = CreateControlN();
    qtconceptmap.keyPressEvent(&ctrl_n);

    assert(conceptmap.GetUndo().count() == 1);
  }

  if (verbose) { TRACE("CTRL-N creates a new QtNode"); }
  {
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    QtConceptMap qtconceptmap;
    qtconceptmap.SetConceptMap(conceptmap);

    assert(conceptmap.GetNodes().size() == 0);
    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());

    auto ctrl_n = CreateControlN();
    qtconceptmap.keyPressEvent(&ctrl_n);

    assert(conceptmap.GetNodes().size() == 1);
    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());
    assert(conceptmap.GetSelectedEdges().size() == 0);
    assert(conceptmap.GetSelectedNodes().size() == 1);
    assert(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
    assert(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());
  }
  if (verbose) { TRACE("CTRL-N, CTRL-N creates two QtNodes"); }
  {
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    QtConceptMap qtconceptmap;
    qtconceptmap.SetConceptMap(conceptmap);

    assert(conceptmap.GetNodes().size() == 0);
    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());

    auto ctrl_n = CreateControlN();
    qtconceptmap.keyPressEvent(&ctrl_n);

    assert(conceptmap.GetNodes().size() == 1);
    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());

    qtconceptmap.keyPressEvent(&ctrl_n);

    assert(conceptmap.GetNodes().size() == 2);
    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());

    assert(conceptmap.GetSelectedEdges().size() == 0);
    assert(conceptmap.GetSelectedNodes().size() == 2);
    assert(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
    assert(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());
  }
  if (verbose) { TRACE("CTRL-N, CTRL-Down: QtNode must move down"); }
  {
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    QtConceptMap qtconceptmap;
    qtconceptmap.SetConceptMap(conceptmap);
    qtconceptmap.showFullScreen();
    auto ctrl_n = CreateControlN();
    qtconceptmap.keyPressEvent(&ctrl_n);

    assert(conceptmap.GetNodes().size() == 1);
    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());
    assert(qtconceptmap.GetQtNodes()[0]->x() == qtconceptmap.GetConceptMap()->GetNodes()[0].GetX());
    assert(qtconceptmap.GetQtNodes()[0]->y() == qtconceptmap.GetConceptMap()->GetNodes()[0].GetY());

    const double x_before{qtconceptmap.GetQtNodes()[0]->x()};
    const double y_before{qtconceptmap.GetQtNodes()[0]->y()};

    auto ctrl_down = CreateControlDown();
    qtconceptmap.keyPressEvent(&ctrl_down);

    const double x_after{qtconceptmap.GetQtNodes()[0]->x()};
    const double y_after{qtconceptmap.GetQtNodes()[0]->y()};
    assert(x_after == x_before);
    assert(y_after > y_before);
  }
  if (verbose) { TRACE("CTRL-N, CTRL-N, delete, creates two QtNodes and deletes two"); }
  {
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    QtConceptMap qtconceptmap;
    qtconceptmap.SetConceptMap(conceptmap);

    assert(conceptmap.GetNodes().size() == 0);

    auto ctrl_n = CreateControlN();
    qtconceptmap.keyPressEvent(&ctrl_n);
    qtconceptmap.keyPressEvent(&ctrl_n);

    assert(conceptmap.GetNodes().size() == 2);
    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());
    assert(conceptmap.GetSelectedNodes().size() == 2);
    assert(conceptmap.GetSelectedEdges().size() == 0);
    assert(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
    assert(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());

    auto del = CreateDel();
    qtconceptmap.keyPressEvent(&del);

    assert(conceptmap.GetNodes().size() == 0);
    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());
    assert(conceptmap.GetSelectedNodes().size() == 0);
    assert(conceptmap.GetSelectedEdges().size() == 0);
    assert(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
    assert(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());
  }
  if (verbose) { TRACE("CTRL-N, CTRL-N, space, delete, creates two QtNodes, selects one and deletes one"); }
  {
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    QtConceptMap qtconceptmap;
    qtconceptmap.SetConceptMap(conceptmap);

    assert(conceptmap.GetNodes().size() == 0);

    auto ctrl_n = CreateControlN();
    qtconceptmap.keyPressEvent(&ctrl_n);
    qtconceptmap.keyPressEvent(&ctrl_n);

    assert(conceptmap.GetNodes().size() == 2);
    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());
    assert(conceptmap.GetSelectedNodes().size() == 2);
    assert(conceptmap.GetSelectedEdges().size() == 0);
    assert(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
    assert(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());

    auto space = CreateSpace();
    qtconceptmap.keyPressEvent(&space);

    TRACE(qtconceptmap.GetSelectedQtNodes().size());
    TRACE(conceptmap.GetSelectedNodes().size());

    assert(conceptmap.GetNodes().size() == 2);
    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());
    assert(conceptmap.GetSelectedNodes().size() == 1);
    assert(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
    assert(conceptmap.GetSelectedEdges().size() == 0);
    assert(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());

    auto del = CreateDel();
    qtconceptmap.keyPressEvent(&del);

    assert(conceptmap.GetNodes().size() == 1);
    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());
    assert(conceptmap.GetSelectedNodes().size() == 1);
    assert(conceptmap.GetSelectedEdges().size() == 0);
    assert(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
    assert(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());

  }
  if (verbose) { TRACE("CTRL-N, CTRL-N, CTRL-E: new Edge should be selected"); }
  {
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    QtConceptMap qtconceptmap;
    qtconceptmap.SetConceptMap(conceptmap);

    assert(conceptmap.GetNodes().size() == 0);
    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());
    assert(conceptmap.GetEdges().size() == 0);
    assert(conceptmap.GetEdges().size() == qtconceptmap.GetQtEdges().size());

    auto ctrl_n = CreateControlN();
    qtconceptmap.keyPressEvent(&ctrl_n);
    qtconceptmap.keyPressEvent(&ctrl_n);

    auto ctrl_e = CreateControlE();
    qtconceptmap.keyPressEvent(&ctrl_e);

    assert(conceptmap.GetNodes().size() == 2);
    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());
    assert(conceptmap.GetEdges().size() == 1);
    assert(conceptmap.GetEdges().size() == qtconceptmap.GetQtEdges().size());
    assert(conceptmap.GetSelectedNodes().size() == 0);
    assert(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
    assert(conceptmap.GetSelectedEdges().size() == 1);
    assert(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());
  }
  if (verbose) { TRACE("CTRL-N, CTRL-N, CTRL-E: new Edge its QtNode must be between the two QtNodes"); }
  {
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    QtConceptMap qtconceptmap;
    qtconceptmap.SetConceptMap(conceptmap);
    auto ctrl_n = CreateControlN();
    qtconceptmap.keyPressEvent(&ctrl_n);
    qtconceptmap.keyPressEvent(&ctrl_n);


    assert(conceptmap.GetNodes().size() == 2);
    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());
    const auto qtfrom = qtconceptmap.GetQtNodes()[0];
    const auto qtto = qtconceptmap.GetQtNodes()[1];

    const double x1{100.0};
    const double y1{200.0};
    const double x2{300.0};
    const double y2{400.0};
    const double x3{(x1+x2)/2.0};
    const double y3{(y1+y2)/2.0};
    qtfrom->GetNode().SetPos(x1,y1);
    qtto->GetNode().SetPos(x2,y2);

    auto ctrl_e = CreateControlE();
    qtconceptmap.keyPressEvent(&ctrl_e);

    assert(conceptmap.GetEdges().size() == 1);
    assert(conceptmap.GetEdges().size() == qtconceptmap.GetQtEdges().size());
    const auto qtedge = qtconceptmap.GetQtEdges()[0];

    assert(std::abs(qtedge->GetQtNode()->x() - x3) < 1.0);
    assert(std::abs(qtedge->GetQtNode()->y() - y3) < 1.0);
    assert(std::abs(qtedge->GetQtNode()->GetNode().GetX() - x3) < 1.0);
    assert(std::abs(qtedge->GetQtNode()->GetNode().GetY() - y3) < 1.0);
  }
  if (verbose) { TRACE("CTRL-N, CTRL-N, Down 10x, CTRL-E: new Edge its QtNode must be between the two QtNodes"); }
  if (!"Not sure of the usefulness of this test")
  {
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    QtConceptMap qtconceptmap;
    qtconceptmap.SetConceptMap(conceptmap);
    qtconceptmap.showFullScreen();
    auto ctrl_n = CreateControlN();

    qtconceptmap.keyPressEvent(&ctrl_n);
    qtconceptmap.keyPressEvent(&ctrl_n);

    const auto qtfrom = qtconceptmap.GetQtNodes()[0];
    const auto qtto = qtconceptmap.GetQtNodes()[1];

    for (int i=0; i!=100; ++i)
    {
      TRACE(qtfrom->GetNode().GetY());
      TRACE(qtto->GetNode().GetY());

      auto ctrl_down = CreateControlDown();
      qtconceptmap.keyPressEvent(&ctrl_down);
      //for (int j=0; j!=1000; ++j) { qApp->processEvents(); }
      //qtconceptmap.showFullScreen();
    }
    assert(qtfrom->GetNode().GetY() > 100.0);
    assert(qtto->GetNode().GetY() > 100.0);

    auto ctrl_e = CreateControlE();
    qtconceptmap.keyPressEvent(&ctrl_e);

    assert(conceptmap.GetEdges().size() == 1);
    assert(conceptmap.GetEdges().size() == qtconceptmap.GetQtEdges().size());
    const auto qtedge = qtconceptmap.GetQtEdges()[0];

    assert(std::abs(qtedge->GetQtNode()->y() > 1000.0));
    assert(!"Green");
  }
  if (verbose) { TRACE("Delete Edge and Node-that-is-head-of-Edge: delete from QtConceptMap"); }
  {
    QtConceptMap qtconceptmap;
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    qtconceptmap.SetConceptMap(conceptmap);

    const auto from = NodeFactory().GetTest(0);
    const auto to = NodeFactory().GetTest(0);
    qtconceptmap.AddNode(from);
    qtconceptmap.AddNode(to);
    qtconceptmap.AddEdge(EdgeFactory().GetTest(0,from,to));
    conceptmap.AddSelected( ConceptMap::Nodes( { from } ) );

    assert(conceptmap.GetNodes().size() == 2);
    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());
    assert(conceptmap.GetEdges().size() == 1);
    assert(conceptmap.GetEdges().size() == qtconceptmap.GetQtEdges().size());
    assert(conceptmap.GetSelectedNodes().size() == 1);
    assert(conceptmap.GetSelectedEdges().size() == 1);

    auto del = CreateDel();
    qtconceptmap.keyPressEvent(&del);

    assert(conceptmap.GetNodes().size() == 1);
    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());
    assert(conceptmap.GetEdges().size() == 0);
    assert(conceptmap.GetEdges().size() == qtconceptmap.GetQtEdges().size());
    assert(conceptmap.GetSelectedNodes().size() == 1);
    assert(conceptmap.GetSelectedEdges().size() == 0);
  }
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

    assert(qtconceptmap.GetQtNodes().size() == 2);
    qtconceptmap.GetQtNodes()[0]->SetCenterPos(-100.0,0.0);
    qtconceptmap.GetQtNodes()[0]->setToolTip("QtNodes[0]");
    qtconceptmap.GetQtNodes()[1]->SetCenterPos( 100.0,0.0);
    qtconceptmap.GetQtNodes()[1]->setToolTip("QtNodes[1]");
    qtconceptmap.GetQtEdges()[0]->setToolTip("QtEdges[0]");
    qtconceptmap.GetQtEdges()[0]->GetQtNode()->setToolTip("QtEdges[0] its center QtNode");

    assert(conceptmap.GetNodes().size() == 2);
    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());
    assert(conceptmap.GetEdges().size() == 1);
    assert(conceptmap.GetEdges().size() == qtconceptmap.GetQtEdges().size());
    assert(conceptmap.GetSelectedNodes().size() == 0);
    assert(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
    assert(conceptmap.GetSelectedEdges().size() == 1);
    assert(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());

    //Fails if item is invisible
    qtconceptmap.showFullScreen();
    for (int i=0; i!=100; ++i) qApp->processEvents();
    assert(qtconceptmap.scene()->selectedItems().count() > 0);


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


    assert(conceptmap.GetNodes().size() == 2);
    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());
    assert(conceptmap.GetEdges().size() == 1);
    assert(conceptmap.GetEdges().size() == qtconceptmap.GetQtEdges().size());
    assert(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
    assert(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());
    assert(conceptmap.GetSelectedNodes().size() == 1);
    assert(conceptmap.GetSelectedEdges().size() == 0);
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

    assert(conceptmap.GetNodes().size() == 2);
    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());
    assert(conceptmap.GetEdges().size() == 1);
    assert(conceptmap.GetEdges().size() == qtconceptmap.GetQtEdges().size());
    assert(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
    assert(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());
    assert(conceptmap.GetSelectedNodes().size() == 0);
    assert(conceptmap.GetSelectedEdges().size() == 1);


    //Select a node
    auto left = CreateLeft();
    qtconceptmap.keyPressEvent(&left);

    TRACE(conceptmap.GetSelectedNodes().size());
    TRACE(qtconceptmap.GetSelectedQtNodes().size());
    TRACE(conceptmap.GetSelectedEdges().size());
    TRACE(qtconceptmap.GetSelectedQtEdges().size());

    assert(conceptmap.GetNodes().size() == 2);
    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());
    assert(conceptmap.GetEdges().size() == 1);
    assert(conceptmap.GetEdges().size() == qtconceptmap.GetQtEdges().size());
    assert(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
    assert(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());
    assert(conceptmap.GetSelectedNodes().size() == 1);
    assert(conceptmap.GetSelectedEdges().size() == 0);

    //Delete the node, edge will be deleted as well
    auto del = CreateDel();
    qtconceptmap.keyPressEvent(&del);

    //Undo the deletion, should bring back node and edge
    auto cntrlz = CreateControlZ();
    qtconceptmap.keyPressEvent(&cntrlz);

    assert(conceptmap.GetNodes().size() == 2);
    assert(conceptmap.GetNodes().size() == qtconceptmap.GetQtNodes().size());
    assert(conceptmap.GetEdges().size() == 1);
    assert(conceptmap.GetEdges().size() == qtconceptmap.GetQtEdges().size());
    assert(conceptmap.GetSelectedNodes().size() == 1);
    assert(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodes().size());
    assert(conceptmap.GetSelectedEdges().size() == 0);
    assert(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());

  }
  assert(!"Fixed issue #1");
  #endif // FIX_ISSUE_1

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
    const int n_nodes_in_conceptmap{static_cast<int>(m.GetConceptMap()->GetNodes().size())};
    assert(n_nodes_in_scene == n_nodes_in_conceptmap);
  }
  #endif
  #endif //FIX_ISSUE_10
  TestTimer::SetMaxCnt(1); //Because the base class has been tested now
#endif // NOT_NOW_20151230

}
#endif
