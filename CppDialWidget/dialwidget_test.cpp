



#include "dialwidget.h"

#include <cassert>
#include <cmath>
#include <iostream>

#include <boost/numeric/conversion/cast.hpp>
#include <boost/math/constants/constants.hpp>

#include "dial.h"
#include "drawcanvas.h"
#include "geometry.h"
#include "textcanvas.h"



void ribi::DialWidgetTest() noexcept
{
  {
    DialWidget a;
    DialWidget b;
    assert(a == b);
  }
  {
    DialWidget w;
    for (double position = 0.0; position < 1.0; position += 0.0625)
    {
      w.GetDial().SetPosition(position);
      std::stringstream s;
      s << '\n' << w.ToTextCanvas(4);
      assert(!s.str().empty());

      s << '\n' << w.ToDrawCanvas(4);
      assert(!s.str().empty());
      //TRACE(s.str());
    }
  }
}

