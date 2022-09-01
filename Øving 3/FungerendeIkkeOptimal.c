#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

#define SIZE (1 * 100000)
//#define swap(a, b) { int c = a; a = b; b = c; }
#define min(a, b) ((a) < (b) ? (a) : (b))

int max_threads;
int n_threads;
int data[SIZE];

pthread_mutex_t t_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t t_cond = PTHREAD_COND_INITIALIZER;

void quicksort(int* t, int left, int right);

void* sort_thread(void* arg)
{
    int** partition = (int**)arg;
    quicksort(partition[0], 0, *partition[1]);
    free(arg);
    pthread_mutex_lock(&t_mutex);
    n_threads -= 1;
    if (n_threads == 0) pthread_cond_signal(&t_cond);
    pthread_mutex_unlock(&t_mutex);
    return NULL;
}

double RAND(double min, double max)
{
    return (double)rand()/(double)RAND_MAX * (max - min) + min;
}

void swap(int *i, int *j)
{
    int tmp = *j;
    *j = *i;
    *i = tmp;
}

int median_3_sort(int* t, int left, int right)
{
    int mid = (left + right) / 2;    
    if (t[left] > t[mid]) swap(&t[left], &t[mid]);
    if (t[mid] > t[right]) {
        swap(&t[mid], &t[right]);
        if (t[left] > t[mid]) swap(&t[left], &t[mid]);
    }
    return mid;
}

void insertion_sort(int* left, int* right) 
{
    for (int* ptri = left + 1; ptri <= right; ptri++) {
        if (*ptri < *left) {
            swap(ptri, left);
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

int partition_standard(int* t, int left, int right) 
{
    int il, ir;
    int mid = median_3_sort(t, left, right);
    int partVal = t[mid];
    swap(&t[mid], &t[right - 1]);
    for(il = left; ir = right - 1;) {
        while(t[++il] < partVal);
        while (t[--ir] > partVal);
        if (il >= ir) break;
        swap(&t[il], &t[ir]);
    }
    swap(&t[il], &t[right - 1]);
    return il;
}

void quicksort(int* t, int left, int right)
{
    if (right - left >= 35) {
        int split_pos = partition_standard(t, left, right);
        quicksort(t, left, split_pos - 1);
        quicksort(t, split_pos + 1, right);
    } else insertion_sort(&t[left], &t[right]);
}

void sort(int* data, int len) 
{
    int n_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    if (n_cpus > 0) max_threads = n_cpus * 2;
    else max_threads = 8;
    int size = SIZE - 1;
    pthread_t thread;
    int** param = malloc(2 * sizeof(int*));
    param[0] = data;
    param[1] = &size;
    n_threads = 1;
    pthread_create(&thread, NULL, sort_thread, param);

    pthread_mutex_lock(&t_mutex);
    pthread_cond_wait(&t_cond, &t_mutex);
    pthread_mutex_unlock(&t_mutex);
}


void fill(int* data, int len) 
{
    for (int i = 0; i < len; i++) {
        data[i] = (int)RAND(0, 200000);
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
    for (int i = 0; i < len; i++) {
        // printf("%d\n", data[i]);
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
    clock_t t;
    int sum_before = 0, sum_after = 0;
    fill(data, SIZE);
    sum_array(data, &sum_before, SIZE);
    t = clock();
    sort(data, SIZE);
    printf("Quicksort used %.3f ms for %d numbers. \n", (double)((clock() - t)/1000), SIZE/1);
    test_sorted(data, SIZE);
    sum_array(data, &sum_after, SIZE);
    test_values(sum_before, sum_after);
    return 0;
}