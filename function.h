enum enum_decisions //корни ур-я
{
    // TODO: DECISIONS_NO_ROOTS, etc
    NO_ROOTS,
    ONE_ROOT,
    TWO_ROOTS,
    // TODO: Infinite (?)
    INFINITE
};

enum enum_opportunity_user //возврат ф-ии с выбором пользователя
{
    // TODO: MODE_*, etc
    STOP,
    GO,
    STAT,
    CONTINUE,
    TEST
};

struct TestCase
{
    double a, b, c;
    int nRootsRef;
    double x1ref, x2ref;
};

struct coefficients
{
    double a, b, c;
};
//---------------------------------------------------------------------------------------------
//user functions
// maybe TODO: remove enum keyword
enum enum_opportunity_user ask_user(int* , int* , int* ); //запрос юзера
void calculate_statics(int*, int*, int*); //статистика пользователя

//---------------------------------------------------------------------------------------------
//user input
void input_coeffs(coefficients* coeffs, int* ); //ввод коэффициентов ур-я
void input_one_coeff(double* ptr_1coeff, int*);//ввод одного коэфф-та + проверка
void custom_input(coefficients* ptr_coeffs); //ввод уравнения
int choose_enter(coefficients* ptr_coeffs, int* ptr_count_bad_enter);// выбор ввода
//---------------------------------------------------------------------------------------------
//output
void print_answer(enum_decisions quantity, double* , double* , int* ); //вывод ответа

//---------------------------------------------------------------------------------------------
//math functions
void decide_equation(coefficients coeffs, double* , double* , enum enum_decisions*);
enum enum_decisions decide_square(coefficients coeffs, double* x1_ptr, double* x2_ptr);
enum enum_decisions decide_line(coefficients coeffs, double* px1); //решение линейного уравнения
bool is_equal(double x1, double x2);
bool is_zero(double x);

//---------------------------------------------------------------------------------------------
//test functions
int RunTests();
void RunOneTest(struct TestCase, int* ptr_count_fail);
void print_failed(TestCase test, float x1, float x2, int quality_local, int* ptr_count_fail);
bool check_test_password(int* ptr_count_bad_enter);
int RunTests_from_file();
int chose_test();

//---------------------------------------------------------------------------------------------
//service functions
void clean_stdin(); //очистка входного потока
