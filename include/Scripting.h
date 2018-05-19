//
// Created by sandy on 11/05/18.
//

#ifndef SCARTERENGINE_SCRIPTING_H
#define SCARTERENGINE_SCRIPTING_H

#include <string>

namespace sce::scripting {
void Initialize();
void Terminate();
int RunSource(const std::string & source);
int RunFile(const std::string & file);
}  // namespace sce::scripting

#endif  // SCARTERENGINE_SCRIPTING_H
