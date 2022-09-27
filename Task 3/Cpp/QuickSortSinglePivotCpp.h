#ifndef QUICKSORTSINGLEPIVOTCPP_H
#define QUICKSORTSINGLEPIVOTCPP_H
#include "../InsertionSort.h"
#include "../QuickSortMacros.h"

#define median3sort(left, mid, right) {             \
        if (*left > *mid) {swap(*left, *mid);}      \
        if (*mid > *right) {                        \
            swap(*mid, *right);                     \
            if(*left > *mid) swap(*left, *mid);     \
        }                                           \
    }                                               \

class QuickSortSingle {
    public:
        QuickSortSingle();
        ~QuickSortSingle();
        void partition(int* left_0, int* right_0, int** pivot);
        void quicksort(int *left, int *right);
};

QuickSortSingle::QuickSortSingle()
{}

QuickSortSingle::~QuickSortSingle()
{
    delete this;
}

void QuickSortSingle::partition(int *left_0, int *right_0, int **pivot)
{
    int *left = left_0 + 1;
    int *right = right_0;
    int* mid = left_0 + divide_2(right_0 - left_0);
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
    *right = piv; *pivot = right;
}

void QuickSortSingle::quicksort(int *left, int *right)
{
    if (right - left >= RUN_INSERTION) {
        int *piv;
        partition(left, right, &piv);
        quicksort(piv + 1, right);
        quicksort(left, piv - 1);
    } else {
        insertion_sort(left, right);
    }
}

#endif