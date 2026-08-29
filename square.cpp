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
/**
    \brief Погрешность double
    \details Данная константа отвечает за погрешность у чисел типа double, при которой число(или разность чисел) считается равным нулю
*/
const double EPSILON          = 1e-3;     // == 0.001
//---------------------------------------------------------------------------------------------
/**
    \brief Размер буфера
    \details Размер буфера, в который записываются введенные команды(все команды имеют меньшую длину)
*/
const int    BUFFER_SIZE      = 10;
//---------------------------------------------------------------------------------------------
/**
    \brief Завершение программы
    \details Команда, при вводе которой пользователем программа завершается
*/
const char*  EXIT_CYCLE       = "stop";
//---------------------------------------------------------------------------------------------
/**
    \brief Продолжение программы
    \details Команда, при вводе которой пользователем программа продолжается(дальнейшее решение уравнений)
*/
const char*  CONTINUE_CYCLE   = "go";
//---------------------------------------------------------------------------------------------
/**
    \brief Открытие статистики
    \details Команда, при вводе которой пользователем открывается раздел статистики
*/
const char*  STATISTIC        = "stat";
//---------------------------------------------------------------------------------------------
/**
    \brief Открытие раздела тестирования
    \details Команда, при вводе которой пользователем открывается раздел тестирования
*/
const char*  TESTING          = "test";
//---------------------------------------------------------------------------------------------
/**
    \brief Обнуление статистики
    \details Команда, при вводе которой пользователем в разделе статистики, статистика обнуляется
    \note Команда предназначена только для раздела статистики
*/
const char*  RESTART_STAT     = "restart";
//---------------------------------------------------------------------------------------------
/**
    \brief Выход из статистики
    \details Команда, при вводе которой пользователем в разделе статистики, осуществляется выход из этого раздела
    \note Команда предназначена только для раздела статистики
*/
const char*  EXIT_STAT        = "exit";
//---------------------------------------------------------------------------------------------
/**
    \brief Пароль от раздела тестирования
    \note Команда предназначена только для входа в раздел тестирования
*/
const char*  PASSWORD         = "5267";
//---------------------------------------------------------------------------------------------
/**
    \brief Выбор тестирования из файла
    \details Команда, при вводе которой пользователем в разделе тестирования, запускается тестирование со значениями из файла
    \note Команда предназначена только для раздела тестирования
*/
const char*  FILE_TEST        = "file";
//---------------------------------------------------------------------------------------------
/**
    \brief Выбор дефолтного тестирования
    \details Команда, при вводе которой пользователем в разделе тестирования, запускается тестирование с дефолтными значениями
    \note Команда предназначена только для раздела тестирования
*/
const char*  DEFAULT_TEST     = "default";
//---------------------------------------------------------------------------------------------
/**
    \brief Выбор дефолтного ввода
    \details Команда, при вводе которой пользователем в разделе ввода уравнения, запускается ввод по 1 коэффициенту
    \note Команда предназначена только для раздела ввода уравнения
*/
const char*  DEFAULT_ENTER    = "def";
//---------------------------------------------------------------------------------------------
/**
    \brief Выбор ввода полного уравнения
    \details Команда, при вводе которой пользователем в разделе ввода уравнения, запускается ввод полного уравнения
    \note Команда предназначена только для раздела ввода уравнения
*/
const char*  CUSTOM_ENTER     = "cus";//---------------------------------------------------------------------------------------------
/**
    \brief Режим дебага
    \details Данный режим предполагает уменьшенное количество выводов(только необходимые для понимания места программы)
    \note Данный режим выбирается только через код, запроса на него нет у пользователя. Может иметь значение 0 и 1.
*/
const int    DEBUG_MODE       = 1;
//---------------------------------------------------------------------------------------------
/**
    \brief Замена микросекунд на миллисекунды
*/
#define sleep_ms(ms) usleep((ms) * 1000)
//---------------------------------------------------------------------------------------------
/**
    \brief Основная функция main
    \details Основная функция в программе. Вызываются функции вывода названия программы и кошечки. Далее вызывается функция запроса 
    пользователя о дальнейших действиях. Программа либо останавливается(команда STOP), либо открывается раздел статистики(команда STAT), 
    либо вызываются функции запроса коэффициентов и решения уравнения(команда GO), либо запрашивается пароль для тестирования, и 
    в случае успеха открывается этот раздел(команда TEST), а в случае неверной команды от пользователя это указывается и запрос повторяется
    \return 0 в случае успешного завершения
*/
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

    while (true)
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
                    choose_test();
                break;
        }
        if (flag_stop_program)
            break;
    }
    return 0;
}

