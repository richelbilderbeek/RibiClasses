#include <boost/test/unit_test.hpp>
#include "qtconceptmapcompetency.h"
#include "conceptmapcompetencies.h"

BOOST_AUTO_TEST_CASE(ribi_cmap_qtcompetency_test)
{
  using namespace ribi::cmap;
  //Conversion between QColor and Competency
  {
    const std::vector<Competency> v = Competencies().GetAllCompetencies();
    std::for_each(v.begin(),v.end(),
      [](const Competency& competency)
      {
        QColor color = QtCompetency().CompetencyToColor(competency);
        BOOST_CHECK(QtCompetency().ColorToCompetency(color) == competency);
      }
    );
  }
  //Conversion between QIcon and Competency
  {
    const std::vector<Competency> v = Competencies().GetAllCompetencies();
    std::for_each(v.begin(),v.end(),
      [](const Competency& competency)
      {
        QIcon icon = QtCompetency().CompetencyToIcon(competency);
        BOOST_CHECK(QtCompetency().IconToCompetency(icon) == competency);
      }
    );
  }
}
