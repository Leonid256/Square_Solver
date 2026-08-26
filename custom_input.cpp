#include "function.hpp"
#include <stdio.h>
#include <ctype.h>
#include "visual.hpp"
#include <unistd.h>
#include <assert.h>
//--------------------------------------------------------------------------------------
const int ENTER_LIMIT = 1000;
//--------------------------------------------------------------------------------------
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
    print_typewriter("Enter Your square equalisation\n", 30);
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


    for (int j = 0; equalisation[j] != '\0';) //leave + - x = xx digit
    {
        double local_coeff = 0;

        // while (isdigit(equalisation[j]))
        // {
        //     local_coeff = local_coeff * 10 + (equalisation[j] - '0');
        //     j++;
        // }

        int len_coeff = 0;
        sscanf(&equalisation[j], "%lg%n", &local_coeff, &len_coeff);
        j += len_coeff;

        if (equalisation[j] == 'x')
        {
            if (equalisation[j + 1] == 'x')     //_xx
            {
                (*ptr_coeffs).a += local_coeff * sign * local_sign;
                j += 2;
                continue;
            }
            else                                //_x_
            {
                (*ptr_coeffs).b += local_coeff * sign * local_sign;
                j += 1;
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
            local_sign = 1;
            j++;
        }
    }

    printf(BLU);
    print_typewriter("Your coeffitients: ", 15);
    printf("%lg %lg %lg\n", (*ptr_coeffs).a, (*ptr_coeffs).b, (*ptr_coeffs).c);
    printf(CRESET);
}
