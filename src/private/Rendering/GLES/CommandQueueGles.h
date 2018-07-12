//
// Created by sandy on 05/08/18.
//

#ifndef SCARTERENGINE_COMMANDQUEUEGLES_H
#define SCARTERENGINE_COMMANDQUEUEGLES_H

#include <functional>
#ifndef NDEBUG
#include <string>
#include <utility>
#endif  // NDEBUG

#include <GLES3/gl32.h>

#include <Logging.h>

#include "../../Command/ReusableCommandQueue.h"

#ifdef NDEBUG
#define __GL_COMMAND_NAME(name)
#else
#define __GL_COMMAND_NAME(name) name,
#endif

#define GL_TO_COMMAND(func, ...)                                         \
  template <typename... As>                                              \
  void func(As... args) {                                                \
    Emplace(__GL_COMMAND_NAME(#func) std::function<void(As...)>(::func), \
            args...);                                                    \
  }
#if !defined(NDEBUG) && !defined(EMSCRIPTEN)
#define GL_TO_COMMAND_DEBUG(func, ...) GL_TO_COMMAND(func, ##__VA_ARGS__)
#else
#define GL_TO_COMMAND_DEBUG(func, ...) \
  template <typename... As>            \
  inline void func(As... args) {}
#endif

template <class T>
class CommandGles : public sce::command::Command {
#ifndef NDEBUG
  std::string commandName_;
#endif  // NDEBUG

  const std::function<T> func_;

 public:
  CommandGles(
#ifndef NDEBUG
      std::string commandName,
#endif  // NDEBUG
      std::function<T> func)
      :
#ifndef NDEBUG
        commandName_(commandName),
#endif  // NDEBUG
        func_(std::move(func)) {
  }
  void Execute() const override {
#ifndef NDEBUG
    LOG(sce::logging::Level::Debug, commandName_);
#endif  // NDEBUG
    func_();
  }
};

namespace sce::rendering::private_ {
class CommandQueueGles : public command::ReusableCommandQueue {
  template <typename... As>
  void Emplace(
#ifndef NDEBUG
      std::string commandName,
#endif  // NDEBUG
      std::function<void(As...)> func, As... args) {
    auto f = std::function<void()>([func, args...]() { func(args...); });
    command::ReusableCommandQueue::Emplace(new CommandGles(
#ifndef NDEBUG
        commandName,
#endif  // NDEBUG
        f));
  }

 public:
  GL_TO_COMMAND(glClearColor)
  GL_TO_COMMAND(glClear)
  GL_TO_COMMAND_DEBUG(glPushDebugGroup)
  GL_TO_COMMAND_DEBUG(glPopDebugGroup)
};
}  // namespace sce::rendering::private_

#endif  // SCARTERENGINE_COMMANDQUEUEGLES_H
