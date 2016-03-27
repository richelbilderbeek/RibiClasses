//---------------------------------------------------------------------------
/*
QtRoundedEditRectItem, editable rectangular-shaped QGraphicsItem
Copyright (C) 2012-2016 Richel Bilderbeek

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
//From http://www.richelbilderbeek.nl/QtRoundedEditRectItem.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtroundededitrectitem.h"
#include <boost/test/unit_test.hpp>

#include "counter.h"
#include "testtimer.h"
#include "trace.h"
#pragma GCC diagnostic pop

BOOST_AUTO_TEST_CASE(ribi_qtrounded_edit_rect_test)
{
  using namespace ribi;
  //using namespace ribi::QtRoundedEditRectItem;
  const bool verbose{false};
  QtRoundedEditRectItem item;
  if (verbose) { TRACE("Set/Get Text must be symmetric"); }
  {
    const auto old_text = item.GetText();
    auto new_text = old_text;
    new_text[0] += " (modified)";
    item.SetText(new_text);
    BOOST_CHECK(item.GetText() == new_text);
  }
  if (verbose) { TRACE("Setting two increasing-length single-line texts should increase the inner width"); }
  {
    item.SetText( {"A"} );
    const double old_width{item.GetInnerWidth()};
    item.SetText( {"AB"} );
    BOOST_CHECK(old_width < item.GetInnerWidth());
  }
  if (verbose) { TRACE("Setting two decreasing-length single-line texts should decrease the inner width"); }
  {
    item.SetText( {"AB"} );
    const double old_width{item.GetInnerWidth()};
    item.SetText( {"A"} );
    BOOST_CHECK(old_width > item.GetInnerWidth());
  }
  if (verbose) { TRACE("Setting two increasing-length single-line texts should keep the inner height the same"); }
  {
    item.SetText( {"A"} );
    const double old_height{item.GetInnerHeight()};
    item.SetText( {"AB"} );
    BOOST_CHECK(old_height == item.GetInnerHeight());
  }
  if (verbose) { TRACE("A text rectangle of a single-line text must have its center at origin"); }
  {
    const auto center = QtRoundedEditRectItem::GetTextRectAtOrigin("ABCDEFG",QFont()).center();
    BOOST_CHECK(center.x() >= -2.0);
    BOOST_CHECK(center.x() <=  2.0);
    BOOST_CHECK(center.y() >= -2.0);
    BOOST_CHECK(center.y() <=  2.0);
  }
  if (verbose) { TRACE("A text rectangle of a multi-line text must have its center at origin"); }
  {
    const auto center = QtRoundedEditRectItem::GetTextRectAtOrigin(
      { "ABCDEFG", "ABCDEFG", "ABCDEFG" },
    QFont()).center();
    BOOST_CHECK(center.x() >= -2.0);
    BOOST_CHECK(center.x() <=  2.0);
    BOOST_CHECK(center.y() >= -2.0);
    BOOST_CHECK(center.y() <=  2.0);
  }
  if (verbose) { TRACE("A padded text rectangle of a multi-line text, line 1/3 must have its center above origin"); }
  {
    const auto center = QtRoundedEditRectItem::GetPaddedTextRectAtLine("ABCDEFG",QFont(),0,3).center();
    //TRACE(center.x());
    //TRACE(center.y());
    BOOST_CHECK(center.x() >= -2.0);
    BOOST_CHECK(center.x() <=  2.0);
    BOOST_CHECK(center.y() <   0.0);
  }
  if (verbose) { TRACE("A padded text rectangle of a multi-line text, line 3/3 must have its center above origin"); }
  {
    const auto center = QtRoundedEditRectItem::GetPaddedTextRectAtLine("ABCDEFG",QFont(),2,3).center();
    //TRACE(center.x());
    //TRACE(center.y());
    BOOST_CHECK(center.x() >= -2.0);
    BOOST_CHECK(center.x() <=  2.0);
    BOOST_CHECK(center.y() >   0.0);
  }
  if (verbose) { TRACE("A padded text rectangle must at least be as wide as a text rectangle"); }
  {
    BOOST_CHECK(QtRoundedEditRectItem::GetTextRectAtOrigin("X",QFont()).width() > 0.0);
    BOOST_CHECK(QtRoundedEditRectItem::GetPaddedTextRectAtOrigin("X",QFont()).width()
      >= QtRoundedEditRectItem::GetTextRectAtOrigin("X",QFont()).width());
  }
  if (verbose) { TRACE("If item changes its selection, m_signal_selected_changed must be emitted"); }
  {
    Counter c{0}; //For receiving the signal
    item.m_signal_selected_changed.connect(
      boost::bind(&ribi::Counter::Inc,&c) //Do not forget the &
    );
    item.SetSelected(true);
    item.SetSelected(false);
    BOOST_CHECK(c.Get() > 0);
  }
}
