#ifndef INSERTIONSORT_H
#define INSERTIONSORT_H

void insertion_sort(int *left, int *right)
{
    for (int *i = left + 1; i <= right; i++) {
        int change = *i;
        int *j = i - 1;
        while (change < *j) {
            *(j + 1) = *j;
            j -= 1;
        }
        *(j + 1) = change;
    }
}

#endif