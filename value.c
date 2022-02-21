//
// Created by Joel Pryde on 1/26/22.
//

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "object.h"
#include "memory.h"
#include "value.h"

void initValueArray(ValueArray* array)
{
    array->values = NULL;
    array->capacity = 0;
    array-> count = 0;
}

void writeValueArray(ValueArray* array, Value value)
{
    if (array->capacity < array->count + 1)
    {
        int oldCapacity = array->capacity;
        array->capacity = GROW_CAPACITY(oldCapacity);
        array->values = GROW_ARRAY(Value, array->values, oldCapacity, array->capacity);
    }

    array->values[array->count] = value;
    array->count++;
}

void freeValueArray(ValueArray* array)
{
    FREE_ARRAY(Value, array->values, array->capacity);
    initValueArray(array);
}

extern bool useTestPrintOutput;
char testPrintOutput[512];

void doPrint(bool testPrint, const char* str, ...)
{
    va_list argList;
    va_start(argList, str);
    if (testPrint)
        vsprintf(testPrintOutput, str, argList);
    else
        vprintf(str, argList);
}

void printValue(Value value, bool forTests)
{
    bool testOutput = forTests && useTestPrintOutput;
    switch (value.type)
    {
        case VAL_BOOL:
            doPrint(testOutput, AS_BOOL(value) ? "true" : "false");
            break;
        case VAL_NIL:
            doPrint(testOutput, "nil");
            break;
        case VAL_NUMBER:
            doPrint(testOutput, "%g", AS_NUMBER(value));
            break;
        case VAL_OBJ:
            printObject(testOutput, value);
            break;
    }
}

bool valuesEqual(Value a, Value b)
{
    if (a.type != b.type)
        return false;
    switch (a.type)
    {
        case VAL_BOOL:      return AS_BOOL(a) == AS_BOOL(b);
        case VAL_NIL:       return true;
        case VAL_NUMBER:    return AS_NUMBER(a) == AS_NUMBER(b);
        case VAL_OBJ:       return AS_OBJ(a) == AS_OBJ(b);
        default: return false;
    }
}