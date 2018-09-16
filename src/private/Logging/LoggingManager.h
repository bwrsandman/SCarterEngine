//
// Created by sandy on 11/05/18.
//

#ifndef SCARTERENGINE_LOGGING_LOGGINGMANAGER_H
#define SCARTERENGINE_LOGGING_LOGGINGMANAGER_H

#include <Logging.h>
#include <string>
#include "../Manager.h"

namespace sce::logging::private_ {
class LoggingManager : public sce::private_::Manager {
  Level logLevel_ = Level::Info;

 public:
  void Initialize(bool reload) override;
  void Terminate(bool reload) override;

  void SetLogLevel(Level level);
  void Log(const std::string & source_file, uint32_t source_line, Level level,
           const std::string & message);
};
}  // namespace sce::logging::private_

#endif  // SCARTERENGINE_LOGGING_LOGGINGMANAGER_H
