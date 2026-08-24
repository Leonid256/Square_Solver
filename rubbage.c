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


for (int i = 0; i < ENTER_LIMIT - 1; i++)
    {
        printf("%s ", equalisation[i]);
    }
