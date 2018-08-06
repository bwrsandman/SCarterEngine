//
// Created by sandy on 05/08/18.
//

#ifndef SCARTERENGINE_REUSABLECOMMANDBUFFER_H
#define SCARTERENGINE_REUSABLECOMMANDBUFFER_H

#include <memory>
#include <vector>
#include "Command.h"

namespace sce::command {
class ReusableCommandQueue {
  std::vector<const Command *> queue;

 public:
  virtual ~ReusableCommandQueue();
  void Emplace(const Command *&& pCmd);
  void Execute() const;
  void Reset();
};
}  // namespace sce::command

#endif  // SCARTERENGINE_REUSABLECOMMANDBUFFER_H
