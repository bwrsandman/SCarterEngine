//
// Created by sandy on 25/12/18.
//

#ifndef SCARTERENGINE_INPUT_H
#define SCARTERENGINE_INPUT_H

#include <cstdint>
#include <functional>
#include "enum.h"

namespace sce::input {

BETTER_ENUM(CallbackCategory, uint8_t, KeyDown, KeyUp);

void Initialize();
void Terminate();

void RunFrame(double dt);
void SetCallback(CallbackCategory category,
                 std::function<void(int32_t)> callback);
}  // namespace sce::input

#endif  // SCARTERENGINE_INPUT_H
