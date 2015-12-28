#ifndef CONCEPTMAPGRAPHFACTORY_H
#define CONCEPTMAPGRAPHFACTORY_H

#include <vector>
#include "conceptmapgraphtypes.h"

namespace ribi {
namespace cmap {

struct Node;

class GraphFactory
{
public:
  using Graphs = std::vector<Graph>;

  GraphFactory();

  Graph FromXml(const std::string& s) const;

  ///Get all the other tests as one vector
  Graphs GetAllTests() const noexcept;
  int GetNumberOfAllTests() const noexcept { return 39; }

  Graph GetEmptyGraph() const noexcept { return Graph(); }

  ///Get the documented heteromorphous test concept maps
  Graphs GetHeteromorphousTestGraphs() const noexcept;
  Graph GetHeteromorphousTestGraph(const int index) const noexcept;

  ///Get the documented complex homomorphous test concept maps
  Graphs GetComplexHomomorphousTestGraphs() const noexcept;
  Graph GetComplexHomomorphousTestGraph(const int index) const noexcept;
  int GetNumberOfComplexHomomorphousTestGraphs() const noexcept { return 12; }

  ///Get the documented simple homomorphous test concept maps
  Graphs GetSimpleHomomorphousTestGraphs() const noexcept;

  Graph GetComplexHomomorphousTestGraph0() const noexcept;
  Graph GetComplexHomomorphousTestGraph1() const noexcept;
  Graph GetComplexHomomorphousTestGraph2() const noexcept;
  Graph GetComplexHomomorphousTestGraph3() const noexcept;
  Graph GetComplexHomomorphousTestGraph4() const noexcept;
  Graph GetComplexHomomorphousTestGraph5() const noexcept;
  Graph GetComplexHomomorphousTestGraph6() const noexcept;
  Graph GetComplexHomomorphousTestGraph7() const noexcept;
  Graph GetComplexHomomorphousTestGraph8() const noexcept;
  Graph GetComplexHomomorphousTestGraph9() const noexcept;
  Graph GetComplexHomomorphousTestGraph10() const noexcept;
  Graph GetComplexHomomorphousTestGraph11() const noexcept;

  Graph GetHeteromorphousTestGraph0() const noexcept;
  Graph GetHeteromorphousTestGraph1() const noexcept;
  Graph GetHeteromorphousTestGraph2() const noexcept;
  Graph GetHeteromorphousTestGraph3() const noexcept;
  Graph GetHeteromorphousTestGraph4() const noexcept;
  Graph GetHeteromorphousTestGraph5() const noexcept;
  Graph GetHeteromorphousTestGraph6() const noexcept;
  Graph GetHeteromorphousTestGraph7() const noexcept;
  Graph GetHeteromorphousTestGraph8() const noexcept;
  Graph GetHeteromorphousTestGraph9() const noexcept;
  Graph GetHeteromorphousTestGraph10() const noexcept;
  Graph GetHeteromorphousTestGraph11() const noexcept;
  Graph GetHeteromorphousTestGraph12() const noexcept;
  Graph GetHeteromorphousTestGraph13() const noexcept;
  Graph GetHeteromorphousTestGraph14() const noexcept;
  Graph GetHeteromorphousTestGraph15() const noexcept;
  Graph GetHeteromorphousTestGraph16() const noexcept;
  Graph GetHeteromorphousTestGraph17() const noexcept;
  Graph GetHeteromorphousTestGraph18() const noexcept;
  Graph GetHeteromorphousTestGraph19() const noexcept;
  Graph GetHeteromorphousTestGraph20() const noexcept;

  private:

  VertexDescriptor AddVertex(const Node& node, Graph& g);
  void AddEdge(
    const Edge& edge,
    const VertexDescriptor& vd_from,
    const VertexDescriptor& vd_to,
    Graph& g
  );

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif

};

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPGRAPHFACTORY_H
