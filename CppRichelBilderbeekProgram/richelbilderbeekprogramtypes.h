#ifndef RIBI_RICHELBILDERBEEKPROGRAMTYPES_H
#define RIBI_RICHELBILDERBEEKPROGRAMTYPES_H

#pragma GCC diagnostic push



#include <boost/bimap.hpp>
#include "richelbilderbeekprogramtype.h"
#pragma GCC diagnostic pop

namespace ribi {

struct ProgramTypes
{
  ProgramTypes();

  ///Convert a type to its enum name
  ///For example, ProgramType 'x' becomes std::string "x"
  std::string ProgramTypeToEnumName(const ProgramType t) const noexcept;

  ///Convert a type to its screen name
  ///For example, ProgramType 'abcDef' might become std::string "Abc Def"
  std::string ProgramTypeToScreenName(const ProgramType t) const noexcept;

  ProgramType EnumNameToProgramType(const std::string& s) const;

  std::vector<std::string> GetAllEnumNames() const noexcept;
  std::vector<std::string> GetAllScreenNames() const noexcept;
  std::vector<ProgramType> GetAll() const noexcept;

  private:
  ///Created by lazy initialization
  static boost::bimap<ProgramType,std::string> m_map_to_enumname;

  ///Created by lazy initialization
  static boost::bimap<ProgramType,std::string> m_map_to_screenname;

  static boost::bimap<ProgramType,std::string> CreateEnumNameMap();
  static boost::bimap<ProgramType,std::string> CreateScreenNameMap();

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

} //~namespace ribi

#endif // RIBI_RICHELBILDERBEEKPROGRAMTYPES_H
