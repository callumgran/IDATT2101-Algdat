#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

#define SIZE (100 * 1000000)
#define RUN_INSERTION 70
#define swap(a, b) { int c = a; a = b; b = c; }
#define median3sort(left, mid, right)       \
    int* mid = left_0 + (right_0 - left_0) / 2; \
    if (*left > *mid) {swap(*left, *mid);}  \
    if (*mid > *right) {                    \
        swap(*mid, *right);                 \
        if(*left > *mid) swap(*left, *mid); \
    }   

int max_threads;
int n_threads;
int data[SIZE];

pthread_mutex_t t_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t t_cond = PTHREAD_COND_INITIALIZER;

void quicksort(int* left, int* right);

void* sort_thread(void* arg)
{
    int** partition = (int**)arg;
    quicksort(partition[0], partition[1]);
    free(arg);
    pthread_mutex_lock(&t_mutex);
    n_threads -= 1;
    if (n_threads == 0) pthread_cond_signal(&t_cond);
    pthread_mutex_unlock(&t_mutex);
    return NULL;
}

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

// int* median3sort(int* left, int* right)
// {
//     int* mid = left + (right - left) / 2;
//     if (*left > *mid) swap(*left, *mid);
//     if (*mid > *right) {
//         swap(*mid, *right);
//         if(*left > *mid) swap(*left, *mid);
//     }
//     return mid;
// }

int* partition_standard(int* left_0, int* right_0) 
{
    int* left = left_0 + 1; 
    int* right = right_0;
    median3sort(left_0, mid, right_0);
    int piv = *mid;
    *mid = *left;
    *left = piv;
    swap(*mid, *(right_0 - 1));
    while (1) {
        do left++; while (*left < piv);
        do right--; while (*right > piv);
        if (left >= right) break;
        swap(*left, *right);
    }
    *(left_0 + 1) = *right;
    *right = piv;
    return right;
}

void quicksort(int* left, int* right) {
    if (right - left >= RUN_INSERTION) {
        int* piv = partition_standard(left, right);
        if (right - left > 300000 && n_threads < max_threads) {
            pthread_t thread;
            int** param = malloc(2 * sizeof(int*));
            param[0] = piv + 1;
            param[1] = right;
            pthread_mutex_lock(&t_mutex);
            n_threads += 1;
            pthread_mutex_unlock(&t_mutex);
            pthread_create(&thread, NULL, sort_thread, param);
            param = malloc(2 * sizeof(int*));
            param[0] = left;
            param[1] = piv - 1;
            pthread_mutex_lock(&t_mutex);
            n_threads += 1;
            pthread_mutex_unlock(&t_mutex);
            pthread_create(&thread, NULL, sort_thread, param);
        } else {
        quicksort(piv + 1, right);
        quicksort(left, piv - 1);
        }
    } else {
        insertion_sort(left, right);
    }
}

void sort(int* data, int len) 
{
    int n_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    if (n_cpus > 0) max_threads = n_cpus * 2;
    else max_threads = 4;

    pthread_t thread;
    int** param = malloc(2 * sizeof(int*));
    param[0] = data;
    param[1] = data + len - 1;
    n_threads = 1;
    pthread_create(&thread, NULL, sort_thread, param);
    pthread_mutex_lock(&t_mutex);
    pthread_cond_wait(&t_cond, &t_mutex);
    pthread_mutex_unlock(&t_mutex);
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
    sort(data, SIZE);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Quicksort used %fms for %d million numbers. \n", elapsed*1000, SIZE/1000000);
    test_sorted(data, SIZE);
    sum_array(data, &sum_after, SIZE);
    test_values(sum_before, sum_after);
    return 0;
}