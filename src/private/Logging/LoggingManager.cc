//
// Created by sandy on 11/05/18.
//

#include "LoggingManager.h"
#include <Debug.h>
#include <array>
#include <iostream>

namespace sce::logging::private_ {
const std::array<const char *, 5> LEVEL_STRINGS{
    "DEBUG", "INFO", "WARN", "ERROR", "FATAL",
};

void LoggingManager::Initialize() {
  DEBUG_RUNTIME_ASSERT_FALSE(this->isInitialized);
  isInitialized = true;
}

void LoggingManager::Terminate() {
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
  std::cout << LEVEL_STRINGS[static_cast<size_t>(level)] << ": " << source_file
            << ":" << source_line << ": " << message << std::endl;
}
}  // namespace sce::logging::private_
