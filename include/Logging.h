//
// Created by sandy on 11/05/18.
//

#ifndef SCARTERENGINE_LOGGING_H
#define SCARTERENGINE_LOGGING_H

#include <string>

#define LOG(level, message)                                     \
  do {                                                          \
    sce::logging::Log(FILE_BASENAME, __LINE__, level, message); \
  } while (false)

namespace sce::logging {

enum class Level {
  Debug,
  Info,
  Warning,
  Error,
  Fatal,
};

void Initialize();
void Terminate();

void SetLogLevel(Level level);
void Log(const std::string & source_file, uint32_t source_line, Level level,
         const std::string & message);
}  // namespace sce::logging

#endif  // SCARTERENGINE_LOGGING_H
