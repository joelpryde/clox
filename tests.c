//
// Created by joelpryde on 2/17/2022.
//

#include "utest.h"
#include "vm.h"

extern char testPrintOutput[512];

UTEST_STATE();

bool checkPrintOutput(const char* checkStr)
{
    return (strncmp(testPrintOutput, checkStr, strlen(checkStr)) == 0);
}

UTEST(interpret, print) {
    InterpretResult result = interpret("print 1;");
    ASSERT_TRUE(result == INTERPRET_OK);
    ASSERT_TRUE(checkPrintOutput("1"));
}

UTEST(interpret, while_) {
    InterpretResult result = interpret("var a=0; while (a<3) a = a + 1; print a;");
    ASSERT_TRUE(result == INTERPRET_OK);
    ASSERT_TRUE(checkPrintOutput("3"));
}

UTEST(interpret, for_) {
    InterpretResult result = interpret("var a=0; for (var t = 3; t > 0; t = t - 1) a = a + 1; print a;");
    ASSERT_TRUE(result == INTERPRET_OK);
    ASSERT_TRUE(checkPrintOutput("3"));
}