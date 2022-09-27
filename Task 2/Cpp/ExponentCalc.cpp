#include <iostream> 
#include <ctime>
#include <cmath>

using namespace std;

typedef double exponent_func(double base, int exponent);

class Exponent {
    public:
        Exponent();
        ~Exponent();
        double exponent_method_slow(double base, int exponent);
        double exponent_method_fast(double base, int exponent);
        double exponent_method_standard(double base, int exponent);
};

Exponent::Exponent()
{}

Exponent::~Exponent()
{
    delete this;
}

double Exponent::exponent_method_slow(double base, int exponent)
{
    if(exponent == 0) return 1;
    return base * exponent_method_slow(base, exponent - 1);
}

double Exponent::exponent_method_fast(double base, int exponent)
{
    if(exponent == 0) return 1;
    else if (exponent % 2 != 0) return base * exponent_method_fast(base * base, (exponent - 1)/2);
    return exponent_method_fast(base * base, exponent / 2); 
}

double Exponent::exponent_method_standard(double base, int exponent)
{
    return pow(base, exponent);
}

void time_test(double base, int exponent, Exponent* exponent_calc, double(Exponent::*fp)(double, int)) 
{
    int seconds = 1;
    time_t start, end;
    double count = 0;
    start = time(NULL);
    while(difftime(time(NULL), start) < seconds) {
        (exponent_calc->*fp)(base, exponent);
        count++;
    }
    double nano_sec = 1000000000*(seconds/count);
    double count_sec = count/seconds;
    cout << "Exponent = " << exponent << count_sec << " cycle per second and " << nano_sec << " nanoseconds pr cycle " << endl;  
}

int main() 
{
    int x = 1.001;
    auto exponent_calc = new Exponent();
    
    cout << "Answer for slow method for 2 to the power of 12 is " << exponent_calc->exponent_method_slow(3, 14) << endl;
    cout << "Answer for fast method for 2 to the power of 12 is " << exponent_calc->exponent_method_fast(3, 14) << endl;
    cout << "Answer for standard method for 2 to the power of 12 is " << exponent_calc->exponent_method_standard(3, 14) << endl;

    cout << "Answer for slow method for 3 to the power of 14 is " << exponent_calc->exponent_method_slow(3, 14) << endl;
    cout << "Answer for the fast method for 3 to the power of 14 is " << exponent_calc->exponent_method_fast(3, 14) << endl;
    cout << "Answer for standard method for 3 to the power of 14 is " << exponent_calc->exponent_method_standard(3, 14) << endl;

    cout << endl << "Slow method time complexity O(n)" << endl;
    time_test(x, 10, exponent_calc, &Exponent::exponent_method_slow);
    time_test(x, 100, exponent_calc, &Exponent::exponent_method_slow);
    time_test(x, 1000, exponent_calc, &Exponent::exponent_method_slow); 
    time_test(x, 10000, exponent_calc, &Exponent::exponent_method_slow);
    cout << endl << "Fast method time complexity O(log(n))" << endl;
    time_test(x, 10, exponent_calc, &Exponent::exponent_method_fast);
    time_test(x, 100, exponent_calc, &Exponent::exponent_method_fast);
    time_test(x, 1000, exponent_calc, &Exponent::exponent_method_fast); 
    time_test(x, 10000, exponent_calc, &Exponent::exponent_method_fast);
    cout << endl << "Standard method time complexity O(1)" << endl;
    time_test(x, 10, exponent_calc, &Exponent::exponent_method_standard);
    time_test(x, 100, exponent_calc, &Exponent::exponent_method_standard); 
    time_test(x, 1000, exponent_calc, &Exponent::exponent_method_standard);      
    time_test(x, 10000, exponent_calc, &Exponent::exponent_method_standard);
    return 0;
}