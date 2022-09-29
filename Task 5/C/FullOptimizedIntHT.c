#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define SIZE (10 * 1000000) // x * million
#define TABLE_SIZE (13499939)
#define double_hash_probe(i, s) (i + s<<2) % TABLE_SIZE
#define HASH_1(k) k % TABLE_SIZE
#define HASH_2(k) k % (TABLE_SIZE - 1) + 1

typedef struct ht_t
{
    int *keys;
    size_t size;
    size_t max;
} ht;

struct ht_t *ht_malloc(size_t max)
{
    struct ht_t *ht = (struct ht_t *)(malloc(sizeof(struct ht_t)));
    ht->max = max;
    ht->size = 0;
    ht->keys = (int *)(malloc(max * sizeof(int)));
    for (int i = 0; i < max; i++)
        ht->keys[i] = 0;
    return ht;
}

int ht_insert_double_hashed_item(ht *ht, int key)
{
    int index = HASH_1(key);
    
    ht->size++;
    if (ht->size > TABLE_SIZE)
    {
        ht->size--;
        return -1;
    }
    else if (ht->keys[index] == 0)
    {
        ht->keys[index] = key;
        return 0;
    }
    else
    {
        int space = HASH_2(key),
            cols = 1;
        if (ht->keys[space] == 0)
        {
            ht->keys[space] = key;
            return cols;
        }
        else
        {
            cols++;
            index = double_hash_probe(index, space);
            while (ht->keys[index] != 0)
            {
                cols++;
                index = double_hash_probe(index, space);
            }
            ht->keys[index] = key;
            return cols;
        }
    }
}

int ht_find_double_hashed_item(struct ht_t *ht, int key)
{
    int index = HASH_1(key);
    if (ht->keys[index] == key)
        return ht->keys[index];
    else
    {
        int space = HASH_2(key);
        if (ht->keys[space] == key)
            return ht->keys[space];
        else
        {
            index = double_hash_probe(index, space);
            if (ht->keys[index] == key)
                return ht->keys[space];
            else
            {
                while (ht->keys[index] != 0)
                {
                    index = double_hash_probe(index, space);
                    if (ht->keys[index] == key)
                        return ht->keys[index];
                }
            }
            return -1;
        }
    }
}

void fill_random(int *data, int len)
{
    srand(time(NULL));
    for (int i = 0; i < len; i++)
        *(data + i) = rand();
}

int main()
{
    clock_t t;
    ht *ht = ht_malloc(TABLE_SIZE);
    int *data = (int *)(malloc(SIZE * sizeof(int))),
        collisions = 0;
    fill_random(data, SIZE);
    t = clock();
    for (int i = 0; i < SIZE; i++)
        collisions += ht_insert_double_hashed_item(ht, data[i]);
    t = clock() - t;
    double time_taken_insert = ((double)t) / (CLOCKS_PER_SEC / 1000);
    printf("Time taken to insert items: %fms\n", time_taken_insert);
    t = clock();
    for (int i = 0; i < SIZE; i++)
        ht_find_double_hashed_item(ht, data[i]);
    t = clock() - t;
    double time_taken_find = ((double)t) / (CLOCKS_PER_SEC / 1000);
    printf("Time taken to find items: %fms\n", time_taken_find);
    printf("Collisions: %d\n", collisions);
    printf("Collisions per element: %f\n", (double)collisions / SIZE);
    printf("Load factor: %f\n", (double)SIZE/TABLE_SIZE);
    free(ht);
    return 0;
}