// TODO: int->double
// ф-я считывания к-ов квадратного уравнения
int gets_square(int* a, int* b, int* c) {
    char x;
    printf("Введите уравнение в формате: a * x*x + b * x + c = 0\n");
    while ((x = getchar()) != ('+')) {
        if (isdigit(x))
            *a = 10 * (*a) + x - '0';
    }

    while ((x = getchar()) != 'x') {
        if (isdigit(x))
            *b = 10 * (*b) + x - '0';
    }

    while ((x = getchar()) != '=') {
        if (isdigit(x))
            *c = 10 * (*c) + x - '0';
    }
    while ((x = getchar()) != '0')
        ;

    //printf("%d %d %d\n", *a, *b, *c);
    return 1;
}
