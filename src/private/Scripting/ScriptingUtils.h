//
// Created by sandy on 31/05/18.
//

#ifndef SCARTERENGINE_SCRIPTING_PRIVATE_SCRIPTING_UTILS_H
#define SCARTERENGINE_SCRIPTING_PRIVATE_SCRIPTING_UTILS_H

#include <deque>
#include <forward_list>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <lua.hpp>

#include <Debug.h>

// FIXME: Once P0433R2 is implemented in libc++, template args can be removed
// https://libcxx.llvm.org/cxx1z_status.html
// #define AUTO_BIND_C_FUNCTION_TO_LUA_ALIASED(alias, func) ...
#define AUTO_BIND_C_FUNCTION_TO_LUA_ALIASED(alias, R, func, As...) \
  luaL_Reg {                                                       \
    alias, [](lua_State * L) -> int {                              \
      using sce::scripting::private_::MakeCallableFunc;            \
      return MakeCallableFunc(L, std::function<R(As)>(func));      \
    }                                                              \
  }

// FIXME: Once P0433R2 is implemented in libc++, template args can be removed
// https://libcxx.llvm.org/cxx1z_status.html
// #define AUTO_BIND_C_FUNCTION_TO_LUA(func) \
//   AUTO_BIND_C_FUNCTION_TO_LUA_ALIASED(#func, func)
#define AUTO_BIND_C_FUNCTION_TO_LUA(R, func, As...) \
  AUTO_BIND_C_FUNCTION_TO_LUA_ALIASED(#func, R, func, As)

#define LUA_USERDATA_CAST(L, pos, T) \
  *sce::scripting::private_::LUA_USERDATA_CAST_PTR<T>(L, pos, #T)

#define DEFINE_LUA_USERDATA_TYPE_PUSH(T)                                       \
  template <>                                                                  \
  void sce::scripting::private_::push(lua_State * L, std::shared_ptr<T> obj) { \
    sce::scripting::private_::push(L, std::move(obj), #T);                     \
  }
#define DEFINE_LUA_USERDATA_TYPE_POP(T)                                    \
  template <>                                                              \
  bool sce::scripting::private_::lua_is<std::shared_ptr<T>>(lua_State * L, \
                                                            int idx) {     \
    bool isNull = lua_isnoneornil(L, idx);                                 \
    return isNull || static_cast<bool>(lua_isuserdata(L, idx));            \
  }                                                                        \
  template <>                                                              \
  std::shared_ptr<T> sce::scripting::private_::lua_to<std::shared_ptr<T>>( \
      lua_State * L, int idx) {                                            \
    return LUA_USERDATA_CAST(L, idx, T);                                   \
  }

#define LUA_USERDATA_GC_REG(T)                                               \
  luaL_Reg {                                                                 \
    "__gc", [](lua_State * L) -> int {                                       \
      sce::scripting::private_::LUA_USERDATA_CAST_PTR<T>(L, 1, #T)->reset(); \
      return 0;                                                              \
    }                                                                        \
  }

// TODO: use template expansion and constexpr
// FIXME: Once P0433R2 is implemented in libc++, template args can be removed
// https://libcxx.llvm.org/cxx1z_status.html
// #define AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA(T, func, As...) \
//   AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA_ALIASED(#func, T, func, As)
#define AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA0(T, R, func) \
  AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA_ALIASED0(#func, T, R, func)
#define AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA1(T, R, func, As...) \
  AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA_ALIASED1(#func, T, R, func, As)
#define AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA2(T, R, func, As...) \
  AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA_ALIASED2(#func, T, R, func, As)
#define AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA3(T, R, func, As...) \
  AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA_ALIASED3(#func, T, R, func, As)
#define AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA_ALIASED0(alias, T, R, func)      \
  _AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA_ALIASED_OPEN(alias, T, R, func, void) \
  _AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA_ALIASED_CLOSE()
#define AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA_ALIASED1(alias, T, R, func, A0) \
  _AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA_ALIASED_OPEN(alias, T, R, func, A0), \
      _1 _AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA_ALIASED_CLOSE()
#define AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA_ALIASED2(alias, T, R, func, A0, \
                                                      A1)                    \
  _AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA_ALIASED_OPEN(alias, T, R, func, A0,  \
                                                     A1),                    \
      _1, _2 _AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA_ALIASED_CLOSE()
#define AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA_ALIASED3(alias, T, R, func, A0, \
                                                      A1, A2)                \
  _AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA_ALIASED_OPEN(alias, T, R, func, A0,  \
                                                     A1, A2),                \
      _1, _2, _3 _AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA_ALIASED_CLOSE()

#define _AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA_ALIASED_OPEN(alias, T, R, func, \
                                                           As...)             \
  luaL_Reg {                                                                  \
    alias, [](lua_State * L) -> int {                                      \
      auto ud = LUA_USERDATA_CAST(L, 1, T);                               \
      using sce::scripting::private_::MakeCallableFunc;                   \
      using std::placeholders::_1;                                           \
      using std::placeholders::_2;                                           \
      using std::placeholders::_3;                                           \
      std::function<R(As)> member_func = std::bind(&T::func, ud.get()
#define _AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA_ALIASED_CLOSE() \
      );                                                      \
  return MakeCallableFunc(L, 2, member_func, member_func);    \
  }                                                           \
  }

namespace sce::scripting::private_ {

// https://stackoverflow.com/questions/9407367/determine-if-a-type-is-an-stl-container-at-compile-time/31105859
namespace is_stl_container_impl {
template <typename T>
struct is_stl_container : std::false_type {};
template <typename T, std::size_t N>
struct is_stl_container<std::array<T, N>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::vector<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::deque<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::list<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::forward_list<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::set<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::multiset<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::map<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::multimap<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::unordered_set<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::unordered_multiset<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::unordered_map<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::unordered_multimap<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::stack<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::queue<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::priority_queue<Args...>> : std::true_type {};
}  // namespace is_stl_container_impl
// type trait to utilize the implementation type traits as well as decay the
// type
template <typename T>
struct is_stl_container {
  static constexpr bool const value =
      is_stl_container_impl::is_stl_container<std::decay_t<T>>::value;
};

template <typename T>
static std::shared_ptr<T> * LUA_USERDATA_CAST_PTR(lua_State * L, int pos,
                                                  const char * typename_) {
  if (lua_isnoneornil(L, pos))
    return nullptr;
  return static_cast<std::shared_ptr<T> *>(luaL_checkudata(L, pos, typename_));
}

template <typename T>
static void push(lua_State * L, T obj);

template <typename T>
static bool lua_is(lua_State * L, int idx);

template <typename T>
static T lua_to(lua_State * L, int idx);

// Regular types
template <typename T, std::enable_if_t<!is_stl_container<T>::value> * = nullptr>
static T pop(lua_State * L, int idx) {
  DEBUG_RUNTIME_ASSERT_TRUE(lua_is<T>(L, idx) != 0);
  return lua_to<T>(L, idx);
}

// Container types
template <typename T, typename VT, typename>
static T pop_container(lua_State * L, int idx);

// Bool
template <>
bool lua_is<bool>(lua_State * L, int idx) {
  return static_cast<bool>(lua_isboolean(L, idx));
}

template <>
bool lua_to<bool>(lua_State * L, int idx) {
  return static_cast<bool>(lua_toboolean(L, idx));
}

// 8 Bit Signed Int
template <>
void push(lua_State * L, int8_t obj) {
  lua_pushinteger(L, obj);
}

template <>
bool lua_is<int8_t>(lua_State * L, int idx) {
  return static_cast<bool>(lua_isinteger(L, idx));
}

template <>
int8_t lua_to<int8_t>(lua_State * L, int idx) {
  return static_cast<int8_t>(lua_tointeger(L, idx));
}

// 16 Bit Signed Int
template <>
void push(lua_State * L, int16_t obj) {
  lua_pushinteger(L, obj);
}

template <>
bool lua_is<int16_t>(lua_State * L, int idx) {
  return static_cast<bool>(lua_isinteger(L, idx));
}

template <>
int16_t lua_to<int16_t>(lua_State * L, int idx) {
  return static_cast<int16_t>(lua_tointeger(L, idx));
}

// 32 Bit Unsigned Int
template <>
void push(lua_State * L, int32_t obj) {
  lua_pushinteger(L, obj);
}

template <>
bool lua_is<int32_t>(lua_State * L, int idx) {
  return static_cast<bool>(lua_isinteger(L, idx));
}

template <>
int32_t lua_to<int32_t>(lua_State * L, int idx) {
  return static_cast<int32_t>(lua_tointeger(L, idx));
}

// 64 Bit Signed Int
template <>
void push(lua_State * L, int64_t obj) {
  lua_pushinteger(L, obj);
}

template <>
bool lua_is<int64_t>(lua_State * L, int idx) {
  return static_cast<bool>(lua_isinteger(L, idx));
}

template <>
int64_t lua_to<int64_t>(lua_State * L, int idx) {
  return static_cast<int64_t>(lua_tointeger(L, idx));
}

// 8 Bit Unsigned Int
template <>
void push(lua_State * L, uint8_t obj) {
  lua_pushinteger(L, obj);
}

template <>
bool lua_is<uint8_t>(lua_State * L, int idx) {
  return static_cast<bool>(lua_isinteger(L, idx));
}

template <>
uint8_t lua_to<uint8_t>(lua_State * L, int idx) {
  return static_cast<uint8_t>(lua_tointeger(L, idx));
}

// 16 Bit Unsigned Int
template <>
void push(lua_State * L, uint16_t obj) {
  lua_pushinteger(L, obj);
}

template <>
bool lua_is<uint16_t>(lua_State * L, int idx) {
  return static_cast<bool>(lua_isinteger(L, idx));
}

template <>
uint16_t lua_to<uint16_t>(lua_State * L, int idx) {
  return static_cast<uint16_t>(lua_tointeger(L, idx));
}

// 32 Bit Unsigned Int
template <>
void push(lua_State * L, uint32_t obj) {
  lua_pushinteger(L, obj);
}

template <>
bool lua_is<uint32_t>(lua_State * L, int idx) {
  return static_cast<bool>(lua_isinteger(L, idx));
}

template <>
uint32_t lua_to<uint32_t>(lua_State * L, int idx) {
  return static_cast<uint32_t>(lua_tointeger(L, idx));
}

// 64 Bit Unsigned Int
template <>
void push(lua_State * L, uint64_t obj) {
  lua_pushinteger(L, obj);
}

template <>
bool lua_is<uint64_t>(lua_State * L, int idx) {
  return static_cast<bool>(lua_isinteger(L, idx));
}

template <>
uint64_t lua_to<uint64_t>(lua_State * L, int idx) {
  return static_cast<uint64_t>(lua_tointeger(L, idx));
}

// Float
template <>
void push(lua_State * L, float obj) {
  lua_pushnumber(L, obj);
}

template <>
bool lua_is<float>(lua_State * L, int idx) {
  return static_cast<bool>(lua_isnumber(L, idx));
}

template <>
float lua_to<float>(lua_State * L, int idx) {
  return static_cast<float>(lua_tonumber(L, idx));
}

// Double
template <>
void push(lua_State * L, double obj) {
  lua_pushnumber(L, obj);
}

template <>
bool lua_is<double>(lua_State * L, int idx) {
  return static_cast<bool>(lua_isnumber(L, idx));
}

template <>
double lua_to<double>(lua_State * L, int idx) {
  return lua_tonumber(L, idx);
}

// C-String
template <>
void push(lua_State * L, const char * obj) {
  lua_pushstring(L, obj);
}

template <>
bool lua_is<const char *>(lua_State * L, int idx) {
  return static_cast<bool>(lua_isstring(L, idx));
}

template <>
const char * lua_to<const char *>(lua_State * L, int idx) {
  return luaL_tolstring(L, idx, nullptr);
}

// String
template <>
void push(lua_State * L, std::string obj) {
  lua_pushstring(L, obj.c_str());
}

template <>
bool lua_is<std::string>(lua_State * L, int idx) {
  return static_cast<bool>(lua_isstring(L, idx));
}

template <>
std::string lua_to<std::string>(lua_State * L, int idx) {
  auto c_str = luaL_tolstring(L, idx, nullptr);
  auto res = std::string(c_str);
  return res;
}

// Shared Pointer
template <typename T>
void push(lua_State * L, std::shared_ptr<T> obj,
          const std::string & typename_) {
  auto ud = lua_newuserdata(L, sizeof(std::shared_ptr<T>));
  DEBUG_RUNTIME_ASSERT_NOT_NULL(ud);
  std::memset(ud, 0, sizeof(std::shared_ptr<T>));
  auto mt = luaL_getmetatable(L, typename_.c_str());
  DEBUG_RUNTIME_ASSERT_TRUE(mt > 0);
  lua_setmetatable(L, -2);
  *static_cast<std::shared_ptr<T> *>(ud) = obj;
}

// TODO: lua_is
// TODO: lua_to

// Unordered Map
template <typename K, typename T>
static void push(lua_State * L, std::unordered_map<K, T> m) {
  lua_createtable(L, static_cast<int>(m.size()), 4);
  for (auto & i : m) {
    push(L, i.first);
    push(L, i.second);
    lua_settable(L, -3);
  }
}

template <typename T, typename VT,
          typename K = std::remove_const_t<typename VT::first_type>,
          typename V = typename VT::second_type,
          std::enable_if_t<std::is_same<T, std::unordered_map<K, V>>::value> * =
              nullptr>
T pop_container(lua_State * L, int idx) {
  DEBUG_RUNTIME_ASSERT_TRUE(lua_istable(L, idx) != 0);
  auto size = static_cast<size_t>(luaL_len(L, idx));

  T res(size);
  lua_pushnil(L);           // first key
  while (lua_next(L, idx))  // pops key and pushes key, value
  {
    auto value = pop<V>(L, -1);
    lua_pushvalue(L, -1);
    auto key = pop<K>(L, -1);
    res[key] = value;
    lua_pop(L, 1);
  }

  return res;
}

// Vector

template <typename T>
void push(lua_State * L, std::vector<T> & obj) {
  lua_createtable(L, static_cast<int>(obj.size()), 0);
  for (auto i = 0; i < obj.size(); ++i) {
    push(L, obj[i]);
    lua_rawseti(L, -2, i + 1);
  }
}

template <typename T, typename VT,
          std::enable_if_t<std::is_same<T, std::vector<VT>>::value> * = nullptr>
T pop_container(lua_State * L, int idx) {
  DEBUG_RUNTIME_ASSERT_TRUE(lua_istable(L, idx) != 0);
  auto size = static_cast<size_t>(luaL_len(L, idx));

  T res(size);
  for (auto i = 0; i < size; ++i) {
    lua_rawgeti(L, idx, i + 1);
    res[i] = pop<VT>(L, -1);
    lua_pop(L, 1);
  }

  return res;
}

// Containers
template <typename T, std::enable_if_t<is_stl_container<T>::value> * = nullptr>
static T pop(lua_State * L, int idx) {
  typedef typename T::value_type VT;
  return pop_container<T, VT>(L, idx);
}

template <typename... Ts>
static int push_args(lua_State * L, Ts... args) {
  (push(L, args), ...);
  return sizeof...(args);
}

// Void returns
template <typename... As, typename... Ps>
int MakeCallableFunc(lua_State * L, int, std::function<void(Ps...)> fn,
                     const std::function<void()> &, As... params) {
  fn(std::forward<As>(params)...);
  return push_args(L);
}

// Other single returns
template <typename R, typename... As, typename... Ps>
int MakeCallableFunc(lua_State * L, int, std::function<R(Ps...)> fn,
                     std::function<R()>, As... params) {
  auto res = fn(std::forward<As>(params)...);
  return push_args(L, res);
}

// std::pair returns
template <typename T0, typename T1, typename... As, typename... Ps>
int MakeCallableFunc(lua_State * L, int,
                     std::function<std::pair<T0, T1>(Ps...)> fn,
                     std::function<std::pair<T0, T1>()>, As... params) {
  T0 res0;
  T1 res1;
  std::tie(res0, res1) = fn(std::forward<As>(params)...);
  return push_args(L, res1, res0);
}

// std::array returns
template <typename T, size_t N, typename... As, typename... Ps>
int MakeCallableFunc(lua_State * L, int,
                     std::function<std::array<T, N>(Ps...)> fn,
                     std::function<std::array<T, N>()>, As... params) {
  auto res = fn(std::forward<As>(params)...);
  for (auto i = static_cast<int>(N - 1); i >= 0; --i) {
    push_args(L, res[i]);
  }
  return static_cast<int>(N);
}

// Handle arguments
template <typename AT, typename R, typename... As, typename... Ts,
          typename... Ps,
          typename = std::enable_if_t<!std::is_array<AT>::value>>
int MakeCallableFunc(lua_State * L, int i, std::function<R(As...)> fn,
                     std::function<R(AT, Ts...)>, Ps... params) {
  std::function<R(Ts...)> fn3;
  return MakeCallableFunc(L, i + 1, fn, fn3, std::forward<Ps>(params)...,
                          pop<AT>(L, i));
}

// Add helper for entry in to creating the binding
template <typename R, typename... As>
int MakeCallableFunc(lua_State * L, std::function<R(As...)> fn) {
  return MakeCallableFunc(L, 1, fn, fn);
}

struct functionNamespace {
  const char * name;
  const bool isClass;
  const size_t func_count;
  const luaL_Reg * funcs;
  const size_t metamethod_count;
  const luaL_Reg * metamethods;
};

}  // namespace sce::scripting::private_

#endif  // SCARTERENGINE_SCRIPTING_PRIVATE_SCRIPTING_UTILS_H
