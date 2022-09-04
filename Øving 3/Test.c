#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

#define SIZE (10 * 1000000)
#define RUN_INSERTION 70
#define swap(a, b) { int c = a; a = b; b = c; }

int data[SIZE];

double RAND(double min, double max)
{
    return (double)rand()/(double)RAND_MAX * (max - min) + min;
}

void sum_array(int* data, int *sum, int len);

// void swap(int a, int b)
// {
//     int tmp = a; 
//     a = b; 
//     b = tmp;
// }

void insertion_sort(int *left, int *right) 
{
    for (int* i = left + 1; i <= right; i++) {
        int change = *i;
        int* j = i - 1;
        while (change < *j) {
            *(j + 1) = *j;
            j -= 1;
        }
        *(j + 1) = change;
    }
}

int* partition_dual(int* left, int* right, int** lp)
{
    if (*left > *right) swap(*left, *right);
    int* j = left + 1;
    int* g = right - 1, *k = left + 1, p = *left, q = *right;
    while (k <= g) {
        if (*k < p) {
            swap(*k, *j);
            j++;
        }
        else if (*k >= q) {
            while (*g > q && k < g) {g--;}
            swap(*k, *g);
            g--;
            if (*k < p) {
                swap(*k, *j);
                j++;
            }
        }
        k++;
    }
    j--;
    g++;
    swap(*left, *j);
    swap(*right, *g);
    *lp = j;
    return g;
}

void quicksort(int* left, int* right) {
    if (right - left >= RUN_INSERTION) {
        int* lp;
        int* hp = partition_dual(left, right, &lp);
        quicksort(left, lp - 1);
        quicksort(lp + 1, hp - 1);
        quicksort(hp + 1, right);
    } else {
        insertion_sort(left, right);
    }
}

void fill(int* data, int len) 
{
    for (int i = 0; i < len; i++) {
        data[i] = rand();
    }
}


void test_sorted(int* data, int len) 
{
    for (int i = 1; i < len; i++) {
        if (data[i] < data[i - 1]) {
            printf("ERROR! Array is not sorted.\n");
            printf("%d and %d\n", data[i], data[i - 1]);
            break;
        }
        if (i == len - 1) printf("SUCCESS! Array is sorted.\n");
    }
}

void sum_array(int* data, int *sum, int len) 
{
    *sum = 0;
    for (int i = 0; i < len; i++) {
        *sum += data[i];
    }
}

void test_values(int sum1, int sum2)
{
    if(sum1 == sum2) printf("SUCCESS! The sum before sorting: %d is equal to the sum after sorting %d\n", sum1, sum2);
    else printf("ERROR! The sum before sorting: %d is not equal to the sum after sorting %d\n", sum1, sum2);
}

int main()
{
    double elapsed;
    struct timespec start, finish;
    int sum_before, sum_after;
    fill(data, SIZE);
    sum_array(data, &sum_before, SIZE);
    printf("Started sorting %d million numbers. \n", SIZE/1000000);
    clock_gettime(CLOCK_MONOTONIC, &start);
    quicksort(data, data + SIZE - 1);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Quicksort used %fms for %d million numbers. \n", elapsed*1000, SIZE/1000000);
    test_sorted(data, SIZE);
    sum_array(data, &sum_after, SIZE);
    test_values(sum_before, sum_after);
    return 0;
}