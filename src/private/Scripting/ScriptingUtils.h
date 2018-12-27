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
#include <SDL_keycode.h>

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

std::string SDLKeyCodeToString(SDL_Keycode keycode) {
  char res[2] = {0};
  switch (keycode) {
    default:
    case SDLK_UNKNOWN:
      return "";
    case SDLK_RETURN:
      return "return";
    case SDLK_ESCAPE:
      return "escape";
    case SDLK_BACKSPACE:
      return "backspace";
    case SDLK_TAB:
      return "tab";
    case SDLK_SPACE:
      return "space";
    case SDLK_CAPSLOCK:
      return "capslock";
    case SDLK_PRINTSCREEN:
      return "printscreen";
    case SDLK_SCROLLLOCK:
      return "scrolllock";
    case SDLK_PAUSE:
      return "pause";
    case SDLK_INSERT:
      return "insert";
    case SDLK_HOME:
      return "home";
    case SDLK_PAGEUP:
      return "pageup";
    case SDLK_DELETE:
      return "delete";
    case SDLK_END:
      return "end";
    case SDLK_PAGEDOWN:
      return "pagedown";
    case SDLK_RIGHT:
      return "right";
    case SDLK_LEFT:
      return "left";
    case SDLK_DOWN:
      return "down";
    case SDLK_UP:
      return "up";
    case SDLK_POWER:
      return "power";
    case SDLK_EXECUTE:
      return "execute";
    case SDLK_HELP:
      return "help";
    case SDLK_MENU:
      return "menu";
    case SDLK_SELECT:
      return "select";
    case SDLK_STOP:
      return "stop";
    case SDLK_AGAIN:
      return "again";
    case SDLK_UNDO:
      return "undo";
    case SDLK_CUT:
      return "cut";
    case SDLK_COPY:
      return "copy";
    case SDLK_PASTE:
      return "paste";
    case SDLK_FIND:
      return "find";
    case SDLK_MUTE:
      return "mute";
    case SDLK_VOLUMEUP:
      return "volumeup";
    case SDLK_VOLUMEDOWN:
      return "volumedown";
    case SDLK_ALTERASE:
      return "alterase";
    case SDLK_SYSREQ:
      return "sysreq";
    case SDLK_CANCEL:
      return "cancel";
    case SDLK_CLEAR:
      return "clear";
    case SDLK_PRIOR:
      return "prior";
    case SDLK_RETURN2:
      return "return2";
    case SDLK_SEPARATOR:
      return "separator";
    case SDLK_OUT:
      return "out";
    case SDLK_OPER:
      return "oper";
    case SDLK_CLEARAGAIN:
      return "clearagain";
    case SDLK_CRSEL:
      return "crsel";
    case SDLK_EXSEL:
      return "exsel";
    case SDLK_THOUSANDSSEPARATOR:
      return "thousandsseparator";
    case SDLK_DECIMALSEPARATOR:
      return "decimalseparator";
    case SDLK_CURRENCYUNIT:
      return "currencyunity";
    case SDLK_CURRENCYSUBUNIT:
      return "currencysubunit";
    case SDLK_LCTRL:
      return "lctrl";
    case SDLK_LSHIFT:
      return "lshift";
    case SDLK_LALT:
      return "lalt";
    case SDLK_LGUI:
      return "lgui";
    case SDLK_RCTRL:
      return "rctrl";
    case SDLK_RSHIFT:
      return "rshift";
    case SDLK_RALT:
      return "ralt";
    case SDLK_RGUI:
      return "rgui";
    case SDLK_MODE:
      return "mode";
    case SDLK_AUDIONEXT:
      return "audionext";
    case SDLK_AUDIOPREV:
      return "audioprev";
    case SDLK_AUDIOSTOP:
      return "audiostop";
    case SDLK_AUDIOPLAY:
      return "audioplay";
    case SDLK_AUDIOMUTE:
      return "audiomute";
    case SDLK_AUDIOREWIND:
      return "audiorewind";
    case SDLK_AUDIOFASTFORWARD:
      return "audiofastforward";
    case SDLK_MEDIASELECT:
      return "mediaselect";
    case SDLK_WWW:
      return "www";
    case SDLK_MAIL:
      return "mail";
    case SDLK_CALCULATOR:
      return "calculator";
    case SDLK_COMPUTER:
      return "computer";
    case SDLK_AC_SEARCH:
      return "appsearch";
    case SDLK_AC_HOME:
      return "apphome";
    case SDLK_AC_BACK:
      return "appback";
    case SDLK_AC_FORWARD:
      return "appforward";
    case SDLK_AC_STOP:
      return "appstop";
    case SDLK_AC_REFRESH:
      return "apprefresh";
    case SDLK_AC_BOOKMARKS:
      return "appbookmarks";
    case SDLK_BRIGHTNESSDOWN:
      return "brightnessdown";
    case SDLK_BRIGHTNESSUP:
      return "brightnessup";
    case SDLK_DISPLAYSWITCH:
      return "displayswitch";
    case SDLK_KBDILLUMTOGGLE:
      return "kbillumtoggle";
    case SDLK_KBDILLUMDOWN:
      return "kbillumdown";
    case SDLK_KBDILLUMUP:
      return "kbillumup";
    case SDLK_EJECT:
      return "eject";
    case SDLK_SLEEP:
      return "sleep";
    case SDLK_APP1:
      return "app1";
    case SDLK_APP2:
      return "app2";
    case SDLK_F1:
      return "f1";
    case SDLK_F2:
      return "f2";
    case SDLK_F3:
      return "f3";
    case SDLK_F4:
      return "f4";
    case SDLK_F5:
      return "f5";
    case SDLK_F6:
      return "f6";
    case SDLK_F7:
      return "f7";
    case SDLK_F8:
      return "f8";
    case SDLK_F9:
      return "f9";
    case SDLK_F10:
      return "f10";
    case SDLK_F11:
      return "f11";
    case SDLK_F12:
      return "f12";
    case SDLK_F13:
      return "f13";
    case SDLK_F14:
      return "f14";
    case SDLK_F15:
      return "f15";
    case SDLK_F16:
      return "f16";
    case SDLK_F17:
      return "f17";
    case SDLK_F18:
      return "f18";
    case SDLK_F19:
      return "f19";
    case SDLK_F20:
      return "f20";
    case SDLK_F21:
      return "f21";
    case SDLK_F22:
      return "f22";
    case SDLK_F23:
      return "f23";
    case SDLK_F24:
      return "f24";
    case SDLK_KP_DIVIDE:
      return "kp/";
    case SDLK_KP_MULTIPLY:
      return "kp*";
    case SDLK_KP_MINUS:
      return "kp-";
    case SDLK_KP_PLUS:
      return "kp+";
    case SDLK_KP_ENTER:
      return "kpenter";
    case SDLK_KP_1:
      return "kp1";
    case SDLK_KP_2:
      return "kp2";
    case SDLK_KP_3:
      return "kp3";
    case SDLK_KP_4:
      return "kp4";
    case SDLK_KP_5:
      return "kp5";
    case SDLK_KP_6:
      return "kp6";
    case SDLK_KP_7:
      return "kp7";
    case SDLK_KP_8:
      return "kp8";
    case SDLK_KP_9:
      return "kp9";
    case SDLK_KP_0:
      return "kp0";
    case SDLK_KP_PERIOD:
      return "kp.";
    case SDLK_KP_COMMA:
      return "kp,";
    case SDLK_KP_EQUALS:
      return "kp=";
    case SDLK_KP_EQUALSAS400:
      return "kpqualsas400";
    case SDLK_KP_00:
      return "kp00";
    case SDLK_KP_000:
      return "kp000";
    case SDLK_KP_LEFTPAREN:
      return "kp(";
    case SDLK_KP_RIGHTPAREN:
      return "kp)";
    case SDLK_KP_LEFTBRACE:
      return "kp{";
    case SDLK_KP_RIGHTBRACE:
      return "kp}";
    case SDLK_KP_TAB:
      return "kptab";
    case SDLK_KP_BACKSPACE:
      return "kpbackspace";
    case SDLK_KP_A:
      return "kpa";
    case SDLK_KP_B:
      return "kpb";
    case SDLK_KP_C:
      return "kpc";
    case SDLK_KP_D:
      return "kpd";
    case SDLK_KP_E:
      return "kpe";
    case SDLK_KP_F:
      return "kpf";
    case SDLK_KP_XOR:
      return "kpxor";
    case SDLK_KP_POWER:
      return "kppower";
    case SDLK_KP_PERCENT:
      return "kp%";
    case SDLK_KP_LESS:
      return "kpless";
    case SDLK_KP_GREATER:
      return "kpgreater";
    case SDLK_KP_AMPERSAND:
      return "kp&";
    case SDLK_KP_DBLAMPERSAND:
      return "kp&&";
    case SDLK_KP_VERTICALBAR:
      return "kp|";
    case SDLK_KP_DBLVERTICALBAR:
      return "kp||";
    case SDLK_KP_COLON:
      return "kp:";
    case SDLK_KP_HASH:
      return "kp#";
    case SDLK_KP_SPACE:
      return "kpspace";
    case SDLK_KP_AT:
      return "kp@";
    case SDLK_KP_EXCLAM:
      return "kp!";
    case SDLK_KP_MEMSTORE:
      return "kpmemstore";
    case SDLK_KP_MEMRECALL:
      return "kpmemrecall";
    case SDLK_KP_MEMCLEAR:
      return "kpmemclear";
    case SDLK_KP_MEMADD:
      return "kpmemadd";
    case SDLK_KP_MEMSUBTRACT:
      return "kpmemsubtract";
    case SDLK_KP_MEMMULTIPLY:
      return "kpmemmultiply";
    case SDLK_KP_MEMDIVIDE:
      return "kpmemdivide";
    case SDLK_KP_PLUSMINUS:
      return "kpplusminus";
    case SDLK_KP_CLEAR:
      return "kpclear";
    case SDLK_KP_CLEARENTRY:
      return "kpclearentry";
    case SDLK_KP_BINARY:
      return "kpbinary";
    case SDLK_KP_OCTAL:
      return "kpoctal";
    case SDLK_KP_DECIMAL:
      return "kpdecimal";
    case SDLK_KP_HEXADECIMAL:
      return "kphexadecimal";
    case SDLK_EXCLAIM:
    case SDLK_QUOTEDBL:
    case SDLK_HASH:
    case SDLK_PERCENT:
    case SDLK_DOLLAR:
    case SDLK_AMPERSAND:
    case SDLK_QUOTE:
    case SDLK_LEFTPAREN:
    case SDLK_RIGHTPAREN:
    case SDLK_ASTERISK:
    case SDLK_PLUS:
    case SDLK_COMMA:
    case SDLK_MINUS:
    case SDLK_PERIOD:
    case SDLK_SLASH:
    case SDLK_0:
    case SDLK_1:
    case SDLK_2:
    case SDLK_3:
    case SDLK_4:
    case SDLK_5:
    case SDLK_6:
    case SDLK_7:
    case SDLK_8:
    case SDLK_9:
    case SDLK_COLON:
    case SDLK_SEMICOLON:
    case SDLK_LESS:
    case SDLK_EQUALS:
    case SDLK_GREATER:
    case SDLK_QUESTION:
    case SDLK_AT:
    case SDLK_LEFTBRACKET:
    case SDLK_BACKSLASH:
    case SDLK_RIGHTBRACKET:
    case SDLK_CARET:
    case SDLK_UNDERSCORE:
    case SDLK_BACKQUOTE:
    case SDLK_a:
    case SDLK_b:
    case SDLK_c:
    case SDLK_d:
    case SDLK_e:
    case SDLK_f:
    case SDLK_g:
    case SDLK_h:
    case SDLK_i:
    case SDLK_j:
    case SDLK_k:
    case SDLK_l:
    case SDLK_m:
    case SDLK_n:
    case SDLK_o:
    case SDLK_p:
    case SDLK_q:
    case SDLK_r:
    case SDLK_s:
    case SDLK_t:
    case SDLK_u:
    case SDLK_v:
    case SDLK_w:
    case SDLK_x:
    case SDLK_y:
    case SDLK_z:
      res[0] = static_cast<char>(keycode);
      return std::string(res);
  }
}

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

