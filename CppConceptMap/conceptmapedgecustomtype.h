#ifndef CONCEPTMAPEDGECUSTOMTYPE_H
#define CONCEPTMAPEDGECUSTOMTYPE_H

#include <boost/graph/properties.hpp>

namespace boost {
  enum edge_custom_type_t { edge_custom_type = 3142 };
  BOOST_INSTALL_PROPERTY(edge,custom_type);
}


#endif // CONCEPTMAPEDGECUSTOMTYPE_H
