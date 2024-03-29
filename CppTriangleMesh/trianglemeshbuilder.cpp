


#include "trianglemeshbuilder.h"

#include <cassert>
#include <vector>
#include <boost/make_shared.hpp>



#include "trianglemeshbuilderimpl.h"



ribi::trim::TriangleMeshBuilder::TriangleMeshBuilder(
  const std::vector<boost::shared_ptr<Cell>>& cells,
  const std::string& mesh_filename,
  const std::function<ribi::foam::PatchFieldType(const std::string&)> boundary_to_patch_field_type_function,
  const CreateVerticalFacesStrategy strategy,
  const bool verbose
) : m_impl{new TriangleMeshBuilderImpl(
        cells,
        mesh_filename,
        boundary_to_patch_field_type_function,
        strategy,
        verbose
      )
   }
{
  #ifndef NDEBUG
  Test();
  #endif
}

ribi::trim::TriangleMeshBuilder::~TriangleMeshBuilder() noexcept
{
  //To prevent incomplete TriangleMeshBuilderImpl errors
}

int ribi::trim::TriangleMeshBuilder::CountCells() const noexcept
{
  assert(m_impl);
  return m_impl->CountCells();
}

int ribi::trim::TriangleMeshBuilder::CountFaces() const noexcept
{
  assert(m_impl);
  return m_impl->CountFaces();
}

#ifndef NDEBUG
void ribi::trim::TriangleMeshBuilder::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  
}
#endif