namespace is_glm_struct_impl {
template <typename T>
struct is_glm_struct : std::false_type {};
template <glm::length_t L, typename T>
struct is_glm_struct<glm::vec<L, T>> : std::true_type {};
}  // namespace is_glm_struct_impl
template <typename T>
struct is_glm_struct {
  static constexpr bool const value =
      is_glm_struct_impl::is_glm_struct<std::decay_t<T>>::value;
};

template <typename T>
struct glm_struct_dim;

template <glm::length_t N, typename T>
struct glm_struct_dim<glm::vec<N, T>> {
  static constexpr glm::length_t const value = N;
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
template <typename T, std::enable_if_t<!is_stl_container<T>::value &&
                                       !is_glm_struct<T>::value> * = nullptr>
static T pop(lua_State * L, int idx) {
  DEBUG_RUNTIME_ASSERT_TRUE(lua_is<T>(L, idx) != 0);
  return lua_to<T>(L, idx);
}

// Container types
template <typename T, typename VT, typename>
static T pop_container(lua_State * L, int idx);

// Bool
template <>
void push(lua_State * L, bool obj) {
  lua_pushboolean(L, obj);
}

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

// 32 Bit Signed Int
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

// glm::vecN
template <glm::length_t N, typename T>
void push(lua_State * L, glm::vec<N, T> obj) {
  lua_createtable(L, N, 0);
  for (auto i = 0; i < N; ++i) {
    push(L, obj[i]);
    lua_rawseti(L, -2, i + 1);
  }
}

template <typename T, glm::length_t N, typename VT>
T pop_glm(lua_State * L, int idx) {
  auto isTable = static_cast<bool>(lua_istable(L, idx));
  if (!isTable)
    return T();
  if (static_cast<size_t>(luaL_len(L, idx)) != N)
    return T();
  bool allValid = true;
  T ret;
  for (auto i = 0; i < N; ++i) {
    lua_rawgeti(L, idx, i + 1);
    allValid &= lua_is<float>(L, -1);
    ret[i] = lua_to<VT>(L, -1);
    lua_pop(L, 1);
    if (!allValid)
      return T();
  }
  return ret;
}

// GLM structs
template <typename T, std::enable_if_t<is_glm_struct<T>::value> * = nullptr>
static T pop(lua_State * L, int idx) {
  typedef typename T::value_type VT;
  return pop_glm<T, glm_struct_dim<T>::value, VT>(L, idx);
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

// Closure

template <>
bool lua_is<std::function<void(std::string)>>(lua_State * L, int idx) {
  auto res = static_cast<bool>(lua_isfunction(L, idx));
  return res;
}

template <>
bool lua_is<std::function<void(int, int)>>(lua_State * L, int idx) {
  auto res = static_cast<bool>(lua_isfunction(L, idx));
  return res;
}

template <>
bool lua_is<std::function<void(int, int, int)>>(lua_State * L, int idx) {
  auto res = static_cast<bool>(lua_isfunction(L, idx));
  return res;
}

template <typename... Ts>
static int push_args(lua_State * L, Ts... args) {
  (push(L, args), ...);
  return sizeof...(args);
}

template <class... As>
std::function<void(As...)> pop_void_closure(lua_State * L, std::string name) {
  int func = lua_getglobal(L, name.c_str());
  if (func == 0 || !lua_is<std::function<void(As...)>>(L, -1)) {
    lua_remove(L, -1);
    return nullptr;
  }
  lua_remove(L, -1);
  return std::function<void(As...)>([L, name](As... args) {
    int func = lua_getglobal(L, name.c_str());
    if (func) {
      auto level = push_args(L, args...);
      lua_call(L, level, 0);
    } else {
      lua_remove(L, -1);
    }
  });
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
