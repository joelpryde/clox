//
// Created by joelpryde on 2/17/2022.
//

#include "utest.h"
#include "vm.h"

extern FILE* printOutput;

UTEST_STATE();

bool checkPrintOutput(const char* checkStr)
{
    const int printOutput_length = ftell(printOutput) + 1;
    rewind(printOutput);
    char buffer[printOutput_length];
    fgets(buffer, printOutput_length, printOutput);
    rewind(printOutput);

    return (strcmp(buffer, checkStr) == 0);
}

UTEST(interpret, print) {
    InterpretResult result = interpret("print 1;");
    ASSERT_TRUE(result == INTERPRET_OK);
    ASSERT_TRUE(checkPrintOutput("1"));
}