//
// Created by joelpryde on 2/4/2022.
//

#ifndef CLOX_OBJECT_H
#define CLOX_OBJECT_H

#include "common.h"
#include "chunk.h"
#include "value.h"

#define OBJ_TYPE(value)     (AS_OBJ(value)->type)

#define IS_FUNCTION(value)  isObjType(value, OBJ_FUNCTION)
#define IS_STRING(value)    isObjType(value, OBJ_STRING)

#define AS_FUNCTION(value)  ((ObjFunction*)AS_OBJ(value))
#define AS_STRING(value)    ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)   (((ObjString*)AS_OBJ(value))->chars)

typedef enum
{
    OBJ_FUNCTION,
    OBJ_STRING
} ObjType;

struct Obj
{
    ObjType type;
    struct Obj* next;
};

typedef struct
{
    Obj obj;
    int arity;
    Chunk chunk;
    ObjString* name;
} ObjFunction;

struct ObjString
{
    Obj obj;
    int length;
    const char* chars;
    uint32_t hash;
};

ObjFunction* newFunction();
ObjString* takeString(const char* chars, int length);
ObjString* copyString(const char* chars, int length);
void printObject(bool forTest, Value value);

static inline bool isObjType(Value value, ObjType objType)
{
    return IS_OBJ(value) && AS_OBJ(value)->type == objType;
}

#endif //CLOX_OBJECT_H
