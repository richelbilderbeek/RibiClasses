#ifndef CONCEPTMAPGRAPHTYPE_H
#define CONCEPTMAPGRAPHTYPE_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include "conceptmapvertexcustomtype.h"
#include "conceptmapvertexisselected.h"
#include "conceptmapedgecustomtype.h"
#include "conceptmapedgeisselected.h"
#include "conceptmapnode.h"
#include "conceptmapedge.h"

namespace ribi {
namespace cmap {

using Graph = boost::adjacency_list
<
  boost::vecS,
  boost::vecS,
  boost::directedS,
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
using VertexDescriptor = boost::graph_traits<Graph>::vertex_descriptor;
using EdgeDescriptor = boost::graph_traits<Graph>::edge_descriptor;

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPGRAPHTYPE_H
