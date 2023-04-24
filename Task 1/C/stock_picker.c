#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define SIZE (1 * 1000000)

struct profit_info_t {
    uint_fast32_t index_min;
    uint_fast32_t index_max; 
    int_fast32_t min_val;
    int_fast32_t max_val;
    int_fast32_t profit;
};

void create_values(int* data, const int len)
{
    for (int i = 0; i < len; i++)
        data[i] = rand();
}

void stock_picker(const int * data, struct profit_info_t *profits)
{
    uint_fast32_t i = 1;
    int_fast32_t curr_val = *(data);
    int_fast32_t min_temp = curr_val;
    int_fast32_t max_temp = curr_val;
    uint_fast32_t index_max_temp = 1;
    uint_fast32_t index_min_temp = 1;
    int_fast32_t temp_dif = 0;        

    for (int i = 0; i < SIZE; i++) {
        if(curr_val > max_temp) { 
            max_temp = curr_val; 
            temp_dif = max_temp - min_temp; 
            index_max_temp = i; 
            if (temp_dif > profits->profit) { 
                profits->profit = temp_dif; 
                profits->min_val = min_temp; 
                profits->max_val = max_temp; 
                profits->index_min = index_min_temp; 
                profits->index_max = index_max_temp; 
            }
        } else if (curr_val < min_temp) { 
            min_temp = curr_val; 
            max_temp = min_temp; 
            index_min_temp = i; 
        }
        curr_val = curr_val + *(data + i);
    }
}

int main() 
{
    struct profit_info_t profits;
    int *restrict data = (int *)(malloc(SIZE * sizeof(int)));
    clock_t t;
    srand(time(NULL));
    create_values(data, SIZE);
    t = clock();
    stock_picker(data, &profits);
    t = clock() - t;

    double time_taken = ((double)t)/(CLOCKS_PER_SEC/1000000);
    
    printf("Buy day: %ld \n", profits.index_min);
    printf("Buy value: %ld \n", profits.min_val);
    printf("Sell day: %ld \n", profits.index_max);
    printf("Sell value: %ld \n", profits.max_val);
    printf("Profit: %ld \n", profits.profit);
    printf("Time used: %f µs \n", time_taken);

    free(data);
    return 0;
}