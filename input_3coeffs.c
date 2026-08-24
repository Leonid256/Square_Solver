#include <assert.h>

void input_coeffs(double* pa, double* pb, double* pc)
{
    assert (pa != NULL);
    assert (pb != NULL);
    assert (pc != NULL);

    int quantity_coeff = 0;
    while (1)
    {
        quantity_coeff = scanf("%lg %lg %lg", pa, pb, pc);
        if (quantity_coeff != 3)
            {
            printf("Wrong enter (Right: a b c)\n");
            clean_stdin();
            continue;
            }
        else
            {
            clean_stdin();
            printf("Your coeffitients: %lg %lg %lg\n\n", *pa, *pb, *pc);
            sleep(1);
            break;
            }
    }
}
