#ifndef RIBI_TRIANGLEMESHCELLSCREATORFACTORY_H
#define RIBI_TRIANGLEMESHCELLSCREATORFACTORY_H

#include <iosfwd>
#include <vector>





#include <boost/checked_delete.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/quantity.hpp>
#include "trianglemeshfwd.h"
#include "trianglemeshcreateverticalfacesstrategy.h"


namespace ribi {
namespace trim {

///Creation of Cell
struct CellsCreatorFactory
{
  //friend class CellFactory;
  //friend class CellsCreator;
  //friend class Dialog;

  CellsCreatorFactory() noexcept;

  boost::shared_ptr<CellsCreator> Create(
    const boost::shared_ptr<const Template> t,
    const int n_cell_layers,
    const boost::units::quantity<boost::units::si::length> layer_height,
    const CreateVerticalFacesStrategy strategy,
    const bool verbose
  ) const noexcept;


  ///Create a cell in the shape of a prism
  /*
        +
       /|\
      +---+
      | | |
      | + |
      |/ \|
      +---+
  */
  boost::shared_ptr<CellsCreator> CreateTestPrism(
    const CreateVerticalFacesStrategy strategy
  ) const noexcept;


  ///Create two prisms-shaped cell to form a cube
  /*
        +---+
       /|\ /|
      +---+ |
      | | | |
      | +-|-+
      |/ \|/
      +---+
  */
  boost::shared_ptr<CellsCreator> CreateTestCube(
    const CreateVerticalFacesStrategy strategy
  ) const noexcept;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

} //~namespace trim
} //~namespace ribi


#endif // RIBI_TRIANGLEMESHCELLSCREATORFACTORY_H
