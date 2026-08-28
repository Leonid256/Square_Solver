#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include "function.hpp"
#include "visual.hpp"
#include <stdbool.h>
#include <unistd.h>

//---------------------------------------------------------------------------------------------
const double EPSILON          = 1e-3;     // == 0.001
const int    BUFFER_SIZE      = 10;
const char*  EXIT_CYCLE       = "stop";
const char*  CONTINUE_CYCLE   = "go";
const char*  STATISTIC        = "stat";
const char*  RESTART_STAT     = "restart";
const char*  EXIT_STAT        = "exit";
const char*  TESTING          = "test";
const char*  PASSWORD         = "5267";
const char*  FILE_TEST        = "file";
const char*  DEFAULT_TEST     = "default";
const char*  DEFAULT_ENTER    = "def";
const char*  CUSTOM_ENTER     = "cus";
const int    DEBUG_MODE       = 0;
//---------------------------------------------------------------------------------------------
#define sleep_ms(ms) usleep((ms) * 1000)
//---------------------------------------------------------------------------------------------

int main(void)
{
    double x1 = NAN, x2 = NAN;

    int count_solutions = 0, count_bad_enter = 0, flag_stat = 1, flag_exit = 1;
    int flag_stop_program = 0;

    enum_decisions quantity = NO_ROOTS;
    coefficients coeffs = {.a = NAN, .b = NAN, .c = NAN};

    if (!DEBUG_MODE)
    {
        print_logo();
        print_cat();
    }

    while (1)
    {
        switch (ask_user(&flag_stat, &flag_exit, &count_bad_enter))
        {
            case STOP:
                flag_stop_program = 1;
                [[fallthrough]];
            case STAT:
                calculate_statics(&count_solutions, &count_bad_enter, &flag_exit);
                break;
            case GO:
                choose_enter(&coeffs, &count_bad_enter);
                decide_equation(coeffs, &x1, &x2, &quantity);
                print_answer(coeffs, quantity, &x1, &x2, &count_solutions);
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
    return 0;
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

    if (is_zero(d))
    {
        *x1_ptr = -coeffs.b / (2 * coeffs.a);
        return ONE_ROOT;
    }
    else if (d < 0)
        return NO_ROOTS;
    else
    {
        *x1_ptr = (-coeffs.b + sqrt(d)) / (2 * coeffs.a);
        *x2_ptr = (-coeffs.b - sqrt(d)) / (2 * coeffs.a);
        //square_function_graphic(coeffs);
        return TWO_ROOTS;
    }
}
//---------------------------------------------------------------------------------------------
// ф-ия решение линейного уравнения
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

    print_typewriter("Enter a\n", 15);
    input_one_coeff(&((*ptr_coeffs).a), ptr_count_bad_enter);

    print_typewriter("Enter b\n", 15);
    input_one_coeff(&((*ptr_coeffs).b), ptr_count_bad_enter);

    print_typewriter("Enter c\n", 15);
    input_one_coeff(&((*ptr_coeffs).c), ptr_count_bad_enter);

    printf(BLU);
    print_typewriter("Your coeffitients: ", 15);
    printf("a = " CRESET);
    printf("%lg", (*ptr_coeffs).a);
    printf(BLU "; b = " CRESET);
    printf("%lg", (*ptr_coeffs).b);
    printf(BLU "; c = " CRESET);
    printf("%lg", (*ptr_coeffs).c);
    printf(BLU ";\n" CRESET);
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
            printf(RED);
            print_typewriter("Wrong enter\n\n", 15);
            printf(CRESET);

            ++(*ptr_count_bad_enter);
            continue;
        }
        else
            break;
    }
}
//---------------------------------------------------------------------------------------------

