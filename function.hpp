//---------------------------------------------------------------------------------------------
/**
    \brief Корни уравнения
    \details Значения количества корней у уравнения: 0 корней, 1 корень, 2 корня или бесконечное количество корней
*/
enum enum_decisions //корни ур-я
{
    DECISION_NO_ROOTS,   /**< нет корней */
    DECISION_ONE_ROOT,   /**< 1 корень */
    DECISION_TWO_ROOTS,  /**< 2 корня */
    DECISION_INFINITE_ROOTS    /**< бесконечное количество корней */
};
//---------------------------------------------------------------------------------------------
/**
    \brief Выбор пользователя
    \details Константные выражения, которые может вернуть функция, запрашивающая у пользователя дальнейшие действия
*/
enum enum_opportunity_user //возврат ф-ии с выбором пользователя
{
    MODE_STOP,       /**< остановка программы */
    MODE_GO,         /**< продолжение решения уравнений */
    MODE_STAT,       /**< открытие статистики */
    MODE_TEST,       /**< открытие раздела тестов */
    MODE_CONTINUE    /**< неверный ввод(новый запрос пользователя)*/
};
//---------------------------------------------------------------------------------------------
/**
    \brief Структура для тестов
    \details структура включает в себя коэффициенты уравнения, количество корней и ожидаемые корни уравнения
*/
struct TestCase
{
    double a;       /**< коэффициент a*/
    double b;       /**< коэффициент b*/
    double c;       /**< коэффициент c*/

    int nRootsRef;  /**< количество корней */

    double x1ref;   /**< Ожидаемый корень x1 */
    double x2ref;   /**< Ожидаемый корень x2 */
};
//---------------------------------------------------------------------------------------------
/**
    \brief Структура с коэффициентами
    \details структура включает в себя коэффициенты уравнения
*/
struct coefficients
{
    double a;       /**< коэффициент a*/
    double b;       /**< коэффициент b*/
    double c;       /**< коэффициент c*/
};
//---------------------------------------------------------------------------------------------
//user functions
enum_opportunity_user ask_user(int* , int* , int* ); //запрос юзера
void calculate_statics(int*, int*, int*); //статистика пользователя

//---------------------------------------------------------------------------------------------
//user input
void input_coeffs(coefficients* coeffs, int* ); //ввод коэффициентов ур-я
void input_one_coeff(double* ptr_1coeff, int*);//ввод одного коэфф-та + проверка
void custom_input(coefficients* ptr_coeffs); //ввод уравнения
int choose_enter(coefficients* ptr_coeffs, int* ptr_count_bad_enter);// выбор ввода
//---------------------------------------------------------------------------------------------
//output
void print_answer(coefficients coeffs, enum_decisions quantity, double* , double* , int* ); //вывод ответа
void print_typewriter(const char *text, int delay_ms);
void print_cat(void);
void print_logo(void);
//---------------------------------------------------------------------------------------------
//math functions
void decide_equation(coefficients coeffs, double* , double* , enum enum_decisions*);
enum_decisions decide_square(coefficients coeffs, double* x1_ptr, double* x2_ptr);
enum_decisions decide_line(coefficients coeffs, double* px1); //решение линейного уравнения
bool is_equal(double x1, double x2);
bool is_zero(double x);
int square_function_graphic(coefficients coeffs, double x1, double x2, enum_decisions quantity);
//---------------------------------------------------------------------------------------------
//test functions
int RunTests();
void RunOneTest(struct TestCase, int* ptr_count_fail, int* ptr_count_success);
void print_failed(TestCase test, float x1, float x2, int quality_local, int* ptr_count_fail);
bool check_test_password(int* ptr_count_bad_enter);
int RunTests_from_file();
int choose_test();

//---------------------------------------------------------------------------------------------
//service functions
void clean_stdin(); //очистка входного потока
