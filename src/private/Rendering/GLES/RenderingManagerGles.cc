//
// Created by sandy on 10/06/18.
//

#include "RenderingManagerGles.h"

#include <Debug.h>
#include <Engine.h>
#include <GLES3/gl32.h>
#include <Logging.h>
#include <fstream>
#include <regex>
#include <sstream>

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

GLuint LoadShaderModule(const std::string & filePath, GLenum type) {
  // TODO: Use FS Manager to load shader binary
  std::ifstream is(filePath, std::ios::binary | std::ios::in | std::ios::ate);
  DEBUG_RUNTIME_ASSERT_TRUE(is.is_open());
  auto shaderSize = (size_t)is.tellg();
  is.seekg(0, std::ios::beg);
  auto shaderSource = new char[shaderSize];
  is.read(shaderSource, shaderSize);
  is.close();
  DEBUG_RUNTIME_ASSERT_TRUE(shaderSize > 0);
  auto shader = glCreateShader(type);
  auto size = static_cast<GLint>(shaderSize);
  glShaderSource(shader, 1, &shaderSource, &size);
  glCompileShader(shader);
  {
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
      GLint errorLength;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errorLength);
      auto errorMessage =
          std::string(static_cast<unsigned long>(errorLength) - 1, 0);

      glGetShaderInfoLog(shader, errorLength, nullptr, errorMessage.data());

      auto ss = std::stringstream(errorMessage);
      std::string to;

      if (!errorMessage.empty()) {
        while (std::getline(ss, to, '\n')) {
          static const std::regex e(R"(^\d+:(\d+)\(\d+\): [^ ]+ (.*))");
          std::smatch m;
          std::regex_search(to, m, e);

          logging::Log(filePath,
                       static_cast<uint32_t>(std::stoi(m[1].first.base())),
                       logging::Level::Error, m[2].first.base());
        }
      }
    }
  }
  delete[] shaderSource;
  return shader;
}

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

  // Generate buffers
  glGenVertexArrays(1, &sceneVertexArray_);
  glGenBuffers(1, &sceneIndexBuffer_);
  glGenBuffers(1, &sceneVertexBuffer_);

  // Create Program
  scenePipelineProgram_ = glCreateProgram();
  auto vertModule = LoadShaderModule("opaque.vert", GL_VERTEX_SHADER);
  auto fragModule = LoadShaderModule("opaque.frag", GL_FRAGMENT_SHADER);
  glAttachShader(scenePipelineProgram_, vertModule);
  glAttachShader(scenePipelineProgram_, fragModule);
  glLinkProgram(scenePipelineProgram_);
  {
    GLint status;
    glGetProgramiv(scenePipelineProgram_, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
      GLint errorLength;
      glGetProgramiv(scenePipelineProgram_, GL_INFO_LOG_LENGTH, &errorLength);
      auto errorMessage =
          std::string(static_cast<unsigned long>(errorLength) - 1, 0);
      glGetProgramInfoLog(scenePipelineProgram_, errorLength, nullptr,
                          errorMessage.data());
      LOG(logging::Level::Error, errorMessage);
      DEBUG_RUNTIME_ASSERT_FALSE(status == GL_FALSE);
    }
  }
  glDetachShader(scenePipelineProgram_, vertModule);
  glDetachShader(scenePipelineProgram_, fragModule);
}

void RenderingManagerGles::GenerateCommands() {
  // TODO: For each existing mesh buffer, reuse or free
  // Merge all vertex and index data into single contiguous array
  std::vector<mesh::Index> indices;
  std::vector<mesh::Vertex> vertices;
  std::vector<MergedMeshIndexBufferLocation> indexBufferLocations;
  MergeMeshes(meshes_, indices, vertices, indexBufferLocations);

  // Stage and Upload index and vertex data
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sceneIndexBuffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mesh::Index) * indices.size(),
                 indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, sceneVertexBuffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mesh::Vertex) * vertices.size(),
                 vertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(sceneVertexArray_);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
  }

  commandQueue_.Reset();
  commandQueue_.glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1,
                                 "Clear Back Buffer");
  commandQueue_.glClearColor(clearColor_.r, clearColor_.g, clearColor_.b,
                             clearColor_.a);
  commandQueue_.glClear(static_cast<uint32_t>(GL_COLOR_BUFFER_BIT));
  commandQueue_.glPopDebugGroup();
  commandQueue_.glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1,
                                 "Draw Opaque Meshes");
  commandQueue_.glUseProgram(scenePipelineProgram_);
  for (auto & location : indexBufferLocations) {
    commandQueue_.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sceneIndexBuffer_);
    commandQueue_.glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1,
                                   location.name);
    static_assert(sizeof(mesh::Index) == 4);
    commandQueue_.glDrawElements(
        GL_TRIANGLES, static_cast<GLsizei>(location.count), GL_UNSIGNED_INT,
        reinterpret_cast<void *>(location.offset * sizeof(uint32_t)));
    commandQueue_.glPopDebugGroup();
  }
  commandQueue_.glPopDebugGroup();
}

void RenderingManagerGles::TerminateInternal() {
  DEBUG_RUNTIME_ASSERT_TRUE(isInitialized);
  glDeleteProgram(scenePipelineProgram_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &sceneIndexBuffer_);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &sceneVertexBuffer_);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &sceneVertexArray_);
  SDL_GL_DeleteContext(context_);
}

void RenderingManagerGles::Submit() {
  commandQueue_.Execute();
}

void RenderingManagerGles::Present() const {
  SDL_GL_SwapWindow(window_);
}
}  // namespace sce::rendering::private_
