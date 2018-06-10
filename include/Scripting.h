//
// Created by sandy on 11/05/18.
//

#ifndef SCARTERENGINE_SCRIPTING_H
#define SCARTERENGINE_SCRIPTING_H

#include <string>

namespace sce::scripting {
void Initialize();
void Terminate();
int LoadSource(const std::string & source);
int LoadFile(const std::string & file);
void RunFrame(double dt);
}  // namespace sce::scripting

#endif  // SCARTERENGINE_SCRIPTING_H
