#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include "function.h"
#include "visual.h"
#include <stdbool.h>

//---------------------------------------------------------------------------------------------
const double EPSILON          = 1e-3;     // == 0.001
const int    BUFFER_SIZE      = 10;
const char*  EXIT_CYCLE       = "stop";
const char*  CONTINUE_CYCLE   = "go";
const char*  STATISTIC        = "stat";
const char*  RESTART_STAT     = "restart";
const char*  EXIT_STAT        = "exit";
const char*  TESTING          = "test";
const char*  PASSWORD         = "1111";
const char*  FILE_TEST        = "file";
const char*  DEFAULT_TEST     = "default";
//---------------------------------------------------------------------------------------------

int main(void)
{
    // TODO: unused variables
    double a = NAN, b = NAN, c = NAN;
    double x1 = NAN, x2 = NAN;

    int count_solutions = 0, count_bad_enter = 0, flag_stat = 1, flag_exit = 1;
    int flag_stop_program = 0;
    enum enum_decisions quantity = NO_ROOTS;
    enum enum_opportunity_user choice = GO;

    struct coefficients coeffs = {.a = NAN, .b = NAN, .c = NAN};

    while (1)
    {
        // TODO: ask_user move to switch (...)
        choice = ask_user(&flag_stat, &flag_exit, &count_bad_enter);
        switch (choice)
        {
            case STOP:
                flag_stop_program = 1;
                [[fallthrough]];
            case STAT:
                statistic(&count_solutions, &count_bad_enter, &flag_exit);
                break;
            case GO:
                input_coeffs(&coeffs, &count_bad_enter);
                decide_equation(coeffs, &x1, &x2, &quantity);
                print_answer(quantity, &x1, &x2, &count_solutions);
                break;
            case CONTINUE:
                break;
            case TEST:
                if (check_test_password(&count_bad_enter))
                    chose_test();
                break;
        }
        if (flag_stop_program)
            break;
    }
    // TODO: return 0
    return 1;
}

//---------------------------------------------------------------------------------------------
bool is_zero(double x) //проверка double на нулевое значение
{
    if (fabs(x) < EPSILON)
        return true;
    else return false;
}
//---------------------------------------------------------------------------------------------
void clean_stdin() //очистка ввода
{
    int c = 0;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}
//---------------------------------------------------------------------------------------------
// ф-ия решение квадратного уравнения
enum enum_decisions decide_square(coefficients coeffs, double* x1_ptr, double* x2_ptr)
{
    assert (x1_ptr != NULL);
    assert (x2_ptr != NULL);
    assert(x1_ptr != x2_ptr);
    assert (!is_zero(coeffs.a));

    // double d = calc_discr(coeff);
    double d = coeffs.b * coeffs.b - 4 * coeffs.a * coeffs.c;

    // TODO: codestyle
    if (is_zero(d))
    {
        *x1_ptr = -coeffs.b / (2 * coeffs.a);
        return ONE_ROOT;
    }
    else if (d < 0)
        return NO_ROOTS;
    else
    {
        // TODO: codestyle
        *x1_ptr = (-coeffs.b + sqrt(d)) / (2 * coeffs.a);
        *x2_ptr = (- coeffs.b - sqrt(d)) / (2 * coeffs.a);
        return TWO_ROOTS;
    }
}
//---------------------------------------------------------------------------------------------
enum enum_decisions decide_line(coefficients coeffs, double* ptr_x1) //линейное уравнение
{
    assert (ptr_x1 != NULL);

    if (is_zero(coeffs.b))
    {
        if (is_zero(coeffs.c))
            return INFINITE;
        else
            return NO_ROOTS;
    }

    *ptr_x1 = (-coeffs.c) / coeffs.b;
    return ONE_ROOT;
}
//---------------------------------------------------------------------------------------------
void decide_equation(coefficients coeffs, double* ptr_x1,
                        double* ptr_x2, enum enum_decisions* ptr_quantity)
{
    assert (ptr_x1 != NULL);
    assert (ptr_x2 != NULL);
    assert (ptr_quantity != NULL);

    if (is_zero(coeffs.a))
        *ptr_quantity = decide_line(coeffs, ptr_x1);
    else
        *ptr_quantity = decide_square(coeffs, ptr_x1, ptr_x2);
}
//---------------------------------------------------------------------------------------------

void input_coeffs(coefficients* ptr_coeffs, int* ptr_count_bad_enter)
{
    assert (ptr_coeffs != NULL);
    assert (ptr_count_bad_enter != NULL);

    printf("Enter a\n");
    input_one_coeff(&((*ptr_coeffs).a), ptr_count_bad_enter);

    printf("Enter b\n");
    input_one_coeff(&((*ptr_coeffs).b), ptr_count_bad_enter);

    printf("Enter c\n");
    input_one_coeff(&((*ptr_coeffs).c), ptr_count_bad_enter);

    printf(BLU "Your coeffitients: %lg %lg %lg\n" CRESET,
                (*ptr_coeffs).a, (*ptr_coeffs).b, (*ptr_coeffs).c);

    sleep(1);
}
//---------------------------------------------------------------------------------------------

void input_one_coeff(double* ptr_1coeff, int* ptr_count_bad_enter)
{
    assert (ptr_1coeff != NULL);
    assert (ptr_count_bad_enter != NULL);

    int right_coeff = 0;

    while (1)
    {
        right_coeff = scanf("%lg", ptr_1coeff);
        clean_stdin();

        if (!right_coeff)
        {
            printf(RED "Wrong enter\n" CRESET);
            ++(*ptr_count_bad_enter);
            continue;
        }
        else
            break;
    }
}
//---------------------------------------------------------------------------------------------