//---------------------------------------------------------------------------------------------
/**
    \brief Функция проверяющая double на равенство нулю
    \details Функция сравнивает модуль числа с плавающей точкой с указанной заранее погрешностью, берущейся за нуль
    \param [in] x проверяемое число
    \return "true" если число равно нулю с учетом погрешности
    \return "false" если число не равно нулю
*/
bool is_zero(double x) //проверка double на нулевое значение
{
    if (fabs(x) < EPSILON)
        return true;
    else return false;
}
//---------------------------------------------------------------------------------------------
/**
     \brief Функция сравнения двух чисел double
     \details Функция сравнивает два числа с плавающей точкой, учитывая введенную заранее погрешность
     \param [in] x1, x2 сравниваемые числа
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
/**
    \brief Функция очистки ввода
    \details Функция очищает входной поток от всех неиспользовавшихся символов
    \note В случае пустого входного потока, функция будет ждать символ новой строки или EOF, не продолжая программу
    \return Функция ничего не возвращает
*/
void clean_stdin() //очистка ввода
{
    int c = 0;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}
//---------------------------------------------------------------------------------------------
/**
    \brief Функция, решающая квадратное уравнение
    \details Данная функция решает исключительно квадратные уравнения, записывая результат решения в значения переданных в нее указателей
    \param [in] coeffs коэффициенты квадратного уравнения
    \note Функция выдает ошибку через assert, если в нее передать коэфициент уравнения a = 0
    \param [out] x1_ptr,  x2_ptr указатели, для записи в их значения результатов решения
    \return "ONE_ROOT" в случае единственного решения (дискриминант d = 0)
    \return "NO_ROOTS" в случае отсутствия решений (дискриминант d < 0)
    \return "TWO_ROOTS" в случае двух решений (дискриминант > 0)
*/
enum enum_decisions decide_square(coefficients coeffs, double* x1_ptr, double* x2_ptr) // ф-ия решение квадратного уравнения
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
/**
    \brief Функция, решающая линейное уравнение
    \details Данная функция решает линейное уравнение, записывая результат в значение переданного ей указателя
    \param [in] coeffs коэффициенты уравнения, где a = 0
    \note Функция выдает ошибку через assert, если в нее передать коэфициент уравнения a != 0
    \param [out] ptr_x указатель для записи результата решения уравнения
    \return "INFINITE" в случае, когда корней уравнения бесконечное количество (тождество 0 = 0)
    \return "NO_ROOTS" в случае отсутствия корней уравнения
    \return "ONE_ROOT" в случае обычного линейного уравнения с одним корнем
*/
enum enum_decisions decide_line(coefficients coeffs, double* ptr_x) // ф-ия решение линейного уравнения
{
    assert (ptr_x != NULL);
    assert (is_zero(coeffs.a));

    if (is_zero(coeffs.b))
    {
        if (is_zero(coeffs.c))
            return INFINITE;
        else
            return NO_ROOTS;
    }

