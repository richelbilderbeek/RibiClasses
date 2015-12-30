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

  GraphFactory() noexcept;

  Graph FromXml(const std::string& s) const;

  ///Get all the other tests as one vector
  Graphs GetAllTests() const noexcept;
  Graph GetTest(const int index) const noexcept;
  int GetNumberOfAllTests() const noexcept { return 7; }

  Graph GetEmptyGraph() const noexcept { return Get0(); }

  ///Get the documented heteromorphous test concept maps
  Graph Get0() const noexcept; //Empty graph
  Graph Get1() const noexcept;
  Graph Get2() const noexcept;
  Graph Get3() const noexcept;
  Graph Get4() const noexcept;
  Graph Get5() const noexcept;
  Graph Get6() const noexcept;

  private:

  VertexDescriptor AddVertex(const Node& node, Graph& g) const noexcept;

  void AddEdge(
    const Edge& edge,
    const VertexDescriptor& vd_from,
    const VertexDescriptor& vd_to,
    Graph& g
  ) const noexcept;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif

};

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPGRAPHFACTORY_H