void print_answer(coefficients coeffs, enum_decisions quantity, double* ptr_x1, double* ptr_x2, int* ptr_count_solutions)
{
    assert (ptr_x1 != NULL);
    assert (ptr_x2 != NULL);
    assert (ptr_count_solutions != NULL);

    sleep(1);

    switch (quantity)
    {
        case NO_ROOTS:
            print_typewriter("Уравнение не имеет решений\n\n", 15);
            break;
        case ONE_ROOT:
            print_typewriter("Уравнение имеет одно решение:\n", 15);
            printf("x1 = x2 = %lg\n\n", *ptr_x1);

            square_function_graphic(coeffs, *ptr_x1, *ptr_x2);

            break;
        case TWO_ROOTS:
            print_typewriter("Уравнение имеет два решения:\n", 15);
            print_typewriter("x1 = ", 10);
            printf("%lg\n", *ptr_x1);
            print_typewriter("x2 = ", 10);
            printf("%lg\n\n",*ptr_x2);

            square_function_graphic(coeffs, *ptr_x1, *ptr_x2);

            break;
        case INFINITE:
            print_typewriter("Уравнение имеет вид: 0 = 0\n", 15);
            print_typewriter("infinite roots\n\n", 15);
            break;
        default:
            assert(0 && "Unreachable"); // custom assert
            break;
    }

    ++(*ptr_count_solutions);
}
//---------------------------------------------------------------------------------------------

