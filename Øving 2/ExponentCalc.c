#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

double exponent_method_one(double base, int exponent)
{
    if(exponent == 0) return 1;
    return base * exponent_method_one(base, exponent - 1);
}

double exponent_method_two(double base, int exponent)
{
    if(exponent == 0) return 1;
    else if (exponent % 2 != 0) return base * exponent_method_two(base * base, (exponent - 1)/2);
    return exponent_method_two(base * base, exponent / 2); 
}

double c_pow(double base, int exponent)
{
    return pow(base, exponent);
}

void time_test(double base, int exponent, double (*func)(double, int)) 
{
    int seconds = 1;
    time_t start, end;
    double count = 0;
    start = time(NULL);
    while(difftime(time(NULL), start) < seconds) {
        func(base, exponent);
        count++;
    }
    double nano_sec = 1000000000*(seconds/count);
    double count_sec = count/seconds;
    printf("Exponent = %d %f cycle per second and %f nanoseconds pr cycle\n",exponent, count_sec,nano_sec);    
}

int main() 
{
    int x = 1.001;
    printf("Answer for method one for 2 to the power of 10 %f\n", exponent_method_one(2, 12));
    printf("Answer for method two for 2 to the power of 10 %f\n", exponent_method_two(2, 12));
    printf("Answer for pow for 2 to the power of 10 %f\n\n", pow(2, 12));

    printf("Answer for method one for 3 to the power of 14 %f\n", exponent_method_one(3, 14));
    printf("Answer for method two for 3 to the power of 14 %f\n", exponent_method_two(3, 14));
    printf("Answer for pow for 3 to the power of 14 %f\n\n", pow(3, 14));
    
    printf("\nMethod one time complexity O(n)\n");
    time_test(x, 10, &exponent_method_one);
    time_test(x, 100, &exponent_method_one);
    time_test(x, 1000, &exponent_method_one); 
    time_test(x, 10000, &exponent_method_one);
    printf("\nMethod two time complexity O(log(n))\n");
    time_test(x, 10, &exponent_method_two);
    time_test(x, 100, &exponent_method_two);
    time_test(x, 1000, &exponent_method_two); 
    time_test(x, 10000, &exponent_method_two);
    printf("\nC math.pow time complexity O(1)\n");
    time_test(x, 10, &c_pow);
    time_test(x, 100, &c_pow); 
    time_test(x, 1000, &c_pow);      
    time_test(x, 10000, &c_pow);
    return 0;
}