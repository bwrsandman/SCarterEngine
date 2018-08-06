//
// Created by sandy on 05/08/18.
//

#include "ReusableCommandQueue.h"

namespace sce::command {

ReusableCommandQueue::~ReusableCommandQueue() {
  for (auto & cmd : queue) {
    delete cmd;
  }
  queue.clear();
}

void ReusableCommandQueue::Emplace(const Command *&& pCmd) {
  queue.push_back(pCmd);
}

void ReusableCommandQueue::Execute() const {
  for (auto & cmd : queue) {
    cmd->Execute();
  }
}

void ReusableCommandQueue::Reset() {
  queue.clear();
}
}  // namespace sce::command
