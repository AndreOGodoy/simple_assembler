#ifndef TP1_CODES_H
#define TP1_CODES_H

#include <map>
#include <string>

namespace Assembler {
struct CODES {
  static std::map<std::string, std::string> REGISTERS;
  static std::map<std::string, std::string> OPS;
};
} // namespace Assembler

#endif // TP1_CODES_H
