//
// Created by sandy on 31/05/18.
//

#ifndef SCARTERENGINE_FUNCTION_TABLES_H
#define SCARTERENGINE_FUNCTION_TABLES_H

#include <array>
#include <map>
#include <sstream>
#include <vector>

#include <lua.hpp>

#include <Debug.h>
#include <Game.h>
#include <Logging.h>
#include <Scene.h>

#include "ScriptingUtils.h"

namespace sce::debug {
constexpr std::array<luaL_Reg, 3> GetFunctionTable() noexcept {
  return {
      luaL_Reg{"AssertTrue",
               [](lua_State * lua) -> int {
                 lua_Debug info = {};
                 DEBUG_RUNTIME_ASSERT_TRUE(lua_getstack(lua, 1, &info) == 1);
                 lua_getinfo(lua, "nSl", &info);

                 debug::runtime::AssertTrue(
                     info.short_src,
                     static_cast<uint32_t>(std::max(0, info.currentline)),
                     sce::scripting::private_::pop<bool>(lua, 1));

                 lua_remove(lua, -1);

                 return 0;
               }},
      luaL_Reg{"AssertFalse",
               [](lua_State * lua) -> int {
                 lua_Debug info = {};
                 DEBUG_RUNTIME_ASSERT_TRUE(lua_getstack(lua, 1, &info) == 1);
                 lua_getinfo(lua, "nSl", &info);

                 debug::runtime::AssertFalse(
                     info.short_src,
                     static_cast<uint32_t>(std::max(0, info.currentline)),
                     sce::scripting::private_::pop<bool>(lua, 1));

                 lua_remove(lua, -1);

                 return 0;
               }},
      luaL_Reg{"AssertNotNull",
               [](lua_State * lua) -> int {
                 lua_Debug info = {};
                 DEBUG_RUNTIME_ASSERT_TRUE(lua_getstack(lua, 1, &info) == 1);
                 lua_getinfo(lua, "nSl", &info);

                 debug::runtime::AssertNotNull(
                     info.short_src,
                     static_cast<uint32_t>(std::max(0, info.currentline)),
                     lua_isnoneornil(lua, 1) ? nullptr
                                             : reinterpret_cast<void *>(1));

                 return 0;
               }},
  };
}
static decltype(auto) function_table = GetFunctionTable();
}  // namespace sce::debug

namespace sce::logging {
constexpr std::array<luaL_Reg, 2> GetFunctionTable() noexcept {
  return {
      luaL_Reg{
          "SetLogLevel",
          [](lua_State * lua) -> int {
            lua_Debug info = {};
            DEBUG_RUNTIME_ASSERT_TRUE(lua_getstack(lua, 1, &info) == 1);
            lua_getinfo(lua, "nSl", &info);

            auto level = logging::Level::_from_string_nothrow(
                sce::scripting::private_::pop<std::string>(lua, 1).c_str());
            DEBUG_RUNTIME_ASSERT_TRUE(level);

            logging::SetLogLevel(*level);

            lua_remove(lua, -1);
            lua_remove(lua, -1);

            return 0;
          }},
      luaL_Reg{
          "Log",
          [](lua_State * lua) -> int {
            lua_Debug info = {};
            DEBUG_RUNTIME_ASSERT_TRUE(lua_getstack(lua, 1, &info) == 1);
            lua_getinfo(lua, "nSl", &info);

            auto level = logging::Level::_from_string_nothrow(
                sce::scripting::private_::pop<std::string>(lua, 1).c_str());
            DEBUG_RUNTIME_ASSERT_TRUE(level);

            logging::Log(info.short_src,
                         static_cast<uint32_t>(std::max(0, info.currentline)),
                         *level,
                         sce::scripting::private_::lua_to<std::string>(lua, 2));

            lua_remove(lua, -1);
            lua_remove(lua, -1);

            return 0;
          }},
  };
}
static decltype(auto) function_table = GetFunctionTable();
}  // namespace sce::logging

using sce::scene::Scene;
DEFINE_LUA_USERDATA_TYPE_PUSH(Scene)
DEFINE_LUA_USERDATA_TYPE_POP(Scene)
namespace sce::game {
constexpr std::array<luaL_Reg, 3> GetFunctionTable() noexcept {
  return {
      AUTO_BIND_C_FUNCTION_TO_LUA(std::shared_ptr<Scene>, AddScene,
                                  std::string),
      AUTO_BIND_C_FUNCTION_TO_LUA(
          void, SetCurrentScene,
          std::shared_ptr<Scene>),  // TODO: deprecate this
      AUTO_BIND_C_FUNCTION_TO_LUA(void, RemoveScene, std::string),
  };
}
constexpr std::array<luaL_Reg, 2> GetMetamethods() noexcept {
  return {
      luaL_Reg{"__newindex",
               [](lua_State * lua) -> int {
                 auto indexName =
                     sce::scripting::private_::pop<std::string>(lua, 2);
                 DEBUG_RUNTIME_ASSERT_FALSE(indexName.empty());

                 if (indexName == "CurrentScene") {
                   auto scene = lua_isnoneornil(lua, 3)
                                    ? nullptr
                                    : LUA_USERDATA_CAST(lua, 3, Scene);
                   SetCurrentScene(scene);
                 } else {
                   luaL_error(lua, "Setting index %s of Game is forbidden.",
                              indexName.c_str());
                 }
                 return 0;
               }},
      luaL_Reg{"__index",
               [](lua_State * lua) -> int {
                 auto indexName =
                     sce::scripting::private_::pop<std::string>(lua, 2);
                 DEBUG_RUNTIME_ASSERT_FALSE(indexName.empty());

                 if (indexName == "CurrentScene") {
                   auto scene = GetCurrentScene();
                   sce::scripting::private_::push(lua, scene);
                   return 1;
                 } else {
                   return lua_rawget(lua, 1);
                 }
               }},
  };
}
static decltype(auto) function_table = GetFunctionTable();
static decltype(auto) metamethods = GetMetamethods();
}  // namespace sce::game

namespace sce::scene {
using namespace sce::scripting::private_;
constexpr std::array<luaL_Reg, 2> GetFunctionTable() noexcept {
  return {
      // TODO: Implicitly add this
      LUA_USERDATA_GC_REG(Scene),
      luaL_Reg{"__tostring",
               [](lua_State * L) -> int {
                 auto scene = LUA_USERDATA_CAST(L, 1, Scene);
                 std::stringstream ss;
                 ss << "Scene"
                    << " {"
                    << " name=\"" << scene->Name().c_str() << "\", "
                    << "}";
                 sce::scripting::private_::push(L, ss.str().c_str());
                 return 1;
               }},
  };
}
static decltype(auto) function_table = GetFunctionTable();
}  // namespace sce::scene

namespace sce::scripting::private_ {

static decltype(auto) function_tables = std::array<functionNamespace, 4>{
    functionNamespace{"Debug", false, sce::debug::function_table.size(),
                      sce::debug::function_table.data()},
    functionNamespace{"Logging", false, sce::logging::function_table.size(),
                      sce::logging::function_table.data()},
    functionNamespace{"Game", false, sce::game::function_table.size(),
                      sce::game::function_table.data(),
                      sce::game::metamethods.size(),
                      sce::game::metamethods.data()},
    functionNamespace{"Scene", true, sce::scene::function_table.size(),
                      sce::scene::function_table.data()},
};
}  // namespace sce::scripting::private_

#endif  // SCARTERENGINE_FUNCTION_TABLES_H
