//
// Created by joelpryde on 2/17/2022.
//

#include "utest.h"
#include "vm.h"
#include "compiler.h"

extern char testPrintOutput[512];

UTEST_STATE();

static bool setupRun = false;

static void reset()
{
    if (setupRun)
    {
        resetCompilerForTests();
        freeVM();
    }

    initVM();
    setupRun = true;
}

bool checkPrintOutput(const char* checkStr)
{
    return (strncmp(testPrintOutput, checkStr, strlen(checkStr)) == 0);
}

UTEST(interpret, print) {
    reset();
    InterpretResult result = interpret("print 1;");
    ASSERT_TRUE(result == INTERPRET_OK);
    ASSERT_TRUE(checkPrintOutput("1"));
}

UTEST(interpret, concat_strings) {
    reset();
    InterpretResult result = interpret("print \"jaba\" + \"hut\";");
    ASSERT_TRUE(result == INTERPRET_OK);
    ASSERT_TRUE(checkPrintOutput("jabahut"));
}

UTEST(interpret, while_) {
    reset();
    InterpretResult result = interpret("var a=0; while (a<3) a = a + 1; print a;");
    ASSERT_TRUE(result == INTERPRET_OK);
    ASSERT_TRUE(checkPrintOutput("3"));
}

UTEST(interpret, for_) {
    reset();
    InterpretResult result = interpret("var a=0; for (var t = 3; t > 0; t = t - 1) a = a + 1; print a;");
    ASSERT_TRUE(result == INTERPRET_OK);
    ASSERT_TRUE(checkPrintOutput("3"));
}

UTEST(interpret, fun_declaration) {
    reset();
    InterpretResult result = interpret("fun someFunction() { print \"blah\"; } print someFunction;");
    ASSERT_TRUE(result == INTERPRET_OK);
    ASSERT_TRUE(checkPrintOutput("<fn someFunction>"));
}

UTEST(interpret, fun_call) {
    reset();
    InterpretResult result = interpret("fun someFunction() { print \"blah\"; } someFunction();");
    ASSERT_TRUE(result == INTERPRET_OK);
    ASSERT_TRUE(checkPrintOutput("blah"));
}

UTEST(interpret, fun_return) {
    reset();
    InterpretResult result = interpret("fun someFunction() { return \"blah\"; } print someFunction();");
    ASSERT_TRUE(result == INTERPRET_OK);
    ASSERT_TRUE(checkPrintOutput("blah"));
}

UTEST(interpret, native_clock) {
    reset();
    InterpretResult result = interpret("print clock();");
    ASSERT_TRUE(result == INTERPRET_OK);
    ASSERT_TRUE(checkPrintOutput("0"));
}

UTEST(interpret, upvalues) {
    reset();
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

UTEST(interpret, closed_upvalues) {
    reset();
    InterpretResult result = interpret(
"var f;"
"fun foo(param) {"
    "fun f_() {"
        "print param;"
    "}"
    "f = f_;"
"}"
"foo(\"param\");"
"f();"
    );
    ASSERT_TRUE(result == INTERPRET_OK);
    ASSERT_TRUE(checkPrintOutput("param"));
}