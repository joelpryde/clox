#//
// Created by joelpryde on 2/4/2022.
//

#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJ(type, objectType) \
    (type*)allocateObject(sizeof(type), objectType)

static Obj* allocateObject(size_t size, ObjType type)
{
    Obj* object = (Obj*)reallocate(NULL, 0, size);
    object->type = type;
    object->next = vm.objects;
    vm.objects = object;
    return object;
}

ObjClosure* newClosure(ObjFunction* function)
{
    ObjUpvalue** upvalues = ALLOCATE(ObjUpvalue*, function->upvalueCount);
    for (int i = 0; i < function->upvalueCount; i++)
        upvalues[i] = NULL;

    ObjClosure* closure = ALLOCATE_OBJ(ObjClosure, OBJ_CLOSURE);
    closure->function = function;
    closure->upvalues = upvalues;
    closure->upvalueCount = function->upvalueCount;
    return closure;
}

ObjFunction* newFunction()
{
    ObjFunction* function = ALLOCATE_OBJ(ObjFunction, OBJ_FUNCTION);
    function->arity = 0;
    function->upvalueCount = 0;
    function->name = NULL;
    initChunk(&function->chunk);
    return function;
}

ObjNative* newNative(NativeFn function)
{
    ObjNative* native = ALLOCATE_OBJ(ObjNative, OBJ_NATIVE);
    native->function = function;
    return native;
}

static ObjString* allocateString(const char* chars, int length, uint32_t hash)
{
    ObjString * string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->length = length;
    string->chars = chars;
    string->hash = hash;
    tableSet(&vm.strings, string, NIL_VAL());
    return string;
}

static uint32_t hashString(const char* chars, int length)
{
    uint32_t hash = 2166136261u;
    for (int i = 0; i < length; i++)
    {
        hash ^= (uint8_t)chars[i];
        hash *= 16777619;
    }
    return hash;
}

ObjString* takeString(const char* chars, int length)
{
    uint32_t hash = hashString(chars, length);
    ObjString* interned = tableFindInternedString(&vm.strings, chars, length, hash);
    if (interned != NULL)
    {
        FREE_ARRAY(char, (char*)chars, length+1);
        return interned;
    }

    return allocateString(chars, length, hash);
}

ObjString* copyString(const char* chars, int length)
{
    uint32_t hash = hashString(chars, length);
    ObjString* interned = tableFindInternedString(&vm.strings, chars, length, hash);
    if (interned != NULL)
        return interned;

    char* heapChars = ALLOCATE(char, length + 1);
    memcpy(heapChars, chars, length);
    heapChars[length] = '\0';
    return allocateString(heapChars, length, hash);
}

ObjUpvalue* newUpValue(Value* slot)
{
    ObjUpvalue* upvalue = ALLOCATE_OBJ(ObjUpvalue, OBJ_UPVALUE);
    upvalue->location = slot;
    return upvalue;
}

static void printFunction(bool forTest, ObjFunction* function)
{
    if (function->name == NULL)
        doPrint(forTest, "<script>");
    else
        doPrint(forTest, "<fn %s>", function->name->chars);
}

void printObject(bool forTest, Value value)
{
    switch (OBJ_TYPE(value))
    {
        case OBJ_CLOSURE:
            printFunction(forTest, AS_CLOSURE(value)->function);
            break;
        case OBJ_FUNCTION:
            printFunction(forTest, AS_FUNCTION(value));
            break;
        case OBJ_NATIVE:
            doPrint(forTest, "<native fn>");
            break;
        case OBJ_STRING:
            doPrint(forTest, "%s", AS_CSTRING(value));
            break;
        case OBJ_UPVALUE:
            doPrint(forTest, "upvalue");
            break;
    }
}
