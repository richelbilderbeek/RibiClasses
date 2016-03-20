//---------------------------------------------------------------------------
/*
Trace, some tracing macro's
Copyright (C) 2010-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppTrace.htm
//---------------------------------------------------------------------------
#include <boost/test/unit_test.hpp>
#include "trace.h"

BOOST_AUTO_TEST_CASE(ribi_trace_test)
{
  BOOST_CHECK(!Trace::GetVersion().empty());
}
