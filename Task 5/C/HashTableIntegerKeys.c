#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define SIZE (10 * 1000000) // x * million
#define TABLE_SIZE (13499939)

#define double_hash_probe(i, s) (i + s<<2) % TABLE_SIZE
#define HASH_1(k) k % TABLE_SIZE
#define HASH_2(k) k % (TABLE_SIZE-1) + 1

typedef struct hti_t
{
    int key;
    int value;
} hti;

typedef struct ht_t
{
    struct hti_t **items;
    size_t size;
    size_t max;
} ht;

struct hti_t *ht_create_item(int key, int value)
{
    struct hti_t *hti = (struct hti_t *)(malloc(sizeof(struct hti_t)));
    hti->key = key;
    hti->value = value;
    return hti;
}

struct ht_t *ht_malloc(size_t max)
{
    struct ht_t *ht = (struct ht_t *)(malloc(sizeof(struct ht_t)));
    ht->max = max;
    ht->size = 0;
    ht->items = (struct hti_t **)(calloc(ht->max, sizeof(struct hti_t *)));
    for (int i = 0; i < max; i++)
        ht->items[i] = 0;
    return ht;
}

void ht_free(struct ht_t *ht)
{
    for (int i = 0; i < ht->max; i++)
    {
        struct hti_t *hti = ht->items[i];
        if (hti)
            free(hti);
    }
    free(ht->items);
    free(ht);
}

int ht_insert_double_hashed_item(ht *ht, int key, int value)
{
    struct hti_t *hti = ht_create_item(key, value);
    int index = HASH_1(key);
    
    ht->size++;
    if (ht->size > TABLE_SIZE)
    {
        free(hti);
        ht->size--;
        return -1;
    }
    else if (ht->items[index] == 0)
    {
        ht->items[index] = hti;
        return 0;
    }
    else
    {
        int space = HASH_2(key),
            cols = 1;
        if (ht->items[space] == 0)
        {
            ht->items[space] = hti;
            return cols;
        }
        else
        {
            cols++;
            index = double_hash_probe(index, space);
            while (ht->items[index])
            {
                cols++;
                index = double_hash_probe(index, space);
            }
            ht->items[index] = hti;
            return cols;
        }
    }
}

int ht_find_double_hashed_item(struct ht_t *ht, int key)
{
    int index = HASH_1(key);
    if (ht->items[index]->key == key)
        return ht->items[index]->value;
    else
    {
        int space = HASH_2(key);
        if (ht->items[space]->key == key)
            return ht->items[space]->value;
        else
        {
            index = double_hash_probe(index, space);
            if (ht->items[index]->key == key)
                return ht->items[space]->value;
            else
            {
                while (ht->items[index])
                {
                    index = double_hash_probe(index, space);
                    if (ht->items[index]->key == key)
                        return ht->items[index]->value;
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
        collisions += ht_insert_double_hashed_item(ht, data[i], data[i]);
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
    ht_free(ht);
    return 0;
}