#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE (8 * 1000000)

int data[SIZE];
int indexMaxTemp = 1;
int indexMaxFinal = 1;
int indexMinTemp = 1;
int indexMinFinal = 1;
int tempDif = 0;
int maxDif = 0;
int minFinal = 0;
int maxFinal = 0; 

double RAND(double min, double max)
{
    return (double)rand()/(double)RAND_MAX * (max - min) + min;
}

void fill(int* data, int len)
{
    for (int i = 0; i < len; i++) {
        data[i] = (int) RAND(-10, 10);
    }
}

void stockPicker(int* stockChanges)
{
    int currVal = *stockChanges;
    int minTemp = currVal; 
    int maxTemp = currVal; 
    for (int i = 1; i <= SIZE; i++) {
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
        currVal = currVal + *(stockChanges + i + 1);
    }
}

int main() 
{
    clock_t t;
    srand(time(NULL));
    fill(data, SIZE);
    t = clock();
    stockPicker(data);
    t = clock() - t;
    double time_taken = ((double)t)/(CLOCKS_PER_SEC/1000);
    printf("Buy day: %d \n", indexMinFinal);
    printf("Buy value: %d \n", minFinal);
    printf("Sell day: %d \n", indexMaxFinal);
    printf("Sell value: %d \n", maxFinal);
    printf("Profit: %d \n", maxDif);
    printf("Time used: %f ms \n", time_taken);
    return 0;
}