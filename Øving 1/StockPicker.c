#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE (8 * 1000000)

typedef struct vector {
    int size;
    int arr[];
} vector;


//n is the number of price-changes and the size of the array.
clock_t t;
int indexMaxTemp = 1;
int indexMaxFinal = 1;
int indexMinTemp = 1;
int indexMinFinal = 1;
int maxTemp = 0;
int maxFinal = 0;
int minTemp = 0;
int minFinal = 0;
int tempDif = 0;
int maxDif = 0;
int currVal = 0;
int* data[SIZE];

double RAND(double min, double max)
{
    return (double)rand()/(double)RAND_MAX * (max - min) + min;
}

void fill()
{
    for (int i = 0; i < SIZE; i++) {
        
    }
}

void stockPicker(vector *stockChanges)
{
    currVal = stockChanges->arr[0];
    maxTemp = currVal; 
    maxFinal = currVal; 
    minTemp = currVal; 
    minFinal = currVal; 
    int loopCounter = SIZE + 1;
    for (int i = 1; i < loopCounter; i++) {
        if(currVal > maxTemp) { 
            maxTemp = currVal; 
            tempDif = maxTemp - minTemp; 
            indexMaxTemp = i; 
            if (tempDif > maxDif) { 
                maxDif = tempDif; 
                minFinal = minTemp; 
                maxFinal = maxTemp; 
                indexMinFinal = indexMinTemp; 
                indexMaxFinal = indexMaxTemp; 
            }
        } else if (currVal < minTemp) { 
            minTemp = currVal; 
            maxTemp = minTemp; 
            indexMinTemp = i; 
        }
        currVal = currVal + stockChanges->arr[i + 1];
    }
}

int main() 
{
    srand(time(NULL));
    vector *stockChanges = (vector *)malloc(sizeof(vector *) + SIZE * sizeof(int));
    for (int i = 0; i < SIZE; i++) {
        stockChanges->arr + i;
        stockChanges->arr[i] = RAND(-10, 10);
    }
    t = clock();
    stockPicker(stockChanges);
    t = clock() - t;
    free(stockChanges);
    double time_taken = ((double)t)/(CLOCKS_PER_SEC/1000);
    printf("Buy day: %d \n", indexMinFinal);
    printf("Buy value: %d \n", minFinal);
    printf("Sell day: %d \n", indexMaxFinal);
    printf("Sell value: %d \n", maxFinal);
    printf("Profit: %d \n", maxDif);
    printf("Time used: %f ms \n", time_taken);
    return 0;
}