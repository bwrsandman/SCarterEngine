//
// Created by sandy on 10/06/18.
//

#include "RenderingManagerGles.h"

#include <Debug.h>
#include <Engine.h>
#include <GLES3/gl32.h>
#include <Logging.h>

namespace sce::rendering::private_ {
RenderingManagerGles::RenderingManagerGles()
    : RenderingManager(SDL_WINDOW_OPENGL) {}

#if !defined(NDEBUG) && !defined(EMSCRIPTEN)
void DebugCallback(GLenum source, GLenum type, GLuint msgCode, GLenum severity,
                   GLsizei length, const char * pMsg,
                   const void * /*pUserData*/) {
  if (!pMsg || pMsg[0] == 0)
    return;
  logging::Level level = logging::Level::Info;
  if (type == GL_DEBUG_TYPE_ERROR) {
    level = logging::Level::Error;
  } else if (severity == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR) {
    level = logging::Level::Warning;
  } else if (severity == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR) {
    level = logging::Level::Warning;
  } else if (severity == GL_DEBUG_TYPE_PORTABILITY) {
    level = logging::Level::Warning;
  } else if (severity == GL_DEBUG_TYPE_PERFORMANCE) {
    level = logging::Level::Warning;
  } else if (severity == GL_DEBUG_TYPE_OTHER) {
    level = logging::Level::Debug;
  } else if (severity == GL_DEBUG_TYPE_MARKER) {
    level = logging::Level::Debug;
  } else if (severity == GL_DEBUG_TYPE_PUSH_GROUP) {
    level = logging::Level::Debug;
  } else if (severity == GL_DEBUG_TYPE_POP_GROUP) {
    level = logging::Level::Debug;
  } else if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
    level = logging::Level::Debug;
  }
  std::string severityString = "Unknown";
  if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
    severityString = "Notification";
  } else if (severity == GL_DEBUG_SEVERITY_LOW) {
    severityString = "Low";
  } else if (severity == GL_DEBUG_SEVERITY_MEDIUM) {
    severityString = "Medium";
  } else if (severity == GL_DEBUG_SEVERITY_HIGH) {
    severityString = "High";
  }
  std::string sourceString = "Unknown";
  if (source == GL_DEBUG_SOURCE_API) {
    sourceString = "API";
  } else if (source == GL_DEBUG_SOURCE_WINDOW_SYSTEM) {
    sourceString = "Windowing system";
  } else if (source == GL_DEBUG_SOURCE_SHADER_COMPILER) {
    sourceString = "Shader Compiler";
  } else if (source == GL_DEBUG_SOURCE_THIRD_PARTY) {
    sourceString = "Third Party";
  } else if (source == GL_DEBUG_SOURCE_APPLICATION) {
    sourceString = "Application";
  } else if (source == GL_DEBUG_SOURCE_OTHER) {
    sourceString = "Other";
  }
  auto message = length < 0
                     ? std::string(pMsg)
                     : std::string(pMsg, static_cast<unsigned long>(length));
  if (message.back() == '\n') {
    message = message.substr(0, message.size() - 1);
  }
  LOG(level, std::string("[") + sourceString + "] (Code " +
                 std::to_string(msgCode) + ", Severity " + severityString +
                 "): " + message);
}
#endif

void RenderingManagerGles::InitializeInternal() {
#if !defined(NDEBUG) && !defined(EMSCRIPTEN)
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif

  context_ = SDL_GL_CreateContext(window_);
  if (context_ == nullptr) {
    LOG(logging::Level::Fatal, SDL_GetError());
    engine::ScheduleQuit();
  }

#if !defined(NDEBUG) && !defined(EMSCRIPTEN)
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(DebugCallback, nullptr);
#endif

  SDL_GL_SetSwapInterval(0);
}

void RenderingManagerGles::GenerateCommands() {
  commandQueue_.Reset();
  commandQueue_.glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1,
                                 "Clear Back Buffer");
  commandQueue_.glClearColor(clearColor_.r, clearColor_.g, clearColor_.b,
                             clearColor_.a);
  commandQueue_.glClear(static_cast<uint32_t>(GL_COLOR_BUFFER_BIT));
  commandQueue_.glPopDebugGroup();
}

void RenderingManagerGles::TerminateInternal() {
  DEBUG_RUNTIME_ASSERT_TRUE(isInitialized);
  SDL_GL_DeleteContext(context_);
}

void RenderingManagerGles::Submit() {
  commandQueue_.Execute();
}

void RenderingManagerGles::Present() const {
  SDL_GL_SwapWindow(window_);
}
}  // namespace sce::rendering::private_
