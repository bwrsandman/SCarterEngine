//
// Created by sandy on 11/05/18.
//

#include <Scripting.h>

#include "private/Scripting/ScriptingManager.h"

namespace sce::scripting {

static private_::ScriptingManager gScriptingManager;

void Initialize() {
  gScriptingManager.Initialize(false);
}

void Terminate() {
  gScriptingManager.Terminate(false);
}

int LoadSource(const std::string & source) {
  return gScriptingManager.LoadSource(source);
}

int LoadFile(const std::string & file) {
  return gScriptingManager.LoadFile(file);
}

void RunFrame(double dt) {
  return gScriptingManager.RunFrame(dt);
}

}  // namespace sce::scripting
