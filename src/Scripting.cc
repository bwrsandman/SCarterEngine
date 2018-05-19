//
// Created by sandy on 11/05/18.
//

#include <Scripting.h>

#include "private/Scripting/ScriptingManager.h"

namespace sce::scripting {

static private_::ScriptingManager gScriptingManager;

void Initialize() {
  gScriptingManager.Initialize();
}

void Terminate() {
  gScriptingManager.Terminate();
}

int RunSource(const std::string & source) {
  return gScriptingManager.RunSource(source);
}

int RunFile(const std::string & file) {
  return gScriptingManager.RunFile(file);
}

}  // namespace sce::scripting
