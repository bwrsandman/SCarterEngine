//
// Created by sandy on 11/05/18.
//

#ifndef SCARTERENGINE_DEBUG_H
#define SCARTERENGINE_DEBUG_H

#include <string>

#define DEBUG_RUNTIME_ASSERT_TRUE(boolean)                             \
  do {                                                                 \
    sce::debug::runtime::AssertTrue(FILE_BASENAME, __LINE__, boolean); \
  } while (false)

#define DEBUG_RUNTIME_ASSERT_FALSE(boolean)                             \
  do {                                                                  \
    sce::debug::runtime::AssertFalse(FILE_BASENAME, __LINE__, boolean); \
  } while (false)

#define DEBUG_RUNTIME_ASSERT_NOT_NULL(ptr)                            \
  do {                                                                \
    sce::debug::runtime::AssertNotNull(FILE_BASENAME, __LINE__, ptr); \
  } while (false)

namespace sce::debug {
namespace compiletime {}
namespace runtime {
void AssertTrue(std::string source_file, uint32_t source_line, bool boolean);
void AssertFalse(std::string source_file, uint32_t source_line, bool boolean);
void AssertNotNull(std::string source_file, uint32_t source_line, void * ptr);
}  // namespace runtime
}  // namespace sce::debug

#endif  // SCARTERENGINE_DEBUG_H
