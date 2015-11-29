#ifndef RIBI_GRAPH_UTILS_H
#define RIBI_GRAPH_UTILS_H

#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <vector>

namespace ribi {

struct graph_utils
{
  graph_utils();

  ///Returns the indices of all vertices the edges point to
  template <class T>
  std::vector<std::pair<int,int>> get_edges_indices(const T& g) const noexcept {
    using my_edge_iterator = typename boost::graph_traits<T>::edge_iterator;
    my_edge_iterator i, j;
    boost::tie(i, j) = boost::edges(g);
    std::vector<std::pair<int,int>> v;
    for ( ; i != j; ++i) {
      v.push_back(
        boost::get(boost::edge_index,*i)
      );
    }
    return v;
  }


  ///Returns the indices of all vertices
  template <class T>
  std::vector<int> get_vertex_indices(const T& g) const noexcept {
    using my_vertex_iterator = typename boost::graph_traits<T>::vertex_iterator;
    my_vertex_iterator i, j;
    boost::tie(i, j) = boost::vertices(g);
    std::vector<int> v;
    for ( ; i != j; ++i) {
      v.push_back(*i);
    }
    return v;
  }



  template <class T>
  void edges_to_os(const T& g, std::ostream& os) const noexcept {
    using my_edge_iterator = typename boost::graph_traits<T>::edge_iterator;
    my_edge_iterator i, j;
    boost::tie(i, j) = boost::edges(g);
    for ( ; i != j; ++i) {
      os << (*i) << '\n';
    }
  }

  template <class T>
  void vertices_to_os(const T& g, std::ostream& os) const noexcept {
    using my_vertex_iterator = typename boost::graph_traits<T>::vertex_iterator;
    my_vertex_iterator i, j;
    boost::tie(i, j) = boost::vertices(g);
    for ( ; i != j; ++i) {
      os << (*i) << '\n';
    }
  }

  #ifndef NDEBUG
  static void test() noexcept;
  #endif
};

} //~namespace ribi

#endif // RIBI_GRAPH_UTILS_H
