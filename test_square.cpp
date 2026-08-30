#include "function.hpp"
#include <stdio.h>
#include <math.h>
#include "visual.hpp"
#include <unistd.h>
#include <assert.h>

//---------------------------------------------------------------------------------------------
/**
    \brief Функция с запуском одного теста
    \details Данная функция проводит один тест функции решения уравнения. Вызывается другими функциями. В случае неуспешного теста 
    вызывает функцию печати этого, при успехе самостоятельно печатает это
    @decide_equation(), RunTests(), RunTests_from_file(), print_failed()
    \param [in, out] test струтура со всеми данными об уравнении
    \param [in, out] ptr_count_fail указатель на счетчик неуспешных тестов
    \param [in, out] ptr_count_success указатель на счетчик успешных тестов
    \return Функция ничего не возращает
*/
void RunOneTest(TestCase test, int* ptr_count_fail, int* ptr_count_success)
{
    assert (ptr_count_fail != NULL);
    assert (ptr_count_success != NULL);

    double x1 = NAN, x2 = NAN;
    enum_decisions quality_local = DECISION_NO_ROOTS;

    coefficients coeffs_test = {.a = NAN, .b = NAN, .c = NAN};

    coeffs_test.a = test.a;
    coeffs_test.b = test.b;
    coeffs_test.c = test.c;

    decide_equation(coeffs_test, &x1, &x2, &quality_local);

    if (quality_local == (enum enum_decisions)test.nRootsRef)
    {
        switch (quality_local)
        {
            case (DECISION_NO_ROOTS):
                if (isnan(x1) && isnan(x2))
                {
                    printf(GRN);
                    print_typewriter("Test PASSED\n", 15);
                    printf(CRESET);
                    (*ptr_count_success)++;
                }
                else
                    print_failed(test, x1,
                    x2, quality_local, ptr_count_fail);
                break;

            case (DECISION_ONE_ROOT):
            case (DECISION_INFINITE_ROOTS):
                if ((is_equal(x1, test.x1ref) && is_equal(x2, test.x2ref)))
                {
                    printf(GRN);
                    print_typewriter("Test PASSED\n", 15);
                    printf(CRESET);
                    (*ptr_count_success)++;
                }
                else
                    print_failed(test, x1,
                    x2, quality_local, ptr_count_fail);
                break;

            case (DECISION_TWO_ROOTS):
                if ((is_equal(x1, test.x1ref) && is_equal(x2, test.x2ref)) ||
                        (is_equal(x1, test.x2ref) && is_equal(x2, test.x1ref)))
                {
                    printf(GRN);
                    print_typewriter("Test PASSED\n", 15);
                    printf(CRESET);
                    (*ptr_count_success)++;
                }
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
/**
    \brief Печать неуспешного теста
    \details При неуспешного тестировании, вызывается данная функция, выводящая полученные и ожидаемые значения решения уравнения
    @RunOneTest()
    \param [in] test структура с ожидаемыми значениеми
    \param [in] x1, x2 полученные корни уравнения
    \param [in] quality_local полученное количество корней уравнения
    \param [in, out] ptr_count_fail указатель на счетчик неверных тестов
    \return Функция ничего не возвращает
*/
void print_failed(TestCase test, float x1, float x2, int quality_local, int* ptr_count_fail)
{
    assert (ptr_count_fail != NULL);

    printf(RED);
    print_typewriter("Test FAILED\n", 15);
    printf(CRESET);
    printf("a = %lg, b = %lg, c = %lg\n", test.a, test.b, test.c);
    printf("got:      x1 = %lg, x2 = %lg, roots: %d\n", x1, x2, quality_local);
    printf("expected: x1 = %lg, x2 = %lg, roots: %d\n" CRESET, test.x1ref, test.x2ref, test.nRootsRef);
    (*ptr_count_fail)++;
}
//---------------------------------------------------------------------------------------------
/**
    \brief Функция дефолтных тестов
    \details Данная функция имеет дефолтные значения для тестов. Она запускает функцию с одинарным тестом, передавая в нее эти значения
    @RunOneTest()
    \return count_fail количество неуспешных тестов
*/
int RunTests()
{
    sleep(1);

    // TODO: think about macro-function (особенно про # в теле макроса)

    int count_fail = 0;
    int count_success = 0;

    TestCase testsStreet[] =
    {{.a = 0, .b = 0, .c = 0, .nRootsRef = DECISION_INFINITE_ROOTS, .x1ref = NAN, .x2ref = NAN},
    {.a = 1, .b = 0, .c = 0, .nRootsRef = 1, .x1ref = 0, .x2ref = 1},    //error
    {.a = 0, .b = 1, .c = 0, .nRootsRef = 1, .x1ref = 0, .x2ref = NAN},
    {.a = 0, .b = 0, .c = 1, .nRootsRef = 0, .x1ref = NAN, .x2ref = NAN},
    {.a = 1, .b = 1, .c = 1, .nRootsRef = 0,.x1ref = NAN, .x2ref = NAN},
    {.a = 1, .b = -1, .c = 0, .nRootsRef = 2, .x1ref = 5267, .x2ref = 1}, //error
    {.a = 0, .b = 1, .c = 1, .nRootsRef = 1, .x1ref = -1, .x2ref = NAN},
    {.a = 1, .b = -59, .c = 864, .nRootsRef = 2, .x1ref = 32, .x2ref = 27},
    {.a = -2, .b = 72, .c = 74, .nRootsRef = 2, .x1ref = -1, .x2ref = 37}};

    int size = sizeof(testsStreet) / sizeof(TestCase);

    for (int i = 0; i < size; i++)
    {
        assert (i < size);

        RunOneTest(testsStreet[i], &count_fail, &count_success);
    }

    print_typewriter("Testing is over\n", 20);
    print_typewriter("Failed test: ", 20);
    printf("%d\n", count_fail);
    print_typewriter("Successful test: ", 20);
    printf("%d\n\n", count_success);

    return count_fail;
}
//---------------------------------------------------------------------------------------------
/**
    \brief Функция тестов из файла
    \details Данная функция открывает файл со значениями для тестов. Она запускает функцию 
    с одинарным тестом, передавая в нее эти значения
    @RunOneTest()
    \return count_fail количество неуспешных тестов
*/
int RunTests_from_file()
{
    FILE* fp = fopen("test_values.txt", "r");

    if (fp == NULL)
    {
        printf("Error, unsuccessful file opening\n");
        return -1;
    }

    int count_fail = 0;
    int count_success = 0;

    TestCase test_i = {.a = NAN, .b = NAN, .c = NAN, .nRootsRef = 0,
                .x1ref = NAN, .x2ref = NAN};

    while (fscanf(fp, " %lg%lg%lg%d%lg%lg", &test_i.a, &test_i.b, &test_i.c, &test_i.nRootsRef,
                    &test_i.x1ref, &test_i.x2ref) == 6)
    {
        //printf("%lg %lg %lg %d %lg %lg\n", test_i.a, test_i.b, test_i.c, test_i.nRootsRef,
                //test_i.x1ref, test_i.x2ref);
        RunOneTest(test_i, &count_fail, &count_success);
    }

    fclose(fp);

    print_typewriter("Testing is over\n", 20);
    print_typewriter("Failed test: ", 20);
    printf("%d\n", count_fail);
    print_typewriter("Successful test: ", 20);
    printf("%d\n\n", count_success);

    return count_fail;
}

