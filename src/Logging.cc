//
// Created by sandy on 11/05/18.
//

#include <Logging.h>
#include "private/Logging/LoggingManager.h"

namespace sce::logging {
private_::LoggingManager gLoggingManager;

void Initialize() {
  gLoggingManager.Initialize(false);
}

void Terminate() {
  gLoggingManager.Terminate(false);
}

void SetLogLevel(Level level) {
  gLoggingManager.SetLogLevel(level);
}

void Log(const std::string & source_file, uint32_t source_line, Level level,
         const std::string & message) {
  gLoggingManager.Log(source_file, source_line, level, message);
}
}  // namespace sce::logging
