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

UTEST(interpret, fun_declaration) {
    InterpretResult result = interpret("fun someFunction() { print \"blah\"; } print someFunction;");
    ASSERT_TRUE(result == INTERPRET_OK);
    ASSERT_TRUE(checkPrintOutput("<fn someFunction>"));
}

UTEST(interpret, fun_call) {
    InterpretResult result = interpret("fun someFunction() { print \"blah\"; } someFunction();");
    ASSERT_TRUE(result == INTERPRET_OK);
    ASSERT_TRUE(checkPrintOutput("blah"));
}

UTEST(interpret, fun_return) {
    InterpretResult result = interpret("fun someFunction() { return \"blah\"; } print someFunction();");
    ASSERT_TRUE(result == INTERPRET_OK);
    ASSERT_TRUE(checkPrintOutput("blah"));
}

UTEST(interpret, native_clock) {
    InterpretResult result = interpret("print clock();");
    ASSERT_TRUE(result == INTERPRET_OK);
    ASSERT_TRUE(checkPrintOutput("0"));
}

UTEST(interpret, upvalues) {
    InterpretResult result = interpret(
"fun outer() {"
    "var x = \"outside\";"
    "fun inner() {"
        "print x;"
    "}"
    "inner();"
"}"
"outer();");
    ASSERT_TRUE(result == INTERPRET_OK);
    ASSERT_TRUE(checkPrintOutput("outside"));
}