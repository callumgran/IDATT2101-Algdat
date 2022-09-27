#ifndef QUICKSORTDUALPIVOTCPP_H
#define QUICKSORTDUALPIVOTCPP_H
#include <iostream>
#include "../InsertionSort.h"
#include "../QuickSortMacros.h"

using namespace std;

#define dual_mid_point(left_0, right_0) {                       \
        swap(*left_0, *(left_0 + (right_0 - left_0) / 3));      \
        swap(*right_0, *(right_0 - (right_0 - left_0) / 3));    \
        if (*left_0 > *right_0) swap(*left_0, *right_0);        \
    }                                                           \

class QuickSortDual {
    public:
        QuickSortDual();
        ~QuickSortDual();
        void partition(int* left_0, int* right_0, int** lp, int** hp);
        void quicksort(int *left, int *right);
};

QuickSortDual::QuickSortDual()
{}

QuickSortDual::~QuickSortDual()
{
    delete this;
}

void QuickSortDual::quicksort(int *left, int *right)
{
    if (right - left >= RUN_INSERTION) {
        int *lp, *hp;
        partition(left, right, &lp, &hp);
        quicksort(left, lp - 1);
        quicksort(lp + 1, hp - 1);
        quicksort(hp + 1, right);
    } else {
        insertion_sort(left, right);
    }
}

void QuickSortDual::partition(int* left_0, int* right_0, int** lp, int** hp)
{
    dual_mid_point(left_0, right_0);
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

#endif