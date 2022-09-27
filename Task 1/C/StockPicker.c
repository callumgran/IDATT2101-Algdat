#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE (8 * 1000000)

typedef struct {
    int index_min, 
        index_max, 
        min_val,
        max_val,
        profit;
} ProfitInfo;

double RAND(double min, double max)
{
    return (double)rand()/(double)RAND_MAX * (max - min) + min;
}

void create_values(int* data, int len)
{
    for (int i = 0; i < len; i++) {
        data[i] = (int) RAND(-10, 10);
    }
}

void stockPicker(int* data, ProfitInfo* profits)
{
    int i = 1,
        curr_val = *(data),
        min_temp = curr_val,
        max_temp = curr_val,
        index_max_temp = 1,
        index_min_temp = 1,
        temp_dif = 0;
    
    while (1)
    {
        if(curr_val > max_temp) 
        { 
            max_temp = curr_val; 
            temp_dif = max_temp - min_temp; 
            index_max_temp = i; 
            if (temp_dif > profits->profit) 
            { 
                profits->profit = temp_dif; 
                profits->min_val = min_temp; 
                profits->max_val = max_temp; 
                profits->index_min = index_min_temp; 
                profits->index_max = index_max_temp; 
            }
        } else if (curr_val < min_temp) 
        { 
            min_temp = curr_val; 
            max_temp = min_temp; 
            index_min_temp = i; 
        }
        if (i == SIZE) break;
        i++;
        curr_val = curr_val + *(data + i);
    }
}

int main() 
{
    ProfitInfo* profits = (ProfitInfo*)(malloc(sizeof(ProfitInfo)));
    int* data = (int*)(malloc(SIZE * sizeof(int)));
    clock_t t;
    srand(time(NULL));
    create_values(data, SIZE);
    t = clock();
    stockPicker(data, profits);
    t = clock() - t;
    double time_taken = ((double)t)/(CLOCKS_PER_SEC/1000000);
    printf("Buy day: %d \n", profits->index_min);
    printf("Buy value: %d \n", profits->min_val);
    printf("Sell day: %d \n", profits->index_max);
    printf("Sell value: %d \n", profits->max_val);
    printf("Profit: %d \n", profits->profit);
    printf("Time used: %f µs \n", time_taken);
    free(profits);
    free(data);
    return 0;
}