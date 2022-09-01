#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct vector {
    int size;
    int arr[];
} vector;

double RAND(double min, double max)
{
    return (double)rand()/(double)RAND_MAX * (max - min) + min;
}

int main() 
{
    int n = 100000000;
    srand(time(NULL));
    vector *stockChanges = (vector *)malloc(sizeof(vector *) + n * sizeof(int));
    for (int i = 0; i < n; i++) {
        stockChanges->arr + i;
        stockChanges->arr[i] = RAND(-10, 10);
    }
    clock_t t;
    t = clock();
    vector *stockPrice = (vector *)malloc(sizeof(vector *) + (n + 1) * sizeof(int));
    stockPrice->arr[0] = 0;
    for(int i = 0; i < n; i++){
        stockPrice->arr[i + 1] = stockPrice->arr[i] + stockChanges->arr[i];
    }
    free(stockChanges);
    int indexMaxTemp = 0;
    int indexMaxFinal = 0;
    int indexMinTemp = 0;
    int indexMinFinal = 0;
    int maxTemp = 0;
    int maxFinal = 0;
    int minTemp = 0;
    int minFinal = 0;
    int difTemp = 0;
    int difFinal = 0;
    for (int i = 0; i < (n + 1); i++){
        if(stockPrice->arr[i] > maxTemp){
            maxTemp = stockPrice->arr[i];
            indexMaxTemp = i;
        }
        if (stockPrice->arr[i] < minTemp){
            minTemp = stockPrice->arr[i];
            maxTemp = minTemp;
            indexMinTemp = i;
        }
        difTemp = maxTemp - minTemp;

        if (difTemp>difFinal){
            difFinal = difTemp;
            minFinal = minTemp;
            maxFinal = maxTemp;
            indexMinFinal = indexMinTemp;
            indexMaxFinal = indexMaxTemp;
        }
    }
    t = clock() - t;
    double time_taken = ((double)t)/(CLOCKS_PER_SEC/1000);
    printf("Table of values: ");
    // for (int i = 0; i < (n + 1); i++)
    //     printf("%d \n", stockPrice->arr[i]);
    printf("Buy value: %d \n", minFinal);
    printf("Sell value: %d \n", maxFinal);
    printf("Buy day: %d \n", indexMinFinal);
    printf("Sell day: %d \n", indexMaxFinal);
    printf("Profit: %d \n", difFinal);
    printf("Time used: %f ms \n", time_taken);
    return 0;
    free(stockPrice);
}