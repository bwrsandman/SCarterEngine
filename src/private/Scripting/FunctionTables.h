//
// Created by sandy on 31/05/18.
//

#ifndef SCARTERENGINE_FUNCTION_TABLES_H
#define SCARTERENGINE_FUNCTION_TABLES_H

#include <array>
#include <sstream>
#include <vector>

#include <lua.hpp>

#include <Debug.h>
#include <Logging.h>

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

namespace sce::scripting::private_ {

static decltype(auto) function_tables = std::array<functionNamespace, 2>{
    functionNamespace{"Debug", false, sce::debug::function_table.size(),
                      sce::debug::function_table.data()},
    functionNamespace{"Logging", false, sce::logging::function_table.size(),
                      sce::logging::function_table.data()},
};
}  // namespace sce::scripting::private_

#endif  // SCARTERENGINE_FUNCTION_TABLES_H
