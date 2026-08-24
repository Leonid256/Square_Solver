#include "function.h"
#include <stdio.h>
#include <ctype.h>
#include "visual.h"
#include <unistd.h>
#include <assert.h>
//--------------------------------------------------------------------------------------
const int ENTER_LIMIT = 1000;
//--------------------------------------------------------------------------------------
void custom_input(coefficients* ptr_coeffs)
{
    assert (ptr_coeffs != NULL);

    int equalisation[ENTER_LIMIT] = {};
    int simbol;

    (*ptr_coeffs).a = 0;
    (*ptr_coeffs).b = 0;
    (*ptr_coeffs).c = 0;

    clean_stdin();
    
    printf(BLU "Enter Your square equalisation\n" CRESET);

    for (int i = 0;(simbol = getchar()) != '\n' && simbol != EOF && i < ENTER_LIMIT - 1;)
    {
        if (simbol == '*' || simbol == ' ' || simbol == '\t')
            continue;
        else
        {
            equalisation[i] = simbol;
            i++;
        }
    }

    int sign = 1, local_sign = 1;

    for (int j = 0; equalisation[j] != '\0'; j++) //leave + - x = xx digit
    {
        if (equalisation[j] == '-')
        {
            local_sign = -1;
            continue;
        }
        else if (equalisation[j] == '+')
        {
            local_sign = 1;
            continue;
        }
        else if (equalisation[j] == 61)
        {
            sign = -1;
            continue;
        }
        else if (equalisation[j] >= 48 && equalisation[j] <= 57)
        {
            if (equalisation[j + 1] == 'x')
            {
                if (equalisation[j + 2] == 'x') //xx
                {
                    (*ptr_coeffs).a += (equalisation[j]-'0') * sign * local_sign;
                    continue;
                }
                else //x
                {
                    (*ptr_coeffs).b += (equalisation[j]-'0') * sign * local_sign;
                    continue;
                }
            }
            else //no x
            {
                (*ptr_coeffs).c += (equalisation[j]-'0') * sign * local_sign;
                continue;
            }
        }
        else if (equalisation[j] == 'x')
        {
            if (equalisation[j - 1] == 'x')     //xX
                continue;
            else if (isdigit(equalisation[j-1]))    //0X
                continue;
            else if (equalisation[j+1] == 'x')  //_Xx
                (*ptr_coeffs).a += 1 * sign * local_sign;
            else                                //_X_
                (*ptr_coeffs).b += 1 * sign * local_sign;
        }
    }
    printf(BLU "Your coeffitients: %lg %lg %lg\n" CRESET,
                (*ptr_coeffs).a, (*ptr_coeffs).b, (*ptr_coeffs).c);
}
