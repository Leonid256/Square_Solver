#include <stdio.h>
#include <ctype.h>

int main()
{
    int d_1 = 0;
    int n_1 = 0;

    scanf(" %d%n", &d_1, &n_1);
    printf("%d %d", d_1, n_1);
    if (isdigit('4'))
        printf("hehe");
    return 0;
}