void print_answer(enum_decisions quantity, double* ptr_x1, double* ptr_x2, int* ptr_count_solutions)
{
    assert (ptr_x1 != NULL);
    assert (ptr_x2 != NULL);
    assert (ptr_count_solutions != NULL);

    sleep(1);

    switch (quantity)
    {
        case NO_ROOTS:
            printf("Нет решений\n\n");
            break;
        case ONE_ROOT:
            printf("x1 = x2 = %lg\n\n", *ptr_x1);
            break;
        case TWO_ROOTS:
            printf("x1 = %lg, x2 = %lg\n\n", *ptr_x1, *ptr_x2);
            break;
        case INFINITE:
            printf("0 = 0; infinite roots\n\n");
            break;
        default:
            assert(0 && "Unreachable"); // custom assert
            break;
    }

    ++(*ptr_count_solutions);
}
//---------------------------------------------------------------------------------------------

// TODO: rename calculate_statics
void statistic(int* ptr_count_solutions, int* ptr_count_bad_enter, int* ptr_flag_exit)
{
    assert (ptr_count_bad_enter != NULL);
    assert (ptr_flag_exit != NULL);
    assert (ptr_count_solutions != NULL);

    char mode_stat[BUFFER_SIZE] = {};

    sleep(1);

    printf("Statistic:\n");
    printf(YEL "Решено уравнений: %d\n" CRESET, *ptr_count_solutions);
    printf(RED "Wrong enter: %d\n\n" CRESET, *ptr_count_bad_enter);

    if (*ptr_flag_exit)
    {
        printf("restart -> restart statistic\nexit -> continue calculations\n");
        scanf(" %7s", mode_stat);      //7 длина читаемой строки
    }

    while (*ptr_flag_exit)
    {
        if (!(strcmp(mode_stat, RESTART_STAT)))
        {
            *ptr_count_solutions = 0;
            *ptr_count_bad_enter = 0;
            printf("Вычисления обнулены\n");
            break;
        }
        else if (!(strcmp(mode_stat, EXIT_STAT)))
            break;
        else
        {
            printf(RED "Wrong enter\n" CRESET);
            clean_stdin();
            (*ptr_count_bad_enter)++;
        }

        scanf(" %7s", mode_stat);
    }
}
//---------------------------------------------------------------------------------------------
enum enum_opportunity_user ask_user(int* ptr_flag_stat, int* ptr_flag_exit, int* ptr_count_bad_enter) //запрос юзера
{
    assert (ptr_flag_stat != NULL);
    assert (ptr_flag_exit != NULL);
    assert (ptr_count_bad_enter != NULL);

    // TODO: codestyle
    char mode[BUFFER_SIZE] = {};

    if (*ptr_flag_stat)
        printf(BLU "go -> continue; stop -> stop; stat -> statistic; test -> testing\n" CRESET);
    else
        printf(BLU "go -> continue; stop -> stop; test -> testing\n" CRESET);

    scanf(" %5s", mode);       //5 - длина считываемой строки

    if (!strcmp(CONTINUE_CYCLE, mode))      //продолжение программы
    {
        *ptr_flag_stat = 1;
        clean_stdin();
        return GO;
    }
    else if (!strcmp(EXIT_CYCLE, mode))     //завершение программы
    {
        printf("The end(\n");
        *ptr_flag_exit = 0;
        return STOP;
    }
    else if (!strcmp(STATISTIC, mode) && *ptr_flag_stat)     //открытие статистики
    {
        *ptr_flag_stat = 0;
        return STAT;
    }
    else if (!strcmp(TESTING, mode))          //начало тестирования
    {
        *ptr_flag_stat = 1;
        return TEST;
    }
    else
    {
        printf(RED "Wrong enter\n\n" CRESET);
        clean_stdin();
        (*ptr_count_bad_enter)++;

        return CONTINUE;
    }
}
//---------------------------------------------------------------------------------------------
bool is_equal(double x1, double x2) //равенство double
{
    if (isnan(x1) && isnan(x2))
        return true;
    else if (fabs(x1 - x2) < EPSILON)
        return true;
    else return false;
}

//---------------------------------------------------------------------------------------------
bool check_test_password(int* ptr_count_bad_enter) //check password before testing
{
    assert (ptr_count_bad_enter != NULL);

    char password[BUFFER_SIZE] = {};
    printf(BLU "Please, enter the password\n" CRESET);
    scanf(" %5s", password);
    if (!strcmp(password, PASSWORD))
    {
        printf(GRN "Right password\n\n" CRESET);
        return true;
    }
    else
    {
        printf(RED "Wrong password, GoodBye\n" CRESET);
        ++(*ptr_count_bad_enter);
        return false;
    }
}
//---------------------------------------------------------------------------------------------
int chose_test()
{
    char test_chose[BUFFER_SIZE] = {};

    printf(BLU "Choose variety of testing\n" CRESET);
    printf("file -> testing from user file\n");
    printf("default -> testing by default values\n");
    scanf(" %8s", test_chose);

    if (!strcmp(test_chose, FILE_TEST))
    {
        // TODO: read filename
        printf(BLU "Good, testing from user file\n" CRESET);
        sleep(1);
        RunTests_from_file();
        return 1;
    }
    else if (!strcmp(test_chose, DEFAULT_TEST))
    {
        printf(BLU "Ok, default testing\n" CRESET);
        sleep(1);   // TODO: is sleep really needed?
        RunTests();
        return 1;
    }
    else
    {
        printf(RED "Wrong enter, bye\n" CRESET);
        return 0;
    }
}
