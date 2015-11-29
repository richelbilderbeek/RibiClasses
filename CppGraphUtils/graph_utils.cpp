#include "graph_utils.h"

ribi::graph_utils::graph_utils()
{
  #ifndef NDEBUG
  test();
  #endif
}


#ifndef NDEBUG
void ribi::graph_utils::test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  const graph_utils u;

  {
    //boost::adjacency_list is the 'Swiss army knife' graph
    using my_graph = boost::adjacency_list
    <
      //Store all edges as a std::vector
      boost::vecS,
      //Store all vertices in a std::vector
      boost::vecS,
      //Relations are both ways (in this example)
      //(note: but you can freely change it to boost::directedS)
      boost::undirectedS,
      //All vertices are person names of type std::string
      boost::property<boost::vertex_name_t,std::string>,
      //All edges are weights equal to the encounter frequencies
      boost::property<boost::edge_weight_t,double>,
      //Graph itself has a std::string name
      boost::property<boost::graph_name_t,std::string>
    >;


    using boost::add_edge;
    using boost::add_vertex;
    using my_vertex_descriptor = my_graph::vertex_descriptor;
    my_graph g;

    //All vertex names
    //Note: cannot use spaces
    std::vector<std::string> names;
    names.push_back("Mr_A");
    names.push_back("Mrs_B");
    names.push_back("Dr_C");
    names.push_back("Prof_D");

    const my_vertex_descriptor v0 = add_vertex(names[0],g);
    const my_vertex_descriptor v1 = add_vertex(names[1],g);
    const my_vertex_descriptor v2 = add_vertex(names[2],g);
    const my_vertex_descriptor v3 = add_vertex(names[3],g);

    std::vector<double> frequencies;
    frequencies.push_back(0.9);
    frequencies.push_back(0.5);
    frequencies.push_back(0.6);
    frequencies.push_back(0.1);

    add_edge(v0,v1,frequencies[0],g);
    add_edge(v1,v2,frequencies[1],g);
    add_edge(v2,v3,frequencies[2],g);
    add_edge(v0,v3,frequencies[3],g);
    //What are the vertex indices?
    {
      const std::vector<int> v{0,1,2,3};
      assert(u.get_vertex_indices(g) == v);
    }
    //What are the vertex indices?
    {
      const std::vector<std::pair<int,int>> v{ {0,1}, {1,2}, {2,3}, {3,0} };
      assert(u.get_edges_indices(g) == v);
    }

    u.vertices_to_os(g,std::cout);
    u.edges_to_os(g,std::cout);
  }
}
#endif

