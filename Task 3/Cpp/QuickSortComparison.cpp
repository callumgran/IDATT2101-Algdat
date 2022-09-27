#include <iostream> 
#include <chrono>
#include "QuickSortDualPivotCpp.h"
#include "QuickSortSinglePivotCpp.h"

using namespace std;

#define SIZE (10 * 1000000) //As there are 2 "arrays", keep in mind each element is 8 bytes of memory. 125mill = ca. 1000mb

double RAND(double min, double max)
{
    return (double)rand()/(double)RAND_MAX * (max - min) + min;
}

void fill_random(int *data, int len)
{
    for (int i = 0; i < len; i++) 
    {
        data[i] = rand();
    }
}

void fill_sorted(int *data, int len)
{
    for (int i = 0; i < len; i++) 
    {
        data[i] = i;
    }
}

void fill_reverse_sorted(int *data, int len)
{
    for (int i = 0; i < len; i++) 
    {
        data[i] = len - i;
    }
}

void fill_half_random(int *data, int len)
{
    int j = divide_2(2);
    int k = len / 3;
    for (int i = 0; i < len; i++) 
    {
        if (i % 2 == 0) {
            data[i] = rand();
        } 
        else if (i % 3 == 0) 
        {
            data[i] = k;
        } 
        else 
        {
            data[i] = j;
        }
    }
}

void fill_small_range(int *data, int len)
{
    for (int i = 0; i < len; i++) 
    {
        data[i] = RAND(0, 5);
    }
}

void test_sorted(int *data, int len)
{
    for (int i = 1; i < len; i++) 
    {
        if (data[i] < data[i - 1]) 
        {
            cout << "ERROR! Array is not sorted." << endl;
            break;
        }
        if (i == len - 1) cout << "SUCCESS! Array is sorted." << endl;
    }
}

void sum_array(int *data, int *sum, int len)
{
    *sum = 0;
    for (int i = 0; i < len; i++) 
    {
        *sum += data[i];
    }
}

void copy(int* data, int* copy, int len)
{
    for (int i = 0; i < len; i++) 
    {
        *(copy + i) = *(data + i);
    }
}

void test_values(int sum1, int sum2)
{
    if (sum1 == sum2) cout << "SUCCESS! The sum before sorting: " << sum1 << " is equal to the sum after sorting " << sum2 << endl;
    else cout << "ERROR! The sum before sorting: " << sum1 << " is not equal to the sum after sorting " << sum2 << endl;
}

template <class T> void run_sort_with_time(int *data, T *class_pointer, void(T::*func_pointer)(int*, int*))
{
    double elapsed;
    int sum_before, sum_after;
    sum_array(data, &sum_before, SIZE);
    test_sorted(data, SIZE);
    cout << "Started sorting" << endl;
    auto start = chrono::high_resolution_clock::now();
    (class_pointer->*func_pointer)(data, data + SIZE - 1);
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Time used was " << duration.count() << "ms for " << SIZE/1000000 << " million numbers" << endl;
    test_sorted(data, SIZE);
    sum_array(data, &sum_after, SIZE);
    test_values(sum_before, sum_after);
}

int main()
{
    auto dual_pivot = new QuickSortDual();
    auto single_pivot = new QuickSortSingle();
    int* data = (int*)(malloc(SIZE * sizeof(int)));
    int* copy_arr = (int*)(malloc(SIZE * sizeof(int)));
    srand(time(NULL));
    cout << "--------------------------------------------------------" << endl;
    cout << "Random Numbers: " << SIZE / 1000000 << " million. " << endl << endl; 
    fill_random(data, SIZE);
    copy(data, copy_arr, SIZE);
    cout << "Single Pivot: " << endl;
    run_sort_with_time<QuickSortSingle>(copy_arr, single_pivot, &QuickSortSingle::quicksort);
    copy(data, copy_arr, SIZE);
    cout << endl << "Dual Pivot: " << endl;
    run_sort_with_time<QuickSortDual>(copy_arr, dual_pivot, &QuickSortDual::quicksort);
    cout << "--------------------------------------------------------" << endl << endl;

    cout << "--------------------------------------------------------" << endl;
    cout << "Sorted Numbers: " << SIZE / 1000000 << " million. " << endl << endl; 
    fill_sorted(data, SIZE);
    copy(data, copy_arr, SIZE);
    cout << "Single Pivot: " << endl;
    run_sort_with_time<QuickSortSingle>(copy_arr, single_pivot, &QuickSortSingle::quicksort);
    copy(data, copy_arr, SIZE);
    cout << endl << "Dual Pivot: " << endl;
    run_sort_with_time<QuickSortDual>(copy_arr, dual_pivot, &QuickSortDual::quicksort);
    cout << "--------------------------------------------------------" << endl << endl;

    cout << "--------------------------------------------------------" << endl;
    cout << "Reverse Sorted Numbers: " << SIZE / 1000000 << " million. " << endl << endl; 
    fill_reverse_sorted(data, SIZE);
    copy(data, copy_arr, SIZE);
    cout << "Single Pivot: " << endl;
    run_sort_with_time<QuickSortSingle>(copy_arr, single_pivot, &QuickSortSingle::quicksort);
    copy(data, copy_arr, SIZE);
    cout << endl << "Dual Pivot: " << endl;
    run_sort_with_time<QuickSortDual>(copy_arr, dual_pivot, &QuickSortDual::quicksort);
    cout << "--------------------------------------------------------" << endl << endl;

    cout << "--------------------------------------------------------" << endl;
    cout << "Half Random Numbers: " << SIZE / 1000000 << " million. " << endl << endl; 
    fill_half_random(data, SIZE);
    copy(data, copy_arr, SIZE);
    cout << "Single Pivot: " << endl;
    run_sort_with_time<QuickSortSingle>(copy_arr, single_pivot, &QuickSortSingle::quicksort);
    copy(data, copy_arr, SIZE);
    cout << endl << "Dual Pivot: " << endl;
    run_sort_with_time<QuickSortDual>(copy_arr, dual_pivot, &QuickSortDual::quicksort);
    cout << "--------------------------------------------------------" << endl << endl;

    cout << "--------------------------------------------------------" << endl;
    cout << "Low Range Numbers: " << SIZE / 1000000 << " million. " << endl << endl; 
    fill_small_range(data, SIZE);
    copy(data, copy_arr, SIZE);
    cout << endl << "Dual Pivot: " << endl;
    run_sort_with_time<QuickSortSingle>(copy_arr, single_pivot, &QuickSortSingle::quicksort);
    copy(data, copy_arr, SIZE);
    cout << endl << "Dual Pivot: " << endl;
    run_sort_with_time<QuickSortDual>(copy_arr, dual_pivot, &QuickSortDual::quicksort);
    cout << "--------------------------------------------------------" << endl << endl;
    free(single_pivot);
    free(dual_pivot);
    free(data);
    free(copy_arr);
    return 0;
}