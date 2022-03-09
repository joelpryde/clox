//
// Created by joelpryde on 1/31/2022.
//

#ifndef CLOX_COMPILER_H
#define CLOX_COMPILER_H

#include "object.h"
#include "vm.h"

void resetCompilerForTests();
ObjFunction* compile(const char* source);
void markCompilerRoots();

#endif //CLOX_COMPILER_H
