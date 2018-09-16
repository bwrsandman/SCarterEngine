//
// Created by sandy on 11/05/18.
//

#ifndef SCARTERENGINE_SCRIPTING_PRIVATE_SCRIPTING_MANAGER_H
#define SCARTERENGINE_SCRIPTING_PRIVATE_SCRIPTING_MANAGER_H

#include <ctime>
#include <lua.hpp>
#include <string>
#include "../Manager.h"

namespace sce::scripting::private_ {

class ScriptingManager : public sce::private_::Manager {
  bool initialized_;
  bool configured_;
  lua_State * lua_state_;
  bool scriptIsFile_;
  std::time_t scriptLastModified_;
  std::string scriptSource_;

  void Check(int load_result);
  /// Will leave function on stack if result is true
  bool GetAndCheckFunction(std::string name);
  void RunScriptInit();
  void RunScriptTerminate();
  bool ScriptModified();
  int ReloadScript();

 public:
  void Initialize(bool reload) override;
  void Terminate(bool reload) override;
  int LoadSource(std::string source);
  int LoadFile(std::string file);
  void RunFrame(double dt);
};

}  // namespace sce::scripting::private_

#endif  // SCARTERENGINE_SCRIPTING_PRIVATE_SCRIPTING_MANAGER_H
