//
// Created by sandy on 11/05/18.
//

#include "LoggingManager.h"
#include <Debug.h>
#include <algorithm>
#include <array>
#include <iostream>

namespace sce::logging::private_ {

void LoggingManager::Initialize(bool reload) {
  DEBUG_RUNTIME_ASSERT_FALSE(this->isInitialized);
  isInitialized = true;
}

void LoggingManager::Terminate(bool reload) {
  DEBUG_RUNTIME_ASSERT_TRUE(this->isInitialized);
  isInitialized = false;
}

void LoggingManager::SetLogLevel(Level level) {
  DEBUG_RUNTIME_ASSERT_TRUE(this->isInitialized);
  logLevel_ = level;
}

void LoggingManager::Log(const std::string & source_file, uint32_t source_line,
                         Level level, const std::string & message) {
  DEBUG_RUNTIME_ASSERT_TRUE(this->isInitialized);
  if (level < logLevel_)
    return;
  auto level_str = std::string(level._to_string());
  std::transform(level_str.begin(), level_str.end(), level_str.begin(),
                 ::toupper);
  std::cout << level_str << ": ";
  // Source line cannot be 0
  // Special case where we don't want line number and file (such as script
  // logging)
  if (source_line > 0) {
    std::cout << source_file << ":" << source_line << ": ";
  }
  std::cout << message << std::endl;
}
}  // namespace sce::logging::private_
