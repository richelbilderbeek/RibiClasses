#ifndef CONCEPTMAPVERTEXISSELECTED_H
#define CONCEPTMAPVERTEXISSELECTED_H

#include <boost/graph/properties.hpp>

namespace boost {
  enum vertex_is_selected_t { vertex_is_selected = 31416 };
  BOOST_INSTALL_PROPERTY(vertex, is_selected);
}


#endif // CONCEPTMAPVERTEXISSELECTED_H
