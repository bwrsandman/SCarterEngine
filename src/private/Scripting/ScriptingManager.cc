//
// Created by sandy on 11/05/18.
//

#include "ScriptingManager.h"

#include <array>
#include <functional>
#include <vector>

#include <Debug.h>
#include <Engine.h>
#include <Game.h>
#include <Logging.h>
#include <sys/stat.h>

#include "FunctionTables.h"

namespace sce::scripting::private_ {

void ScriptingManager::Initialize(bool reload) {
  if (initialized_)
    return;

  configured_ = false;

  lua_state_ = luaL_newstate();

  if (lua_state_ == nullptr) {
    LOG(logging::Level::Fatal, "Could not create new Lua state.");
    return;
  }

  luaL_openlibs(lua_state_);

  lua_newtable(lua_state_);

  // Registering Namespaces
  // TODO use luaL_newlib and luaL_newlibtable
  for (auto & function_table : function_tables) {
    lua_newtable(lua_state_);
    lua_setfield(lua_state_, -2, function_table.name);
  }

  for (auto & function_table : function_tables) {
    // Registering class functions
    if (function_table.isClass) {
      // Create the type metatable
      luaL_newmetatable(lua_state_, function_table.name);
      lua_pushvalue(lua_state_, -1);  // Push copy of metatable onto stack
      lua_setfield(lua_state_, -2, "__index");  // metatable.__index = metatable
    } else {
      lua_newtable(lua_state_);
    }
    // Register static functions
    // TODO use luaL_setfuncs
    for (auto i = 0; i < function_table.func_count; ++i) {
      lua_pushcclosure(lua_state_, function_table.funcs[i].func, 0);
      lua_setfield(lua_state_, -2, function_table.funcs[i].name);
    }
    if (function_table.isClass) {
      lua_pop(lua_state_, 1);  // Remove metatable from stack
    } else {
      if (function_table.metamethod_count) {
        auto name = function_table.name + std::string("Metatable");
        luaL_newmetatable(lua_state_, name.c_str());
        for (auto i = 0; i < function_table.metamethod_count; ++i) {
          lua_pushcclosure(lua_state_, function_table.metamethods[i].func, 0);
          lua_setfield(lua_state_, -2, function_table.metamethods[i].name);
        }
        lua_setmetatable(lua_state_, -2);
      }
      lua_setfield(lua_state_, -2, function_table.name);
    }
  }

  lua_setglobal(lua_state_, "Engine");

  initialized_ = true;
}

void ScriptingManager::Terminate(bool reload) {
  if (initialized_) {
    DEBUG_RUNTIME_ASSERT_NOT_NULL(lua_state_);
    RunScriptTerminate();
    lua_close(lua_state_);
  }
  initialized_ = false;
}

void ScriptingManager::Check(int load_result) {
  if (load_result != EXIT_SUCCESS) {
    auto error = lua_tostring(lua_state_, -1);
    logging::Log("", 0, logging::Level::Error, error);
    engine::ScheduleQuit();
    return;
  }

  int loop = lua_getglobal(lua_state_, "Loop");
  if (loop == 0) {
    LOG(logging::Level::Warning,
        "Lua Script has no \"Loop()\" function, game will quit.");
    engine::ScheduleQuit();
    return;
  }
  if (!lua_isfunction(lua_state_, -1)) {
    LOG(logging::Level::Warning,
        "Lua Global \"Loop\" is not a function, game will quit.");
    engine::ScheduleQuit();
    return;
  }

  configured_ = true;
}

int ScriptingManager::LoadSource(std::string source) {
  auto result = luaL_dostring(lua_state_, source.c_str());
  Check(result);
  RunScriptInit();
  scriptIsFile_ = false;
  scriptSource_ = source;
  scriptLastModified_ = std::time(nullptr);
  return result;
}

int ScriptingManager::LoadFile(std::string file) {
  int result = luaL_dofile(lua_state_, file.c_str());
  Check(result);
  RunScriptInit();
  struct stat file_stat {};
  result = ::stat(file.c_str(), &file_stat);
  if (result != 0) {
    engine::ScheduleQuit();
    LOG(logging::Level::Error, "Could not stat file");
    return result;
  }
  scriptIsFile_ = true;
  scriptSource_ = file;
  scriptLastModified_ = file_stat.st_mtime;
  return result;
}

bool ScriptingManager::ScriptModified() {
  DEBUG_RUNTIME_ASSERT_TRUE(initialized_);
  DEBUG_RUNTIME_ASSERT_TRUE(configured_);

  if (scriptIsFile_) {
    struct stat file_stat {};
    auto result = ::stat(scriptSource_.c_str(), &file_stat);
    if (result != 0) {
      LOG(logging::Level::Warning, "Could not stat file");
      return false;
    }
    return scriptLastModified_ < file_stat.st_mtime;
  } else {
    // TODO
    return false;
  }
}

int ScriptingManager::ReloadScript() {
  int result;
  RunScriptTerminate();
  configured_ = false;
  engine::Reload();
  if (scriptIsFile_) {
    result = LoadFile(scriptSource_);
  } else {
    result = LoadSource(scriptSource_);
  }
  return result;
}

void ScriptingManager::RunFrame(double dt) {
  DEBUG_RUNTIME_ASSERT_TRUE(initialized_);
  DEBUG_RUNTIME_ASSERT_TRUE(configured_);

  if (!GetAndCheckFunction("Loop")) {
    LOG(logging::Level::Warning, "Game will quit.");
    engine::ScheduleQuit();
    return;
  }
  push(lua_state_, dt);
  lua_call(lua_state_, 1, 0);

  int Engine = lua_getglobal(lua_state_, "Engine");
  if (Engine == 0) {
    LOG(logging::Level::Fatal, "Lua Global \"Engine\" is missing.");
    engine::ScheduleQuit();
    return;
  }
  if (!lua_istable(lua_state_, -1)) {
    LOG(logging::Level::Fatal, "Lua Global \"Engine\" is no longer a table.");
    engine::ScheduleQuit();
    lua_remove(lua_state_, -1);
    return;
  }

  int shouldQuit = lua_getfield(lua_state_, -1, "Quit");
  if (shouldQuit > 0) {
    if (lua_isfunction(lua_state_, -1)) {
      lua_call(lua_state_, 0, 1);
      if (pop<bool>(lua_state_, -1))
        engine::ScheduleQuit();
      lua_remove(lua_state_, -1);
      lua_remove(lua_state_, -1);
      return;
    }
    if (lua_is<bool>(lua_state_, -1)) {
      if (pop<bool>(lua_state_, -1))
        engine::ScheduleQuit();
      lua_remove(lua_state_, -1);
      lua_remove(lua_state_, -1);
      return;
    }
    lua_remove(lua_state_, -1);
    LOG(logging::Level::Warning,
        "Lua Global \"Engine.Quit\" could not be interpreted.");
  }

  if (ScriptModified()) {
    ReloadScript();
  }
}

bool ScriptingManager::GetAndCheckFunction(std::string name) {
  int func = lua_getglobal(lua_state_, name.c_str());
  if (func == 0) {
    LOG(logging::Level::Warning,
        "Lua Script has no \"" + name + "()\" function.");
    lua_remove(lua_state_, -1);
    return false;
  }
  if (!lua_isfunction(lua_state_, -1)) {
    LOG(logging::Level::Warning,
        "Lua Global \"" + name + "\" is not a function.");
    lua_remove(lua_state_, -1);
    return false;
  }
  return true;
}

void ScriptingManager::RunScriptInit() {
  if (!GetAndCheckFunction("Initialize")) {
    lua_remove(lua_state_, -1);
    return;
  }
  lua_call(lua_state_, 0, 0);
}

void ScriptingManager::RunScriptTerminate() {
  if (!GetAndCheckFunction("Terminate")) {
    lua_remove(lua_state_, -1);
    return;
  }
  lua_call(lua_state_, 0, 0);
}
}  // namespace sce::scripting::private_
