#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "HashTable.h"

#define SIZE (10 * 1000000) // x * million

#define FIBONACCI 11400714819323198485llu


int find_power_of_2(size_t n)
{
    if (n == 1) return 0;
    return find_power_of_2(n>>1) + 1;
}

size_t fib_hash(const void *val, size_t val_size, size_t max)
{
    size_t key = 1;

    for (int i = 0; i < val_size; i++)
        key += key * ((int8_t*)val)[i];
    
    return (key * FIBONACCI) >> (64 - find_power_of_2(max));
}

size_t binary_hash(const void *val, size_t val_size, size_t max)
{
    size_t key = 1;

    for (int i = 0; i < val_size; i++)
        key += key * ((int8_t*)val)[i];
    
    return key & (max - 1);
}


int main()
{

    return 0;
}