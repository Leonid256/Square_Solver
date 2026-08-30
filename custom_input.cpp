#include "function.hpp"
#include <stdio.h>
#include <ctype.h>
#include "visual.hpp"
#include <unistd.h>
#include <assert.h>
//--------------------------------------------------------------------------------------
/**
    \brief Максимальная длина вводимого уравнения
*/
const int ENTER_LIMIT = 1000;
//--------------------------------------------------------------------------------------
/**
    \brief Функция для ввода целого уравнения
    \details Данная функция считывает введенное пользователем уравнение в любом формате, из любого количества слагаемых. Допустимы
    символы х, *, +, -, =, а также цифры. Остальные символы(а также *) удаляются. 
    \param [out] ptr_coeffs указатель на коэффициенты уравнения
    \return функция ничего не возращает
*/
void custom_input(coefficients* ptr_coeffs)
{
    assert (ptr_coeffs != NULL);

    char equalisation[ENTER_LIMIT] = {};
    int simbol;

    (*ptr_coeffs).a = 0;
    (*ptr_coeffs).b = 0;
    (*ptr_coeffs).c = 0;

    clean_stdin();

    printf(BLU);
    print_typewriter("Enter Your square equalisation ", 30);
    printf(RED);
    print_typewriter("(extra symbols will be removed)\n", 30);
    printf(CRESET);

    int i = 0;
    for (i = 0;(simbol = getchar()) != '\n' && simbol != EOF && i < ENTER_LIMIT - 1;)
    {
        if (isdigit(simbol) || simbol == 'x' || simbol == '+'
                || simbol == '-' || simbol == '=')
        {
            equalisation[i++] = simbol;
        }
        else if (simbol == 'X')
        {
            equalisation[i++] = 'x';
        }
        else
            continue;
    }

    int sign = 1, local_sign = 1;


    for (int i = 0; equalisation[i] != '\0';) //leave + - x = xx digit
    {
        double local_coeff = 0;

        while (isdigit(equalisation[i]))
        {
            local_coeff = local_coeff * 10 + (equalisation[i] - '0');
            i++;
        }

        // int len_coeff = 0;     //alternative (NOT WORK!!!)
        // sscanf(&equalisation[j], "%lg%n", &local_coeff, &len_coeff);
        // j += len_coeff;

        if (equalisation[i] == 'x')
        {
            if (equalisation[i + 1] == 'x')     //_xx
            {
                if (local_coeff != 0)
                    (*ptr_coeffs).a += local_coeff * sign * local_sign;
                else
                    (*ptr_coeffs).a += 1 * sign * local_sign; //case xx
                i += 2;
                continue;
            }
            else                                //_x_
            {
                if (local_coeff != 0)
                    (*ptr_coeffs).b += local_coeff * sign * local_sign;
                else
                    (*ptr_coeffs).b += 1 * sign * local_sign; //case x
                i += 1;
                continue;
            }
        }
        else
        {
            (*ptr_coeffs).c += local_coeff * sign * local_sign;
        }

        if (equalisation[i] == '+')
        {
            local_sign = 1;
            i++;
        }
        else if (equalisation[i] == '-')
        {
            local_sign = -1;
            i++;
        }
        else if (equalisation[i] == '=')
        {
            sign = -1;
            local_sign = 1;
            i++;
        }
    }

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
