#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

#define SIZE (100)
#define swap(a, b) { int c = a; a = b; b = c; }
#define fast3sort(a, b, c)            \
    if (b < a) {                        \
        if (c < a) {                    \
            if (c < b) { swap(a, c); }  \
            else { int h = a; a = b; b = c; c = h;} \
        }                               \
        else { swap(a, b); }            \
    }                                   \
    else {                              \
        if (c < b) {                    \
            if (c < a) { int h = c; c = b; b = a; a = h;} \
            else { swap(b, c); }        \
        }                               \
    }                                   \

int max_threads;
int n_threads;
int data[SIZE];

pthread_mutex_t t_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t t_cond = PTHREAD_COND_INITIALIZER;

void quicksort(int* left, int* right);

double RAND(double min, double max)
{
    return (double)rand()/(double)RAND_MAX * (max - min) + min;
}

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

void insertion_sort(int* left, int* right) 
{
    for (int* ptri = left + 1; ptri <= right; ptri++) {
        if (*ptri < *left) {
            swap(*ptri, *left);
        }
    }
    for (int* ptri = left + 2; ptri <= right; ptri++) {
        int h = *ptri;
        int* ptrj = ptri - 1;
        while (h < *ptrj) {
            *(ptrj + 1) = *ptrj;
            ptrj -= 1;
        }
        *(ptrj + 1) = h;
    }
}

void partition_standard(int* left_0, int* right_0, int** left_1, int** right_1, int** left_2, int** right_2) 
{
    int* left = left_0 + 1;
    int* right = right_0;

    int* mid = left_0 + (right_0 - left_0) / 2;    
    int piv = *mid;
    *mid = *left;
    median3sort(*left_0, piv, *right_0);
    *left = piv;

    while (1) {
        do left += 1; while(*left < piv);
        do right -= 1; while (*right > piv);
        if (left >= right) break;
        swap(*left, *right);
    }
    *(left_0 + 1) = *right;
    *right = piv;

    if (right < mid) {
        *left_1 = left_0; *right_1 = right - 1;
        *left_2 = right + 1; *right_2 = right_0;
    }
    else {
        *left_1 = right + 1; *right_1 = right_0;
        *left_2 = left_0; *right_2 = right - 1;
    }
}

void quicksort(int* left, int* right) {

    while (right - left >= 50) {
        int *l, *r;
        partition_standard(left, right, &l, &r, &left, &right);

        if (right - left > 100000 && n_threads < max_threads) {
            // start a new thread - max_threads is a soft limit
            pthread_t thread;
            int** param = malloc(2 * sizeof(int*));
            param[0] = left;
            param[1] = right;
            pthread_mutex_lock(&t_mutex);
            n_threads += 1;
            pthread_mutex_unlock(&t_mutex);
            pthread_create(&thread, NULL, sort_thread, param);
            left = l;
            right = r;
        }
        else {
            quicksort(l, r);
        }
    }
    insertion_sort(left, right);
}


void sort(int* data, int len) 
{
    int n_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    if (n_cpus > 0) max_threads = n_cpus * 2;
    else max_threads = 8;

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
        // data[i] = (int)RAND(0, 10000);
        data[i] = rand();
    }
}

void test_sorted(int* data, int len) 
{
    for (int i = 1; i < len; i++) {
        if (data[i] < data[i - 1]) {
            printf("ERROR! Array is not sorted.\n");
            break;
        }
    }
    printf("SUCCESS! Array is sorted.\n");
}

void sum_array(int* data, int *sum, int len) 
{
    *sum = 0;
    for (int i = 0; i < len; i++) {
        *sum += data[i];
        // printf("%d\n", data[i]);
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
    printf("Started sorting %d numbers\n", SIZE);
    clock_gettime(CLOCK_MONOTONIC, &start);
    sort(data, SIZE);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Quicksort used %f s for %d numbers. \n", elapsed, SIZE/1);
    test_sorted(data, SIZE);
    sum_array(data, &sum_after, SIZE);
    test_values(sum_before, sum_after);
    return 0;
}