    *ptr_x = (-coeffs.c) / coeffs.b;
    return ONE_ROOT;
}
//---------------------------------------------------------------------------------------------
/**
    \brief Функция, решающая любое уравнение 2 степени
    \details Данная функция получает коэффициенты уравнения и, исходя из них, вызывает либо функцию, решающую линейное уравнение, либо
        функцию, решающую квадратное уравнение
    @see decide_square, decide_line
    \param [in] coeffs коэффициенты уравнения 2 степени
    \param [out] ptr_x1, ptr_x2 указатели для записи результатов решения уравнения
    \note В случае единственного решения, результат будет записан по указателю на первый корень (ptr_x1)
    \param ptr_quantity указатель для записи количества корней уравнения
    \return Функция ничего не возвращает
*/
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
/**
    \brief Функция для ввода коэффициентов уравнения 2 степени
    \details Данная функция запрашивает у пользователя 3 коэффициента уравнения, вызывая функцию, считывающую 1 коэффициент. Записывает 
             ввод в значения переданных ей указателей. В конце печатает введенные пользователем коэффициенты
    @see input_one_coeff
    \param [out] ptr_coeffs указатель для записи введенных коэффициентов уравнения
    \param [in, out] ptr_count_bad_enter указатель на количество неверных вводов во всей программе
    \return Функция ничего не возвращает
*/
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
/**
    \brief Функция, запрашивающая у пользователя один коэффициент
    \details Данная функция считывает один введеднный коэффициент уравнения и проверяет его на верность (тип double). В случае 
        верного ввода, функция записывает коэффициент по указателю, переданному ей, иначе запрашивает повторно ввод коэффициента(увеличивая
        количество неверных вводов в программе). Вызывается функцией ввода трех коэффициентов уравнения (input_coeffs)
    @see input_coeffs
    \param [out] ptr_1coeff указатель на адрес, куда записывается введенный коэффициент
    \param [in, out] ptr_count_bad_enter указатель на счетчик неверных вводов во всей программе, который увеличивается при неверном вводе
    \return Функция ничего не возращает
*/
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
/**
    \brief Функция печатает результат решения уравнения 2 степени
    \details Данная функция печатает количество корней уравнения, а также сами корни в случае их наличия. При одном или двух решениях
        данная функция строит график уравнения, вызывая square_function_graphic. Также функция увеличивает счетчик количества решенных
        уравнений
    @see square_function_graphic
    \param [in] coeffs коэффициенты решенного уравнения(нужны для передачи в качестве параметров функции построения графиков)
    \param [in] quantity количество решений уравнения
    \param [in] ptr_x1, ptr_x2 указатели на значения корней уравнения
    \param [in, out] ptr_count_solutions указатель на счетчик количества решенных уравнений
    \return Функция ничего не возращает
*/
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

            square_function_graphic(coeffs, *ptr_x1, *ptr_x2, quantity);

            break;
        case TWO_ROOTS:
            print_typewriter("Уравнение имеет два решения:\n", 15);
            print_typewriter("x1 = ", 10);
            printf("%lg\n", *ptr_x1);
            print_typewriter("x2 = ", 10);
            printf("%lg\n\n",*ptr_x2);

            square_function_graphic(coeffs, *ptr_x1, *ptr_x2, quantity);

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
/**
    \brief Функция, отвечающая за статистику в программе
    \details Данная функция печатает статистику программы(количество решенных уравнений и неверных вводов). Функция запрашивает у 
    пользователя дальнейшие действия. По желанию пользователя, статистику можно обнулитьили же выйти из этого раздела. Стоит проверка на 
    некорректный ввод(запрос пользователя повторяется). Статистика выводится при завершении программы, однако в этом случае 
    нет запроса пользователя
    \param [in] ptr_count_solutions указатель на счетчик решенных уравнений
    \param [in, out] ptr_count_bad_enter указатель на счетчик неверных вводов в программе(в самой статистике при 
            неверном вводе счетчик также растет)
    \param [in] ptr_flag_exit указатель на флаг завершения программы
    \return Функция ничего не возращает
*/
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
/**
    \brief Функция, запрашивающая дальнейшие действия у пользователя
    \details Функция дает пользователю выбор дальнейших действий(решение уравнений, остановка программы, открытие статистики,
        открытие раздела тестирования функции решения уравнений). Функция проверяет ввод, и в случае неверного ввода печатает это
    \note Функция не позволяет дважды подряд открывать раздел статистики
    \param [in, out] ptr_flag_stat Указатель на флаг возможности открытия раздела статистики(дважды подряд открытие невозможно)
    \param [out] ptr_flag_exit Указатель на флаг закрытия программы(для отсутствия запроса пользователя в разделе статистики)
    \param [in, out] ptr_count_bad_enter Указатель на счетчик неверных вводов в программе
    \return "GO" в случае выбора пользователем продолжения решения уравнений
    \return "STOP" в случае выбора пользователем остановки программы
    \return "STAT" в случае выбора пользователем открытия статистики
    \return "TEST" в случае выбора пользователем открытия раздела тестирования
    \return "CONTINUE" в случае неверного ввода
*/
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
    \brief Функция, проверяющая пароль для раздела тестирования
    \details Данная функция запрашивает пароль от раздела тестирования, далее сравнивает его с константным значением 
    \param указатель на счетчик неверных вводов в программе
    \note В случае неверного ввода пароля, раздел тестирования сразу закрывается, нет повторного запроса пароля
    \return "true" в случае верного пароля
    \return "false" в случае неверного пароля
