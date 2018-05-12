//
// Created by sandy on 11/05/18.
//

#include <Debug.h>

#include <iomanip>
#include <iostream>

namespace sce::debug {
namespace compiletime {}
namespace runtime {
void AssertTrue(std::string source_file, uint32_t source_line, bool boolean) {
#ifndef NDEBUG
  if (!boolean) {
    std::cerr << source_file << ":" << source_line << ": assert true failed"
              << std::endl;
    std::terminate();
  }
#endif
}

void AssertFalse(std::string source_file, uint32_t source_line, bool boolean) {
#ifndef NDEBUG
  if (boolean) {
    std::cerr << source_file << ":" << source_line << ": assert false failed"
              << std::endl;
    std::terminate();
  }
#endif
}

void AssertNotNull(std::string source_file, uint32_t source_line, void * ptr) {
#ifndef NDEBUG
  if (ptr == nullptr) {
    std::cerr << source_file << ":" << source_line << ": pointer is null"
              << std::endl;
    std::terminate();
  }
#endif
}
}  // namespace runtime
}  // namespace sce::debug
