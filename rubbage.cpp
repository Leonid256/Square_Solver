#include <stdlib.h>
#include <stdio.h>
#include "function.h"

int main()
{
    while(1)
        {int d = rand();
        printf("%d\n", d);}
}

switch (quality_local)
{
    case (NO_ROOTS):
        if (isnan(x1) && isnan(x2))
            printf(GRN "Test PASSED\n" CRESET);
        else
            print_failed(a, b, c, x1ref, x2ref, nRootsRef, x1, x2, quality_local, ptr_count_fail);
        break;

    case (ONE_ROOT):
    case (INFINITE):
        if ((isnan(x1) && is_equal(x2, x1ref)) || (isnan(x2) && is_equal(x1, x1ref)) ||
            (is_equal(x1, x1ref) && is_equal(x2, x2ref)))
            printf(GRN "Test PASSED\n" CRESET);
        else
            print_failed(a, b, c, x1ref, x2ref, nRootsRef, x1, x2, quality_local, ptr_count_fail);
        break;

    case (TWO_ROOTS):
        if ((is_equal(x1, x1ref) && is_equal(x2, x2ref)) ||
                (is_equal(x1, x2ref) && is_equal(x2, x1ref)))
            printf(GRN "Test PASSED\n" CRESET);
        else
            print_failed(a, b, c, x1ref, x2ref, nRootsRef, x1, x2, quality_local, ptr_count_fail);
        break;

    default:
        print_failed(a, b, c, x1ref, x2ref, nRootsRef, x1, x2, quality_local, ptr_count_fail);
        break;
}


/*enum enum_decisions //корни ур-я
{
    NO_ROOTS,
    ONE_ROOT,
    TWO_ROOTS,
    INFINITE
};

enum enum_opportunity_user //возврат ф-ии с выбором пользователя
{
    STOP,
    GO,
    STAT,
    CONTINUE,
    TEST
};

enum enum_decisions decide_square(double a, double b, double c, double* x1_ptr, double* x2_ptr);
int is_zero(double x);
void clean_stdin(); //очистка входного потока
enum enum_decisions decide_line(double b, double c, double* px1); //решение линейного уравнения
void input_coeffs(double* a, double* b, double* c, int* pcount_bad_enter); //ввод коэффициентов ур-я
void decide_equation(double a, double b, double c, double* px1, double* px2, enum enum_decisions* p_quantity); //решение ур-я
void print_answer(int quantity, double* px1, double* px2, int* count_solutions); //вывод ответа
void input_one_coeff(double*, int*);//ввод одного коэфф-та + проверка
void statistic(int*, int*, int*); //статистика пользователя
enum enum_opportunity_user ask_user(int* pflag_stat, int* pflag_exit, int* pcount_bad_enter); //запрос юзера
*/

if (equalisation[j] == '=')
    {
        sign = (-1);
        local_sign = 1;
        j++;
        continue;
    }





int sign = 1, local_sign = 1;

for (int j = 0; equalisation[j] != '\0';) //leave + - x = xx digit
{
    int local_coeff = 0;

    while (isdigit(equalisation[j]))
    {
        local_coeff = local_coeff * 10 + (equalisation[j] - '0');
        j++;
    }

    //printf("hehehe\n");

    if (equalisation[j] == 'x')
    {
        if (equalisation[j + 1] == 'x')     //_xx
        {
            (*ptr_coeffs).a += local_coeff * sign * local_sign;
            j += 2;
            continue;
        }
        else
        {
            (*ptr_coeffs).b += local_coeff * sign * local_sign;
            j += 2;
            continue;
        }
    }
    else
    {
        (*ptr_coeffs).c += local_coeff * sign * local_sign;
    }

    if (equalisation[j] == '+')
    {
        local_sign = 1;
        j++;
    }
    else if (equalisation[j] == '-')
    {
        local_sign = -1;
        j++;
    }
    else if (equalisation[j] == '=')
    {
        sign = -1;
        j++;
    }
}

TestCase test1 = {.a = 0, .b = 0, .c = 0, .nRootsRef = INFINITE,
                .x1ref = NAN, .x2ref = NAN};
    TestCase test2 = {.a = 1, .b = 0, .c = 0, .nRootsRef = 1,
                .x1ref = 0, .x2ref = 1}; //error

    TestCase test3 = {.a = 0, .b = 1, .c = 0, .nRootsRef = 1,
                .x1ref = 0, .x2ref = NAN};

    TestCase test4 = {.a = 0, .b = 0, .c = 1, .nRootsRef = 0,
                .x1ref = NAN, .x2ref = NAN};
    TestCase test5 = {.a = 1, .b = 1, .c = 1, .nRootsRef = 0,
                .x1ref = NAN, .x2ref = NAN};
    TestCase test6 = {.a = 1, .b = -1, .c = 0, .nRootsRef = 2,
                .x1ref = 5267, .x2ref = 1}; //error
    TestCase test7 = {.a = 0, .b = 1, .c = 1, .nRootsRef = 1,
                .x1ref = -1, .x2ref = NAN};

TestCase testsStreet[] =
{{.a = 0, .b = 0, .c = 0, .nRootsRef = INFINITE, .x1ref = NAN, .x2ref = NAN},
{.a = 1, .b = 0, .c = 0, .nRootsRef = 1, .x1ref = 0, .x2ref = 1},    //error
{.a = 0, .b = 1, .c = 0, .nRootsRef = 1, .x1ref = 0, .x2ref = NAN},
{.a = 0, .b = 0, .c = 1, .nRootsRef = 0, .x1ref = NAN, .x2ref = NAN},
{.a = 1, .b = 1, .c = 1, .nRootsRef = 0,.x1ref = NAN, .x2ref = NAN},
{.a = 1, .b = -1, .c = 0, .nRootsRef = 2, .x1ref = 5267, .x2ref = 1}, //error
{.a = 0, .b = 1, .c = 1, .nRootsRef = 1, .x1ref = -1, .x2ref = NAN}}

int size = sizeof(testsStreet) / sizeof(TestCase);

for (int i = 0; i < size; i++)
{
    RunOneTest(testsStreet[i], &count_fail);
}


//printf(BLU "Choose enter format\n" CRESET);
    //printf(MAG "default -> default enter(by 1 coeff)\n");
    //char choose_enter[] = "Choose enter format\n";


#include "raylib.h"
#include "function.hpp"

int square_function_graphic(coefficients coeffs)
{
    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Square_function_graphic");

    SetTargetFPS(60);

    float x_left = 0.0;
    float x_right = 0.0;
    float y_both = 0.0;

    while (!WindowShouldClose())
    {
        Vector2 startPos_left = {(float)screenWidth/2 - x_left, (float)screenHeight/3 + y_both};
        Vector2 startPos_right = {(float)screenWidth/2 + x_right, (float)screenHeight/3 + y_both};

        y_both = coeffs.a * x_left * x_left + coeffs.b * x_left + coeffs.c;

        x_left += 0.5;
        x_right += 0.5;

        Vector2 endPos_left = {(float)screenWidth/2 - x_left, (float)screenHeight/3 + y_both};
        Vector2 endPos_right = {(float)screenWidth/2 + x_right, (float)screenHeight/3 + y_both};


        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Square graphic", 10, 10, 20, DARKGRAY);

            DrawLineEx(startPos_left, endPos_left, 10.0, MAROON);
            DrawLineEx(startPos_right, endPos_right, 10.0, MAROON);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
