#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

#define SIZE (5 * 1000000)
#define RUN_INSERTION 70
#define swap(a, b) { int c = a; a = b; b = c; }

#define dualmidpoint(left_0, right_0)                  \
    swap(*left_0, *(left_0 + (right_0 - left_0) / 3));  \
    swap(*right_0, *(right_0 - (right_0 - left_0) / 3));\
    if (*left_0 > *right_0) swap(*left_0, *right_0);    \

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

void sum_array(int* data, int *sum, int len);

void* sort_thread(void* arg)
{
    int** partition = (int**)arg;
    quicksort(partition[0], partition[1]);
    free(arg);
    pthread_mutex_lock(&t_mutex);
    n_threads -= 1;
    if (n_threads == 0) pthread_cond_signal(&t_cond);
    pthread_mutex_unlock(&t_mutex);
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

void partition_dual(int* left_0, int* right_0, int** lp, int** hp)
{
    dualmidpoint(left_0, right_0);
    int* left = left_0 + 1;
    int* right = right_0 - 1, *left_1 = left_0 + 1, left_0_val = *left_0, right_0_val = *right_0;
    while (left_1 <= right) {
        if (*left_1 < left_0_val) {                     
            swap(*left_1, *left);                       
            left++;                                     
        } else if (*left_1 >= right_0_val) {
            while (*right > right_0_val && left_1 < right) {right--;}
            swap(*left_1, *right);
            right--;
            if (*left_1 < left_0_val) {                     
                swap(*left_1, *left);                       
                left++;                                     
            }                                               
        }
        left_1++;
    }
    left--;
    right++;
    swap(*left_0, *left); swap(*right_0, *right);
    *lp = left; *hp = right;
}

void quicksort(int* left, int* right) {
    if (right - left >= RUN_INSERTION) {
        int* lp, * hp;
        partition_dual(left, right, &lp, &hp);
        if (right - left > 300000 && n_threads < max_threads) {
            pthread_t thread;
            int** param = malloc(2 * sizeof(int*));
            param[0] = left;
            param[1] = lp - 1;
            pthread_mutex_lock(&t_mutex);
            n_threads += 1;
            pthread_mutex_unlock(&t_mutex);
            pthread_create(&thread, NULL, sort_thread, param);
            param = malloc(2 * sizeof(int*));
            param[0] = lp + 1;
            param[1] = hp - 1;
            pthread_mutex_lock(&t_mutex);
            n_threads += 1;
            pthread_mutex_unlock(&t_mutex);
            pthread_create(&thread, NULL, sort_thread, param);
            param = malloc(2 * sizeof(int*));
            param[0] = hp + 1;
            param[1] = right;
            pthread_mutex_lock(&t_mutex);
            n_threads += 1;
            pthread_mutex_unlock(&t_mutex);
            pthread_create(&thread, NULL, sort_thread, param);
        } else {
        quicksort(left, lp - 1);
        quicksort(lp + 1, hp - 1);
        quicksort(hp + 1, right);
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

void fill_random(int* data, int len) 
{
    for (int i = 0; i < len; i++) {
        data[i] = rand();
    }
}

void fill_sorted(int* data, int len) 
{
    for (int i = 0; i < len; i++) {
        data[i] = i;
    }
}

void fill_reverse_sorted(int* data, int len) 
{
    for (int i = 0; i < len; i++) {
        data[i] = len - i;
    }
}

void fill_half_duplicates(int* data, int len) 
{
    int j = len / 2;
    int k = len / 3;
    for (int i = 0; i < len; i++) {
        if (i % 2 == 0) {
            data[i] = rand();
        } else if (i % 3 == 0) {
            data[i] = k;
        } else {
            data[i] = j;
        }
    }
}

void fill_small_range(int* data, int len)
{
    for (int i = 0; i < len; i++) {
        data[i] = RAND(0, 5);
    }
}

void test_sorted(int* data, int len) 
{
    for (int i = 1; i < len; i++) {
        if (data[i] < data[i - 1]) {
            printf("ERROR! Array is not sorted.\n");
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

void run_sort_with_time(int* data, void (*func)(int*, int))
{
    double elapsed;
    struct timespec start, finish;
    int sum_before, sum_after;
    func(data, SIZE);
    sum_array(data, &sum_before, SIZE);
    printf("Started sorting\n");
    clock_gettime(CLOCK_MONOTONIC, &start);
    sort(data, SIZE);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Quicksort used %fms for %d million numbers. \n", elapsed*1000, SIZE/1000000);
    test_sorted(data, SIZE);
    sum_array(data, &sum_after, SIZE);
    test_values(sum_before, sum_after);
}

int main()
{
    srand(time(NULL));
    printf("--------------------------------------------------------\n");
    printf("Random Numbers: %d million. \n", SIZE/1000000);
    run_sort_with_time(data, &fill_random);
    printf("--------------------------------------------------------\n\n");

    printf("--------------------------------------------------------\n");
    printf("Sorted Numbers: %d million. \n", SIZE/1000000);
    run_sort_with_time(data, &fill_sorted);
    printf("--------------------------------------------------------\n\n");

    printf("--------------------------------------------------------\n");
    printf("Reverse sorted Numbers: %d million. \n", SIZE/1000000);
    run_sort_with_time(data, &fill_reverse_sorted);
    printf("--------------------------------------------------------\n\n");

    printf("--------------------------------------------------------\n");
    printf("Half duplicates Numbers: %d million. \n", SIZE/1000000);
    run_sort_with_time(data, &fill_half_duplicates);
    printf("--------------------------------------------------------\n\n");

    printf("--------------------------------------------------------\n");
    printf("Low range Numbers: %d million. \n", SIZE/1000000);
    run_sort_with_time(data, &fill_small_range);
    printf("--------------------------------------------------------\n\n");
    return 0;
}