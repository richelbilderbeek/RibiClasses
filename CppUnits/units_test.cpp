#ifndef NDEBUG
#include "units.h"

#include <iostream>
#include <sstream>

#include <boost/units/io.hpp>

#include "concentration.h"
#include "hydrogenmoleculeamount.h"
#include "rate.h"
#include "massflow.h"
#include "moleculeamount.h"
#include "speciesdensity.h"
#include "speciesgrowth.h"
#include "sulfidemoleculeamount.h"
#include "ribi_time.h"
#include "volumetricflow.h"

void ribi::units::Test() noexcept
{
  {
    TestConcentration();
    TestHydrogenMoleculeAmount();
    TestMassFlow();
    TestRate();
    TestMoleculeAmount();
    TestSpeciesDensity();
    TestSpeciesGrowth();
    TestSulfideMoleculeAmount();
    TestTime();
    TestVolumetricFlow();
  }
  #ifdef MUST_NOT_COMPILE_CREATE_HYDROGEN_FROM_SULFIDE
  //Cannot create hydrogen from sulfide
  {
    using HydrogenMoleculeAmount = boost::units::quantity<boost::units::si::hydrogen_molecule_amount>;
    const HydrogenMoleculeAmount h{1.0 * boost::units::si::sulfide_molecules_mol}; //Must fail
  }
  #endif
  #ifdef MUST_NOT_COMPILE_CREATE_SULFIDE_FROM_HYDROGEN
  //Cannot create sulfide from hydrogen
  {
    using SulfideMoleculeAmount = boost::units::quantity<boost::units::si::sulfide_molecule_amount>;
    const SulfideMoleculeAmount s{1.0 * boost::units::si::hydrogen_molecules_mol};
  }
  #endif
  #ifdef MUST_NOT_COMPILE_ADD_HYDROGEN_AND_SULFIDE
  //Cannot add hydrogen and sulfide molecule amounts
  {
    using HydrogenMoleculeAmount = boost::units::quantity<boost::units::si::hydrogen_molecule_amount>;
    using SulfideMoleculeAmount = boost::units::quantity<boost::units::si::sulfide_molecule_amount>;
    const HydrogenMoleculeAmount h{1.0 * boost::units::si::hydrogen_molecules_mol};
    const SulfideMoleculeAmount s{1.0 * boost::units::si::sulfide_molecules_mol};
    h + s; //Must fail
  }
  #endif
}
#endif

