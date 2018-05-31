//
// Created by sandy on 11/05/18.
//

#include "ScriptingManager.h"

#include <array>
#include <functional>
#include <vector>

#include <Debug.h>
#include <Game.h>
#include <Logging.h>

#include "FunctionTables.h"

namespace sce::scripting::private_ {

void ScriptingManager::Initialize() {
  if (initialized_)
    return;

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

void ScriptingManager::Terminate() {
  if (initialized_) {
    DEBUG_RUNTIME_ASSERT_NOT_NULL(lua_state_);
    lua_close(lua_state_);
  }
  initialized_ = false;
}

int ScriptingManager::RunSource(std::string source) {
  auto result = luaL_dostring(lua_state_, source.c_str());
  if (result != EXIT_SUCCESS) {
    auto error = lua_tostring(lua_state_, -1);
    logging::Log("", 0, logging::Level::Error, error);
  }
  return result;
}

int ScriptingManager::RunFile(std::string file) {
  auto result = luaL_dofile(lua_state_, file.c_str());
  if (result != EXIT_SUCCESS) {
    auto error = lua_tostring(lua_state_, -1);
    logging::Log("", 0, logging::Level::Error, error);
  }

  return result;
}
}  // namespace sce::scripting::private_
