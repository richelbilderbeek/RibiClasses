#ifndef CONCEPTMAPISEXAMPLESVERTICESISOMORPHIC_H
#define CONCEPTMAPISEXAMPLESVERTICESISOMORPHIC_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/isomorphism.hpp>
#include "conceptmapexamplesvertexinvariant.h"

template <typename graph>
bool is_examples_vertices_isomorphic(
  const graph &g,
  const graph &h
) noexcept {
  using vd = typename graph::vertex_descriptor;
  auto vertex_index_map = get(boost::vertex_index, g);
  std::vector<vd> iso(boost::num_vertices(g));

  typename examples_vertex_invariant<graph>::custom_vertex_to_int_map shared_custom;
  examples_vertex_invariant<graph> inv1{g, shared_custom};
  examples_vertex_invariant<graph> inv2{h, shared_custom};
  inv1.collect_custom();
  inv2.collect_custom();

  return boost::isomorphism(g, h,
    boost::isomorphism_map(
      make_iterator_property_map(
        iso.begin(),
        vertex_index_map
      )
    )
    .vertex_invariant1(inv1)
    .vertex_invariant2(inv2)
  );
}

#endif // CONCEPTMAPISEXAMPLESVERTICESISOMORPHIC_H