void calculate_statics(int* ptr_count_solutions, int* ptr_count_bad_enter, int* ptr_flag_exit)
{
    assert (ptr_count_bad_enter != NULL);
    assert (ptr_flag_exit != NULL);
    assert (ptr_count_solutions != NULL);

    char mode_stat[BUFFER_SIZE] = {};

    printf("-------------------------|\n");
    print_typewriter("Statistic:               |\n", 25);

    printf(YEL);
    print_typewriter("Решено уравнений: ", 20);
    printf("%d", *ptr_count_solutions);
    printf(CRESET);
    printf("      |\n");

    printf(RED);
    print_typewriter("Wrong enter: ", 20);
    printf("%d", *ptr_count_bad_enter);
    printf(CRESET);
    printf("           |\n");
    printf("-------------------------|\n\n");

    if (*ptr_flag_exit)
    {
        print_typewriter("restart -> restart statistic\nexit -> continue calculations\n", 30);
        scanf(" %7s", mode_stat);      //7 длина читаемой строки
    }

    while (*ptr_flag_exit)
    {
        if (!(strcmp(mode_stat, RESTART_STAT)))
        {
            *ptr_count_solutions = 0;
            *ptr_count_bad_enter = 0;
            print_typewriter("Вычисления обнулены\n\n", 20);
            break;
        }
        else if (!(strcmp(mode_stat, EXIT_STAT)))
            break;
        else
        {
            printf(RED);
            print_typewriter("Wrong enter\n", 10);
            printf(CRESET);

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

    char mode[BUFFER_SIZE] = {};

    print_typewriter("Выберите дальнейшее действие\n", 10);

    if (*ptr_flag_stat)
    {
        printf(BLU);

        if (!DEBUG_MODE)
            print_typewriter("go -> continue solving; stop -> stop; stat -> statistic; test -> testing\n", 10);

        printf(CRESET);
    }
    else
    {
        printf(BLU);

        if (!DEBUG_MODE)
            print_typewriter("go -> continue solving; stop -> stop; test -> testing\n", 10);

        printf(CRESET);
    }

    scanf(" %5s", mode);       //5 - длина считываемой строки

    if (!strcmp(CONTINUE_CYCLE, mode))      //продолжение программы
    {
        *ptr_flag_stat = 1;
        clean_stdin();
        return GO;
    }
    else if (!strcmp(EXIT_CYCLE, mode))     //завершение программы
    {
        print_typewriter("The end(\n", 15);

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
        printf(RED);
        print_typewriter("Wrong enter\n\n", 15);
        printf(CRESET);

        clean_stdin();
        (*ptr_count_bad_enter)++;

        return CONTINUE;
    }
}
//---------------------------------------------------------------------------------------------
/**
     \brief Функция сравнения двух чисел double
     \details Функция сравнивает два числа с плавающей точкой, учитывая введенную заранее погрешность
     \param [in] x1,
     \param [in] x2 сравниваемые числа
     \return "true" если числа равны с учетом погрешности
     \return "false" в противном случае
*/
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

    printf(BLU);
    print_typewriter("Please, enter the password\n", 20);
    printf(CRESET);

    scanf(" %5s", password);

    if (!strcmp(password, PASSWORD))
    {
        printf(GRN);
        print_typewriter("Right password\n\n", 15);
        printf(CRESET);

        return true;
    }
    else
    {
        printf(RED);
        print_typewriter("Wrong password, GoodBye\n", 15);
        printf(CRESET);

        ++(*ptr_count_bad_enter);
        return false;
    }
}
//---------------------------------------------------------------------------------------------
int chose_test()
{
    char test_chose[BUFFER_SIZE] = {};

    printf(BLU);
    print_typewriter("Choose variety of testing\n", 20);
    printf(CRESET);

    if(!DEBUG_MODE)
    {
        print_typewriter("file -> testing from user file\n", 20);
        print_typewriter("default -> testing by default values\n", 20);
    }

    scanf(" %8s", test_chose);

    if (!strcmp(test_chose, FILE_TEST))
    {
        // TODO: read filename
        printf(BLU);
        print_typewriter("Good, testing from user file\n", 20);
        printf(CRESET);

        RunTests_from_file();
        return 1;
    }
    else if (!strcmp(test_chose, DEFAULT_TEST))
    {
        printf(BLU);
        print_typewriter("Ok, default testing\n", 20);
        printf(CRESET);

        RunTests();
        return 1;
    }
    else
    {
        printf(RED);
        print_typewriter("Wrong enter, bye\n", 20);
        printf(CRESET);
        return 0;
    }
}
//------------------------------------------------------------------------------------------
int choose_enter(coefficients* ptr_coeffs, int* ptr_count_bad_enter)
{
    assert (ptr_coeffs != NULL);
    assert (ptr_count_bad_enter != NULL);

    char test_chose[BUFFER_SIZE] = {};

    print_typewriter("Choose enter format\n", 20);
    
    if (!DEBUG_MODE)
    {
        printf(MAG);
        print_typewriter("def -> default enter(by 1 coeff)\n", 15);
        print_typewriter("cus -> custom enter(full equalisation)\n", 15);
        printf(CRESET);
    }

    while (1)
    {
        scanf(" %8s", test_chose);      //8 - длина считываемой строки

        if (!strcmp(test_chose, DEFAULT_ENTER))
        {
            printf(BLU);

            if (!DEBUG_MODE)
                print_typewriter("Enter coefficients one by one\n", 25);

            printf(CRESET);

            input_coeffs(ptr_coeffs, ptr_count_bad_enter);
            return 0;
        }
        else if (!strcmp(test_chose, CUSTOM_ENTER))
        {
            custom_input(ptr_coeffs);
            return 0;
        }
        else
        {
            printf(RED);
            print_typewriter("Wrong enter\n\n", 15);
            printf(CRESET);

            clean_stdin();
            continue;
        }
    }

    return 1;
}
//----------------------------------------------------------------------------------------
void print_typewriter(const char *text, int delay_ms)
{
    while (*text) {
        putchar(*text);
        fflush(stdout);
        sleep_ms(delay_ms);
        text++;
    }
}
//----------------------------------------------------------------------------------------
void print_cat(void)
{
    printf(WHT "\t\t|\\---/|\n" CRESET);
    printf(BLU "\t\t| o_o |\n" CRESET);
    printf(RED "\t\t \\_^_/ \n" CRESET);
}
//----------------------------------------------------------------------------------------
void print_logo(void)
{
    printf("|--------------------------------------|\n");
    printf("|" MAG "Hello, that is square solver programm" CRESET " |\n");
    printf("|--------------------------------------|\n");
}
