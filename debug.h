//
// Created by Joel Pryde on 1/26/22.
//

#ifndef CLOX_DEBUG_H
#define CLOX_DEBUG_H

#include "chunk.h"

void disassembleChunk(Chunk *ptr, const char *string);
int disassembleInstruction(Chunk *chunk, int offset);

#endif //CLOX_DEBUG_H
