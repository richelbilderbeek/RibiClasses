#include "qtconceptmaprateexamplesdialognewname.h"
#include <boost/test/unit_test.hpp>

#include <QDesktopWidget>
#include <QKeyEvent>
#include <QListWidgetItem>

#include "conceptmapconceptfactory.h"
#include "conceptmapconcept.h"
#include "conceptmapexamplefactory.h"
#include "conceptmapexamplesfactory.h"
#include "conceptmaphelper.h"
#include "qtconceptmapcompetency.h"
#include "testtimer.h"
#include "trace.h"
#include "ui_qtconceptmaprateexamplesdialognewname.h"

BOOST_AUTO_TEST_CASE(ribi_cmap_qtrateexamplesdialog_test)
{
  {
    const int sz = ribi::cmap::ConceptFactory().GetTests().size();
    for (int i=0; i!=sz; ++i)
    {
      BOOST_CHECK(i < static_cast<int>(ConceptFactory().GetTests().size()));
      const auto a = QtRateExamplesDialogNewName(ConceptFactory().GetTests()[i]).GetRatedExamples();
      for (int j=0; j!=sz; ++j)
      {
        BOOST_CHECK(j < static_cast<int>(ConceptFactory().GetTests().size()));
        const auto b = QtRateExamplesDialogNewName(ConceptFactory().GetTests()[j]).GetRatedExamples();
        BOOST_CHECK(a != b);
        if (i == j)
        {
          BOOST_CHECK(a == b);
        }
        else
        {
          BOOST_CHECK(a != b);
        }
      }
    }
  }
}
