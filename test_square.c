#include "function.h"
#include <stdio.h>
#include <math.h>
#include "visual.h"
#include <unistd.h>
#include <assert.h>
//---------------------------------------------------------------------------------------------
//const char* "test_values.txt"
//---------------------------------------------------------------------------------------------
void RunOneTest(TestCase test, int* ptr_count_fail)
{
    assert (ptr_count_fail != NULL);

    double x1 = NAN, x2 = NAN;
    // TODO: codestyle (enum keyword)
    enum enum_decisions quality_local = NO_ROOTS;

    // TODO: read about struct
    struct coefficients coeffs_test = {.a = NAN, .b = NAN, .c = NAN};

    coeffs_test.a = test.a;
    coeffs_test.b = test.b;
    coeffs_test.c = test.c;

    decide_equation(coeffs_test, &x1, &x2, &quality_local);

    if (quality_local == (enum enum_decisions)test.nRootsRef)
    {
        switch (quality_local)
        {
            case (NO_ROOTS):
                if (isnan(x1) && isnan(x2))
                    printf(GRN "Test PASSED\n" CRESET);
                else
                    print_failed(test, x1,
                    x2, quality_local, ptr_count_fail);
                break;

            case (ONE_ROOT):
            case (INFINITE):
                if ((is_equal(x1, test.x1ref) && is_equal(x2, test.x2ref)))
                    printf(GRN "Test PASSED\n" CRESET);
                else
                    print_failed(test, x1,
                    x2, quality_local, ptr_count_fail);
                break;

            case (TWO_ROOTS):
                if ((is_equal(x1, test.x1ref) && is_equal(x2, test.x2ref)) ||
                        (is_equal(x1, test.x2ref) && is_equal(x2, test.x1ref)))
                    printf(GRN "Test PASSED\n" CRESET);
                else
                    print_failed(test, x1,
                    x2, quality_local, ptr_count_fail);
                break;

            default:
                print_failed(test, x1,
                    x2, quality_local, ptr_count_fail);
                break;
        }
    }
}
//---------------------------------------------------------------------------------------------
void print_failed(TestCase test, float x1, float x2, int quality_local, int* ptr_count_fail)
{
    assert (ptr_count_fail != NULL);

    printf(RED "Test FAILED\n");
    printf("a = %lg, b = %lg, c = %lg\n", test.a, test.b, test.c);
    printf("got:      x1 = %lg, x2 = %lg, roots: %d\n", x1, x2, quality_local);
    printf("expected: x1 = %lg, x2 = %lg, roots: %d\n" CRESET, test.x1ref, test.x2ref, test.nRootsRef);
    ++(*ptr_count_fail);
}
//---------------------------------------------------------------------------------------------
int RunTests()
{
    sleep(1);

    // TODO: think about macro-function (особенно про # в теле макроса)

    int count_fail = 0;
    TestCase test1 = {.a = 0, .b = 0, .c = 0, .nRootsRef = INFINITE,
                .x1ref = NAN, .x2ref = NAN};
    RunOneTest(test1, &count_fail);

    TestCase test2 = {.a = 1, .b = 0, .c = 0, .nRootsRef = 1,
                .x1ref = 0, .x2ref = 1}; //error
    RunOneTest(test2, &count_fail);

    TestCase test3 = {.a = 0, .b = 1, .c = 0, .nRootsRef = 1,
                .x1ref = 0, .x2ref = NAN};
    RunOneTest(test3, &count_fail);

    TestCase test4 = {.a = 0, .b = 0, .c = 1, .nRootsRef = 0,
                .x1ref = NAN, .x2ref = NAN};
    RunOneTest(test4, &count_fail);

    TestCase test5 = {.a = 1, .b = 1, .c = 1, .nRootsRef = 0,
                .x1ref = NAN, .x2ref = NAN};
    RunOneTest(test5, &count_fail);

    TestCase test6 = {.a = 1, .b = -1, .c = 0, .nRootsRef = 2,
                .x1ref = 5267, .x2ref = 1}; //error
    RunOneTest(test6, &count_fail);

    TestCase test7 = {.a = 0, .b = 1, .c = 1, .nRootsRef = 1,
                .x1ref = -1, .x2ref = NAN};
    RunOneTest(test7, &count_fail);

    printf("Testing is over\n");
    printf("Failed test: %d\n\n", count_fail);

    return count_fail;
}
//---------------------------------------------------------------------------------------------
int RunTests_from_file()
{
    if (FILE* fp = fopen("test_values.txt", "r"))
    {
        int count_fail = 0;

        TestCase test_i = {.a = NAN, .b = NAN, .c = NAN, .nRootsRef = 0,
                    .x1ref = NAN, .x2ref = NAN};

        while (fscanf(fp, " %lg%lg%lg%d%lg%lg", &test_i.a, &test_i.b, &test_i.c, &test_i.nRootsRef,
                        &test_i.x1ref, &test_i.x2ref) == 6)
        {
            //printf("%lg %lg %lg %d %lg %lg\n", test_i.a, test_i.b, test_i.c, test_i.nRootsRef,
                    //test_i.x1ref, test_i.x2ref);
            RunOneTest(test_i, &count_fail);
        }

        fclose(fp);
        printf("Testing is over\n");
        printf("Failed test: %d\n\n", count_fail);

        return count_fail;
    }

    return -1;
}

