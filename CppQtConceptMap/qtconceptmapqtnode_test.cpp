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
#include "qtconceptmapqtnode.h"

#include <climits>

#include <boost/lambda/lambda.hpp>
#include <boost/signals2.hpp>

#include <QCursor>
#include <QKeyEvent>
#include <QPainter>
#include <QPen>

#include "conceptmapconcept.h"
#include "conceptmapexamples.h"
#include "conceptmapnodefactory.h"
#include "conceptmapnode.h"
#include "container.h"
#include "qtconceptmapbrushfactory.h"
#include "qtconceptmapeditstrategy.h"
#include "qtconceptmapqtnodefactory.h"
#include "qtconceptmapratestrategy.h"
#include "qtitemdisplaystrategy.h"
#include "testtimer.h"
#include "trace.h"

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_CASE(ribi_cmap_qtedge_dialog_test)
{
  using namespace ribi;
  using namespace ribi::cmap;
  const bool verbose{false};
  const double max_error = 2.0;

  if (verbose) { TRACE("QtNode can be converted to QtRoundedEditRectItem") }
  {
    const auto qtnode = QtNodeFactory().GetTest(1);
    const boost::shared_ptr<QtRoundedEditRectItem> edit_rect{boost::dynamic_pointer_cast<QtRoundedEditRectItem>(qtnode)};
    BOOST_CHECK(edit_rect);
    BOOST_CHECK(qtnode == edit_rect);
  }

  if (verbose) { TRACE("Test X coordinat in Node and QtRoundedEditRectItem being equal at creation") }
  {
    const auto qtnode = QtNodeFactory().GetTest(1);
    const boost::shared_ptr<QtRoundedEditRectItem> edit_rect{boost::dynamic_pointer_cast<QtRoundedEditRectItem>(qtnode)};
    const auto node = qtnode->GetNode();
    const double node_x = node.GetX();
    const double edit_rect_x = edit_rect->GetCenterX();
    BOOST_CHECK(std::abs(node_x - edit_rect_x) < max_error);
  }
  if (verbose) { TRACE("Test Y coordinat in Node and QtRoundedEditRectItem being equal at creation") }
  {
    const auto qtnode = QtNodeFactory().GetTest(1);
    const boost::shared_ptr<QtRoundedEditRectItem> edit_rect{boost::dynamic_pointer_cast<QtRoundedEditRectItem>(qtnode)};
    const auto node = qtnode->GetNode();
    const double node_y = node.GetY();
    const double edit_rect_y = edit_rect->GetCenterY();
    BOOST_CHECK(std::abs(node_y - edit_rect_y) < max_error);
  }
  if (verbose) {TRACE("When changing the concept's name via QtNode, the Node must be changed as well");}
  {
    const auto qtnode = QtNodeFactory().GetTest(1);
    const boost::shared_ptr<QtRoundedEditRectItem> qtrectitem{boost::dynamic_pointer_cast<QtRoundedEditRectItem>(qtnode)};
    const std::string old_name = qtrectitem->GetText()[0];
    const std::string new_name{old_name + " (modified)"};
    qtrectitem->SetText( { new_name } );
    const std::string new_name_again{qtnode->GetNode().GetConcept().GetName()};
    BOOST_CHECK(new_name_again == new_name);
  }
}

std::string ribi::cmap::QtNode::ToStr() const noexcept
{
  std::stringstream s;
  s << (*this);
  return s.str();
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const QtNode& qtnode) noexcept
{
  os
    << qtnode.GetNode()
  ;
  return os;
}
