#ifndef RIBI_TRIANGLEMESHFACE_H
#define RIBI_TRIANGLEMESHFACE_H

#include <iosfwd>
#include <set>
#include <vector>





#include <boost/checked_delete.hpp>
#include <boost/geometry.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signals2.hpp>
#include "trianglemeshfaceorientation.h"
#include "trianglemeshfwd.h"


namespace ribi {
namespace trim {

///Sure, its points can change...
struct Face
{
  //friend class Dialog;
  //friend class Cell;
  //friend class CellsCreator;
  //friend class Helper;
  //friend class Point;
  //friend class Template;
  //friend class TriangleMeshBuilderImpl;
  //friend void CellsCheck(const std::vector<boost::shared_ptr<Cell>>& cells) noexcept;

  typedef boost::geometry::model::point<double,3,boost::geometry::cs::cartesian> Coordinat3D;
  typedef std::set<Coordinat3D,std::function<bool(Coordinat3D,Coordinat3D)>> Coordinat3dSet;

  Face(const Face& ) = delete;
  //Face(      Face&&) = delete;
  Face& operator=(const Face& ) = delete;
  //Face& operator=(      Face&&) = delete;

  Coordinat3D CalcCenter() const noexcept;

  int CalcPriority() const noexcept;

  ///When the Face its points know their Layers, call DoExtractCoordinats
  bool CanExtractCoordinats() const noexcept;

  ///Throws is the face its m_orientation and Points orientationare mismatch
  ///This function is called by the Faces' Points, after they received a Z value
  void CheckOrientation() const;

  ///When the Face its points know their Layers, call this member function
  void DoExtractCoordinats() const;

  Coordinat3dSet GetCoordinats() const noexcept { return m_coordinats; }

  int GetIndex() const noexcept { return m_index; }

  ///nullptr if no neighbour
  boost::shared_ptr<const Cell> GetNeighbour() const noexcept;
  boost::shared_ptr<      Cell> GetNonConstNeighbour()       noexcept;

  FaceOrientation GetOrientation() const noexcept { return m_orientation; }

  ///nullptr if no owner, a Volume::m_cellindex type
  boost::shared_ptr<const Cell> GetConstOwner() const noexcept;

  boost::shared_ptr<const Point> GetPoint(const int index) const noexcept;

  const std::vector<boost::shared_ptr<Point>>& GetPoints() const noexcept { return m_points; }

  void SetBoundaryType(const std::string type) const noexcept { m_type = type; }

  ///Before saving a Face to OpenFOAM, its Points' winding needs to be set to the correct order:
  /// - if the Face is a boundary face, the normal needs to point outwards;
  ///   going away from the mesh; its points needs to be ordered clockwise
  ///   when viewed from its cell's center
  /// - if the Face is an internal face, the normal needs to point inside
  ///   the cell with the heighest index
  void SetCorrectWinding() noexcept;

  ///If a Face has both an owner Cell and a Neighbour Cell, transfer
  ///ownership to the neighbour
  void TransferOwnership() noexcept;

  static const int sm_face_no_index = -2;

  private:
  ~Face() noexcept;
  friend void boost::checked_delete<>(Face* x);
  friend void boost::checked_delete<>(const Face* x);

  ///Cells this Face belongs to
  mutable std::vector<boost::weak_ptr<const Cell>> m_belongs_to;

  ///m_coordinats is used to speed up 'FaceExists', which compares a new Face
  ///with one already present, by comparing their sorted coordinats
  mutable Coordinat3dSet m_coordinats;

  ///Track the creation and deletion of faces
  static std::set<const Face*> sm_faces;

  ///The index of this Face in an TriangleMeshBuilder vector. It is determined at the end
  mutable int m_index;

  const FaceOrientation m_orientation;

  ///m_points must be a std::vector instead of a std::set, as
  ///their orders matter (and changed by ReversePoints)
  std::vector<boost::shared_ptr<Point>> m_points;

  ///The type of boundary this Face belongs to. It is determined at the end
  ///By default it is BoundaryType::internalMesh: a Face that is between two cells
  mutable std::string m_type;

  friend class FaceFactory;
  ///Enforce a Face is only created by a FaceFactory
  explicit Face(
    const std::vector<boost::shared_ptr<Point>>& points,
    const FaceOrientation any_orientation,
    const int index,
    const FaceFactory& lock
  );

  void OnCellDestroyed(const Cell* const cell) noexcept;

  friend class CellFactory;
  void AddBelongsTo(boost::weak_ptr<const Cell> cell);

  ///Determined in the end
  friend class TriangleMeshBuilderImpl;
  std::string GetBoundaryType() const noexcept { return m_type; }
  boost::shared_ptr<Cell> GetNonConstOwner() noexcept;
  void SetIndex(const int index) const noexcept { m_index = index; }

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif

  friend std::ostream& operator<<(std::ostream& os, const Cell& cell) noexcept;
  friend std::ostream& operator<<(std::ostream& os, const Face& f) noexcept;
  friend bool operator==(const Face& lhs, const Face& rhs) noexcept;
  friend bool operator!=(const Face& lhs, const Face& rhs) noexcept;
};

bool operator==(const Face& lhs, const Face& rhs) noexcept;
bool operator!=(const Face& lhs, const Face& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const Face& f) noexcept;

bool operator<(const boost::shared_ptr<const Face>& lhs, const boost::shared_ptr<      Face>& rhs) = delete;
bool operator<(const boost::shared_ptr<const Face>& lhs, const boost::shared_ptr<const Face>& rhs) = delete;
bool operator<(const boost::shared_ptr<      Face>& lhs, const boost::shared_ptr<      Face>& rhs) = delete;
bool operator<(const boost::shared_ptr<      Face>& lhs, const boost::shared_ptr<const Face>& rhs) = delete;


} //~namespace trim
} //~namespace ribi

#endif // RIBI_TRIANGLEMESHFACE_H
