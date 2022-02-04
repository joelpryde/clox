//
// Created by joelpryde on 1/31/2022.
//

#ifndef CLOX_COMPILER_H
#define CLOX_COMPILER_H

#include "object.h"
#include "vm.h"

bool compile(const char* source, Chunk* chunk);

#endif //CLOX_COMPILER_H
