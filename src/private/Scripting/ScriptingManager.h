//
// Created by sandy on 11/05/18.
//

#ifndef SCARTERENGINE_SCRIPTING_PRIVATE_SCRIPTING_MANAGER_H
#define SCARTERENGINE_SCRIPTING_PRIVATE_SCRIPTING_MANAGER_H

#include <lua.hpp>
#include <string>
#include "../Manager.h"

namespace sce::scripting::private_ {

class ScriptingManager : public sce::private_::Manager {
  bool initialized_;
  lua_State * lua_state_;

 public:
  void Initialize() override;
  void Terminate() override;
  int RunSource(std::string source);
  int RunFile(std::string file);
};

}  // namespace sce::scripting::private_

#endif  // SCARTERENGINE_SCRIPTING_PRIVATE_SCRIPTING_MANAGER_H
