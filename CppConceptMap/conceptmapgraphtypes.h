#ifndef CONCEPTMAPGRAPHTYPE_H
#define CONCEPTMAPGRAPHTYPE_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include "conceptmapnode.h"
#include "conceptmapedge.h"
#include "install_edge_custom_type.h"
#include "install_edge_is_selected.h"
#include "install_vertex_custom_type.h"
#include "install_vertex_is_selected.h"

namespace ribi {
namespace cmap {

using ConceptMap = boost::adjacency_list
<
  boost::vecS,
  boost::vecS,
  boost::undirectedS,
  boost::property<
    boost::vertex_custom_type_t, Node,
    boost::property<
      boost::vertex_is_selected_t, bool
    >
  >,
  boost::property<
    boost::edge_custom_type_t, Edge,
    boost::property<
      boost::edge_is_selected_t, bool
    >
  >
>;
using VertexDescriptor = boost::graph_traits<ConceptMap>::vertex_descriptor;
static_assert(sizeof(VertexDescriptor) == sizeof(VertexDescriptor&),"On university computer");
using EdgeDescriptor = boost::graph_traits<ConceptMap>::edge_descriptor;

} //~namespace cmap
} //~namespace ribi



#endif // CONCEPTMAPGRAPHTYPE_H
