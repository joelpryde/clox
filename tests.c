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

struct TestFixture {
    const char* expected;
    InterpretResult result;
};

UTEST_F_SETUP(TestFixture) {
    reset();
}

UTEST_F_TEARDOWN(TestFixture) {
    ASSERT_TRUE(utest_fixture->result == INTERPRET_OK);
    ASSERT_TRUE(strncmp(testPrintOutput, utest_fixture->expected, strlen(utest_fixture->expected)) == 0);
}

UTEST_F(TestFixture, interpret_print) {
    utest_fixture->result = interpret("print 1;");
    utest_fixture->expected = "1";
}

UTEST_F(TestFixture, interpret_concat_strings) {
    utest_fixture->result = interpret("print \"jaba\" + \"hut\";");
    utest_fixture->expected = "jabahut";
}

UTEST_F(TestFixture, interpret_while_) {
    utest_fixture->result = interpret("var a=0; while (a<3) a = a + 1; print a;");
    utest_fixture->expected = "3";
}

UTEST_F(TestFixture, interpret_for_) {
    utest_fixture->result = interpret("var a=0; for (var t = 3; t > 0; t = t - 1) a = a + 1; print a;");
    utest_fixture->expected = "3";
}

UTEST_F(TestFixture, interpret_fun_declaration) {
    utest_fixture->result = interpret("fun someFunction() { print \"blah\"; } print someFunction;");
    utest_fixture->expected = "<fn someFunction>";
}

UTEST_F(TestFixture, interpret_fun_call) {
    utest_fixture->result = interpret("fun someFunction() { print \"blah\"; } someFunction();");
    utest_fixture->expected = "blah";
}

UTEST_F(TestFixture, interpret_fun_return) {
    utest_fixture->result = interpret("fun someFunction() { return \"blah\"; } print someFunction();");
    utest_fixture->expected = "blah";
}

UTEST_F(TestFixture, interpret_native_clock) {
    utest_fixture->result = interpret("print clock();");
    utest_fixture->expected = "0";
}

UTEST_F(TestFixture, interpret_upvalues) {
    utest_fixture->result = interpret(
"fun outer() {"
    "var x = \"outside\";"
    "fun inner() {"
        "print x;"
    "}"
    "inner();"
"}"
"outer();");
    utest_fixture->expected = "outside";
}

UTEST_F(TestFixture, interpret_closed_upvalues) {
    utest_fixture->result = interpret(
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
    utest_fixture->expected = "param";
}