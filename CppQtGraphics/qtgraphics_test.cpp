//---------------------------------------------------------------------------
/*
QtGraphics, Qt graphics code snippets
Copyright (C) 2015-2016 Richel Bilderbeek

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
//From http://www.richelbilderbeek.nl/CppQtArrowItem.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include "qtgraphics.h"
#include <boost/test/unit_test.hpp>

#include "stopwatch.h"
#include "testtimer.h"
#include "trace.h"
#pragma GCC diagnostic pop

BOOST_AUTO_TEST_CASE(ribi_qtgraphics_test)
{
  using namespace ribi;
  const bool verbose{false};
  if (verbose) { TRACE("Default-construction of QtGraphics"); }
  {
    const QtGraphics q;
  }
  //CreateImage
  {
    const QImage a = QtGraphics().CreateImage(256,256,64);
    BOOST_CHECK(!a.isNull());
  }
  {
    QImage target = QtGraphics().CreateImage(256,256,64);
    BOOST_CHECK(!target.isNull());
    const QImage source = QtGraphics().CreateImage(196,156,196);
    BOOST_CHECK(!source.isNull());
    QtGraphics().DrawImage(target,source,32,64);
  }
  {
    QImage target_slowest = QtGraphics().CreateImage(256,256,64);
    QImage target_slow    = QtGraphics().CreateImage(256,256,64);
    QImage target_fast    = QtGraphics().CreateImage(256,256,64);
    const QImage source   = QtGraphics().CreateImage(196,156,196);
    QtGraphics().DrawImage(       target_fast   ,source,32,64);
    QtGraphics().DrawImageSlow   (target_slow   ,source,32,64);
    QtGraphics().DrawImageSlowest(target_slowest,source,32,64);
    BOOST_CHECK(target_fast == target_slow);
    BOOST_CHECK(target_fast == target_slowest);
  }
  const bool do_timing{false};
  if (do_timing)
  {
    //TRACE 't_fast' line 188 in file '../../Classes/CppQtGraphics/qtgraphics.cpp': '0,032000000000000001'
    //TRACE 't_slow' line 189 in file '../../Classes/CppQtGraphics/qtgraphics.cpp': '0,51700000000000002'
    //TRACE 't_slowest' line 190 in file '../../Classes/CppQtGraphics/qtgraphics.cpp': '1,169'
    const int sz{5000};
    QImage target_fast    = QtGraphics().CreateImage(sz,sz);
    QImage target_slow    = QtGraphics().CreateImage(sz,sz);
    QImage target_slowest = QtGraphics().CreateImage(sz,sz);
    const QImage source   = QtGraphics().CreateImage(sz,sz);
    Stopwatch s_fast;
    QtGraphics().DrawImage(target_fast,source,0,0);
    const double t_fast{s_fast.GetElapsedSecs()};
    Stopwatch s_slow;
    QtGraphics().DrawImageSlow(target_slow,source,0,0);
    const double t_slow{s_slow.GetElapsedSecs()};
    Stopwatch s_slowest;
    QtGraphics().DrawImageSlowest(target_slowest,source,0,0);
    const double t_slowest{s_slowest.GetElapsedSecs()};
    TRACE(t_fast);
    TRACE(t_slow);
    TRACE(t_slowest);
    BOOST_CHECK(t_fast < t_slow);
    BOOST_CHECK(t_slow < t_slowest);
    BOOST_CHECK(t_fast * 10.0 < t_slow);
  }
}
