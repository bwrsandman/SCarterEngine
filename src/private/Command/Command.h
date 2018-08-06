//
// Created by sandy on 05/08/18.
//

#ifndef SCARTERENGINE_COMMAND_H
#define SCARTERENGINE_COMMAND_H

namespace sce::command {
class Command {
 public:
  virtual ~Command() = default;
  virtual void Execute() const = 0;
};
}  // namespace sce::command

#endif  // SCARTERENGINE_COMMAND_H