*/
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
/**
    \brief Функция для выбора режима тестирования
    \details Данная функция позволяет пользователю выбрать режим тестирования(значениями из файла, либо значениями 
        указанными в программе дефолтно). В случае верного ввода функция вызывает функции тестирования
    @see RunTests_from_file(), RunTests()
    \note В случае неверного ввода режима раздел тестирования закрывается
    \return 1 в случае успешного запуска тестирования(любого)
    \return 0 в случае неверного ввода пользователем
*/
int choose_test()
{
    char test_choose[BUFFER_SIZE] = {};

    printf(BLU);
    print_typewriter("Choose variety of testing\n", 20);
    printf(CRESET);

    if(!DEBUG_MODE)
    {
        print_typewriter("file -> testing from user file\n", 20);
        print_typewriter("default -> testing by default values\n", 20);
    }

    scanf(" %8s", test_choose);

    if (!strcmp(test_choose, FILE_TEST))
    {
        // TODO: read filename
        printf(BLU);
        print_typewriter("Good, testing from user file\n", 20);
        printf(CRESET);

        RunTests_from_file();
        return 1;
    }
    else if (!strcmp(test_choose, DEFAULT_TEST))
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
/**
    \brief Функция, позволяющая пользователю выбрать способ ввода уравнения
    \details Данная функция запрашивает у пользователя формат ввода уравнения(по одному коэффициенту или же целое уравнение), а также 
        проверяет введенный текст на корректность. В случае неверного ввода, запрос повторяется(и увеличивается счетчик неверных вводов). 
        После успешного ввода запускается выбранный пользователем способ ввода уравнения
    @see input_coeffs(), custom_input()
    \param [out] ptr_coeffs указатель на коэффициенты уравнения, передаваемый функциям, считывающим сами коэффициенты
    \param [in, out] ptr_count_bad_enter указатель на счетчик неверных вводов программе
    \return 0 в случае успешного ввода коэффициентов либо уравнения
    \return 1 в ином случае
*/
int choose_enter(coefficients* ptr_coeffs, int* ptr_count_bad_enter)
{
    assert (ptr_coeffs != NULL);
    assert (ptr_count_bad_enter != NULL);

    char test_choose[BUFFER_SIZE] = {};

    print_typewriter("Choose enter format\n", 20);
    
    if (!DEBUG_MODE)
    {
        printf(MAG);
        print_typewriter("def -> default enter(by 1 coeff)\n", 15);
        print_typewriter("cus -> custom enter(full equalisation)\n", 15);
        printf(CRESET);
    }

    while (true)
    {
        scanf(" %8s", test_choose);      //8 - длина считываемой строки

        if (!strcmp(test_choose, DEFAULT_ENTER))
        {
            printf(BLU);

            if (!DEBUG_MODE)
                print_typewriter("Enter coefficients one by one\n", 25);

            printf(CRESET);

            input_coeffs(ptr_coeffs, ptr_count_bad_enter);
            return 0;
        }
        else if (!strcmp(test_choose, CUSTOM_ENTER))
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
/**
    \brief Функция плавного вывода
    \details Данная функция позволяет выводить текст плавно, с задержкой после каждого символа
    \param [in] text - текст, который нужно вывести
    \param [in] delay_ms задержка после вывода каждого символа в миллисекундах
    \return функция ничего не возвращает
*/
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
/**
    \brief Вывод кошечки :)
    \details Данная функция выводит на экран кошечку, раскрашенную в цвета триколора
    \param функция не принимает параметров
    \return функция ничего не возвращает
*/
void print_cat(void)
{
    printf(WHT "\t\t|\\---/|\n" CRESET);
    printf(BLU "\t\t| o_o |\n" CRESET);
    printf(RED "\t\t \\_^_/ \n" CRESET);
}
//----------------------------------------------------------------------------------------
/**
    \brief Вывод названия программы
    \details Функция выводит название программы в рамочке
    \param функция не принимает параметров
    \return функция ничего не возвращает
*/
void print_logo(void)
{
    printf("|--------------------------------------|\n");
    printf("|" MAG "Hello, that is square solver programm" CRESET " |\n");
    printf("|--------------------------------------|\n");
}
