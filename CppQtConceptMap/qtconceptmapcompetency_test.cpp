#include <boost/test/unit_test.hpp>
#include "qtconceptmapcompetency.h"
#include "conceptmapcompetencies.h"
#include <QDataStream>

BOOST_AUTO_TEST_CASE(ribi_cmap_qtcompetency_competency_to_color)
{
  using namespace ribi::cmap;
  //Conversion between QColor and Competency
  {
    const std::vector<Competency> v = Competencies().GetAllCompetencies();
    std::for_each(v.begin(),v.end(),
      [](const Competency& competency)
      {
        QColor color = QtCompetency().CompetencyToColor(competency);
        BOOST_CHECK_EQUAL(QtCompetency().ColorToCompetency(color), competency);
      }
    );
  }
}

BOOST_AUTO_TEST_CASE(ribi_cmap_qtcompetency_competency_to_icon_assume_this_works)
{
  using namespace ribi::cmap;
  const QIcon image_misc = QtCompetency().CompetencyToIcon(Competency::profession);
  const QIcon image_uninitialized = QtCompetency().CompetencyToIcon(Competency::uninitialized);
  BOOST_CHECK(image_misc.pixmap(14,14).toImage() == image_misc.pixmap(14,14).toImage());
  BOOST_CHECK(image_uninitialized.pixmap(14,14).toImage() == image_uninitialized.pixmap(14,14).toImage());
  BOOST_CHECK(image_misc.pixmap(14,14).toImage() != image_uninitialized.pixmap(14,14).toImage());
}

BOOST_AUTO_TEST_CASE(ribi_cmap_qtcompetency_competency_to_icon)
{
  using namespace ribi::cmap;
  const std::vector<Competency> v = Competencies().GetAllCompetencies();
  std::for_each(v.begin(),v.end(),
    [](const Competency& competency)
    {
      QIcon icon = QtCompetency().CompetencyToIcon(competency);
      BOOST_CHECK(!icon.isNull());
      BOOST_CHECK_EQUAL(QtCompetency().IconToCompetency(icon), competency);
    }
  );
}
