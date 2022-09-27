#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include "../InsertionSort.h"
#include "../QuickSortMacros.h"

#define SIZE (50 * 1000000) // As there are 2 "arrays", keep in mind each element is 8 bytes of memory. 125mill = ca. 1000mb
#define RUN_INSERTION 100   // This size is in reality 50 as it is checked by memory placement.

#define median3sort(left, mid, right) \
    {                                 \
        if (*left > *mid)             \
        {                             \
            swap(*left, *mid);        \
        }                             \
        if (*mid > *right)            \
        {                             \
            swap(*mid, *right);       \
            if (*left > *mid)         \
                swap(*left, *mid);    \
        }                             \
    }

#define dual_mid_point(left_0, right_0)                      \
    {                                                        \
        swap(*left_0, *(left_0 + (right_0 - left_0) / 3));   \
        swap(*right_0, *(right_0 - (right_0 - left_0) / 3)); \
        if (*left_0 > *right_0)                              \
            swap(*left_0, *right_0);                         \
    }

void quicksort_single(int *left, int *right);

void quicksort_dual(int *left, int *right);

double RAND(double min, double max)
{
    return (double)rand() / (double)RAND_MAX * (max - min) + min;
}

void partition_single(int *left_0, int *right_0, int **pivot)
{
    int *left = left_0 + 1;
    int *right = right_0;
    int *mid = left_0 + divide_2(right_0 - left_0);
    median3sort(left_0, mid, right_0);
    int piv = *mid;
    *mid = *left;
    *left = piv;
    swap(*mid, *(right_0 - 1));
    while (1)
    {
        do
            left++;
        while (*left < piv);
        do
            right--;
        while (*right > piv);
        if (left >= right)
            break;
        swap(*left, *right);
    }
    *(left_0 + 1) = *right;
    *right = piv;
    *pivot = right;
}

void partition_dual(int *left_0, int *right_0, int **lp, int **hp)
{
    dual_mid_point(left_0, right_0);
    int *left = left_0 + 1;
    int *right = right_0 - 1, *left_1 = left_0 + 1, left_0_val = *left_0, right_0_val = *right_0;
    while (left_1 <= right)
    {
        if (*left_1 < left_0_val)
        {
            swap(*left_1, *left);
            left++;
        }
        else if (*left_1 >= right_0_val)
        {
            while (*right > right_0_val && left_1 < right)
                right--;
            swap(*left_1, *right);
            right--;
            if (*left_1 < left_0_val)
            {
                swap(*left_1, *left);
                left++;
            }
        }
        left_1++;
    }
    left--;
    right++;
    swap(*left_0, *left);
    swap(*right_0, *right);
    *lp = left;
    *hp = right;
}

void quicksort_single(int *left, int *right)
{
    if (right - left >= RUN_INSERTION)
    {
        int *piv;
        partition_single(left, right, &piv);
        quicksort_single(piv + 1, right);
        quicksort_single(left, piv - 1);
    }
    else
    {
        insertion_sort(left, right);
    }
}

void quicksort_dual(int *left, int *right)
{
    if (right - left >= RUN_INSERTION)
    {
        int *lp, *hp;
        partition_dual(left, right, &lp, &hp);
        quicksort_dual(left, lp - 1);
        quicksort_dual(lp + 1, hp - 1);
        quicksort_dual(hp + 1, right);
    }
    else
    {
        insertion_sort(left, right);
    }
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
        if (i % 2 == 0)
        {
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
            printf("ERROR! Array is not sorted.\n");
            break;
        }
        if (i == len - 1)
            printf("SUCCESS! Array is sorted.\n");
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

void copy(int *data, int *copy, int len)
{
    for (int i = 0; i < len; i++)
    {
        *(copy + i) = *(data + i);
    }
}

void test_values(int sum1, int sum2)
{
    if (sum1 == sum2)
        printf("SUCCESS! The sum before sorting: %d is equal to the sum after sorting %d\n", sum1, sum2);
    else
        printf("ERROR! The sum before sorting: %d is not equal to the sum after sorting %d\n", sum1, sum2);
}

void run_sort_with_time(int *data, void (*func)(int *, int *))
{
    double elapsed;
    struct timespec start, finish;
    int sum_before, sum_after;
    sum_array(data, &sum_before, SIZE);
    test_sorted(data, SIZE);
    printf("Started sorting\n");
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    func(data, data + SIZE - 1);
    clock_gettime(CLOCK_MONOTONIC_RAW, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Time used was %fms for %d million numbers. \n", elapsed * 1000, SIZE / 1000000);
    test_sorted(data, SIZE);
    sum_array(data, &sum_after, SIZE);
    test_values(sum_before, sum_after);
}

int main()
{
    int *data = (int *)(malloc(SIZE * sizeof(int)));
    int *copy_arr = (int *)(malloc(SIZE * sizeof(int)));
    srand(time(NULL));
    printf("--------------------------------------------------------\n");
    printf("Random Numbers: %d million. \n\n", SIZE / 1000000);
    fill_random(data, SIZE);
    copy(data, copy_arr, SIZE);
    printf("Single Pivot: \n");
    run_sort_with_time(copy_arr, &quicksort_single);
    copy(data, copy_arr, SIZE);
    printf("\nDual Pivot: \n");
    run_sort_with_time(copy_arr, &quicksort_dual);
    printf("--------------------------------------------------------\n\n");

    printf("--------------------------------------------------------\n");
    printf("Sorted Numbers: %d million. \n\n", SIZE / 1000000);
    fill_sorted(data, SIZE);
    copy(data, copy_arr, SIZE);
    printf("Single Pivot: \n");
    run_sort_with_time(copy_arr, &quicksort_single);
    copy(data, copy_arr, SIZE);
    printf("\nDual Pivot: \n");
    run_sort_with_time(copy_arr, &quicksort_dual);
    printf("--------------------------------------------------------\n\n");

    printf("--------------------------------------------------------\n");
    printf("Reverse sorted Numbers: %d million. \n\n", SIZE / 1000000);
    fill_reverse_sorted(data, SIZE);
    copy(data, copy_arr, SIZE);
    printf("Single Pivot: \n");
    run_sort_with_time(copy_arr, &quicksort_single);
    copy(data, copy_arr, SIZE);
    printf("\nDual Pivot: \n");
    run_sort_with_time(copy_arr, &quicksort_dual);
    printf("--------------------------------------------------------\n\n");

    printf("--------------------------------------------------------\n");
    printf("Half random Numbers: %d million. \n\n", SIZE / 1000000);
    fill_half_random(data, SIZE);
    copy(data, copy_arr, SIZE);
    printf("Single Pivot: \n");
    run_sort_with_time(copy_arr, &quicksort_single);
    copy(data, copy_arr, SIZE);
    printf("\nDual Pivot: \n");
    run_sort_with_time(copy_arr, &quicksort_dual);
    printf("--------------------------------------------------------\n\n");

    printf("--------------------------------------------------------\n");
    printf("Low range Numbers: %d million. \n\n", SIZE / 1000000);
    fill_small_range(data, SIZE);
    copy(data, copy_arr, SIZE);
    printf("Single Pivot: \n");
    run_sort_with_time(copy_arr, &quicksort_single);
    copy(data, copy_arr, SIZE);
    printf("\nDual Pivot: \n");
    run_sort_with_time(copy_arr, &quicksort_dual);
    printf("--------------------------------------------------------\n\n");
    free(data);
    free(copy_arr);
    return 0;
}