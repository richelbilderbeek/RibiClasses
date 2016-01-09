#ifndef CONCEPTMAPEXAMPLEVERTEXINVARIANT_H
#define CONCEPTMAPEXAMPLEVERTEXINVARIANT_H

#include <iostream>
#include <map>
#include <boost/graph/adjacency_list.hpp>
#include <boost/range.hpp>
#include "conceptmapexample.h"
#include "install_vertex_custom_type.h"

template <class graph>
struct example_vertex_invariant {
  using custom_vertex_to_int_map = std::map<ribi::cmap::Example, size_t>;
  using result_type = size_t;
  using argument_type = typename graph::vertex_descriptor;

  const graph& m_graph;
  custom_vertex_to_int_map& m_mappings;

  size_t operator()(argument_type u) const {
      return m_mappings.at(boost::get(boost::vertex_custom_type, m_graph, u));
  }
  size_t max() const noexcept { return m_mappings.size(); }

  void collect_custom() noexcept {
    for (auto vd : boost::make_iterator_range(boost::vertices(m_graph))) {
      size_t next_id = m_mappings.size();
      auto ins = m_mappings.insert(
        { boost::get(boost::vertex_custom_type, m_graph, vd), next_id}
      );
      if (ins.second) {
        //std::cout << "Mapped '" << ins.first->first << "' to " << ins.first->second << "\n";
      }
    }
  }
};

#endif // CONCEPTMAPEXAMPLEVERTEXINVARIANT_H
