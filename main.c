#include "common.h"
#include "chunk.h"
#include "debug.h"

void disassemble(Chunk *ptr, const char *string);

int main(int argc, const char* argv[])
{
    Chunk chunk;
    initChunk(&chunk);

    int constant = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT);
    writeChunk(&chunk, constant);

    disassembleChunk(&chunk, "test chunk");

    freeChunk(&chunk);

    return 0;
}
