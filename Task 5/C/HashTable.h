#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <stdlib.h> //malloc
#include <string.h> //memcpy

typedef struct dll_ln_t ln;

typedef struct dll_t dll;

typedef struct dll_it_t it;

typedef struct hti_t hti;

typedef struct ht_t ht;

typedef size_t hash_func(const void *, size_t, size_t);
typedef void free_func(struct hti_t*);

/*
* List node functions.
*/

/*
* Creates list node and adds memory. 
*/
struct dll_ln_t *dll_ln_malloc(void *e, struct dll_ln_t *n, struct dll_ln_t *p, free_func);
/*
* Removes list node and frees memory.  
*/
void dll_ln_free(struct dll_ln_t *ln);

/*
* Linked List functions.
*/

/*
* Creates linked list and adds memory. 
*/
struct dll_t *dll_malloc(size_t size);
/*
*  Removes linked list and frees memory.
*/
void dll_free(struct dll_t *dll);
/*
* Adds item to first position of linked list and allocates memory.
*/
void dll_add_first(struct dll_t *dll, void *value, free_func);
/*
* Adds item to last position of linked list and allocates memory.
*/
void dll_add_last(struct dll_t *dll, void *value, free_func);
/*
* Removes item from linked list and frees memory.
*/
struct dll_ln_t* dll_remove_ln(struct dll_t *dll, struct dll_ln_t *ln);

/*
* List Iterator functions.
*/

/*
*  Starts iterator at start of list.
*/
void dll_it_start(struct dll_it_t *it, struct dll_t *dll);
/*
*  Starts iterator at end of list.
*/
void dll_it_start_end(struct dll_it_t *it, struct dll_t *dll);
/*
*  Moves iterator to next item in list.
*/
void dll_it_next(struct dll_it_t *it);
/*
*  Moves iterator to previous item in list.
*/
void dll_it_prev(struct dll_it_t *it);
/*
*  Checks if iterator is at the end of the list.
*/
int dll_it_end(struct dll_it_t *it);

/*
* Hash Table Item functions.
*/

/*
* Creates hash table item and allocates memory.
*/
int hti_create_(struct ht_t *ht, const void *key, size_t key_size, void *value,
                size_t val_size, hash_func);
/*
* Removes hash table item and frees memory.
*/
void ht_item_free(struct hti_t *hti);

/*
* Hash Table functions.
*/

/*
* Creates hash table and adds memory. 
*/
struct ht_t *ht_malloc(size_t max);
/*
* Removes hash table and frees memory.
*/
void ht_free(struct ht_t *ht);
/*
* Adds an item to the hash table.
*/
int ht_insert_item(struct ht_t *ht, const void *key, size_t key_size, void *value,
                size_t val_size, hash_func);
/*
* Adds a doubly hashed item to the hash table.
*/
int ht_insert_item_double_hash(struct ht_t *ht, const void *key, size_t key_size, void *value,
                size_t val_size, hash_func);
/*
* Finds an item in the hash table.
*/
void* ht_find_item(struct ht_t *ht, const void *key, size_t key_size, hash_func hashing);
/*
* Finds a doubly hashed item to the hash table.
*/
void* ht_find_double_hashed_item(struct ht_t *ht, const void *key, size_t key_size, hash_func hash_func1, hash_func hash_func2);
/*
* Removes an item from the hash table.
*/
int ht_remove_item(struct ht_t *ht, const void *key, size_t key_size, hash_func hashing);
/*
* Creates overflow if needed for collisions.
*/
struct dll_t** ht_create_overflow(struct ht_t *ht);
/*
* Removes overflow and frees memory.
*/
void ht_free_overflow(struct ht_t *ht);
/*
* Handles a collision if one occurs.
*/
void ht_handle_collision(struct ht_t *ht, size_t hash, struct hti_t *hti);

struct dll_ln_t {
    void *element;
    struct dll_ln_t *next;
    struct dll_ln_t *prev;
    void (*free_func)(struct hti_t *);
};

struct dll_ln_t* dll_ln_malloc(void *e, struct dll_ln_t *n, struct dll_ln_t *p, free_func free_func) 
{
    struct dll_ln_t *ln = (struct dll_ln_t*)(malloc(sizeof(struct dll_ln_t)));
    ln->element = e;
    ln->next = n;
    ln->prev = p;
    ln->free_func = free_func;
    return ln;
}

void dll_ln_free(struct dll_ln_t *ln)
{
    ln->free_func(ln->element);
    free(ln);
}

struct dll_t {
    struct dll_ln_t *head;
    struct dll_ln_t *tail;
    size_t size;
};

struct dll_it_t {
    struct dll_ln_t *place;
};

struct dll_t *dll_malloc(size_t size)
{
    struct dll_t *dll = (struct dll_t*)(malloc(sizeof(struct dll_t)));
    dll->head = NULL;
    dll->tail = NULL;
    dll->size = size;
}

void dll_free(struct dll_t *dll) {
    struct dll_ln_t* temp = dll->head;
    while (dll->head) {
        temp = dll->head;
        dll->head = dll->head->next;
        dll_ln_free(temp);
    }
}

void dll_add_first(struct dll_t *dll, void *element, free_func free_func)
{
    struct dll_ln_t *ln = dll_ln_malloc(element, dll->head, NULL, free_func);
    dll->head = ln;
    if (!dll->tail) dll->tail = ln;
    else ln->next->prev = ln;
    dll->size++;
}

void dll_add_last(struct dll_t *dll, void *element, free_func free_func)
{
    struct dll_ln_t *ln = dll_ln_malloc(element, NULL, dll->tail, free_func);
    if (dll->tail) dll->tail->next = ln;
    else dll->head = ln;
    dll->tail = ln;
    dll->size++;
}

struct dll_ln_t *dll_remove_ln(struct dll_t *dll, struct dll_ln_t *ln)
{
    if(ln->prev)
        ln->prev->next = ln->next;
    else dll->head= ln->next;
    if(ln->next)
        ln->next->prev = ln->prev;
    else dll->tail = ln->prev;
    ln->next = NULL;
    ln->prev = NULL;
    dll->size--;
    return ln;
}

void dll_it_start(struct dll_it_t *it, struct dll_t* dll)
{
    it->place = dll->head;
}

void dll_start_end(struct dll_it_t *it, struct dll_t* dll)
{
    it->place = dll->tail;
}

int dll_it_end(struct dll_it_t *it)
{
    return !it->place;
}

void dll_it_next(struct dll_it_t *it)
{
    if(!dll_it_end(it))
        it->place = it->place->next;
}

void dll_it_prev(struct dll_it_t *it)
{
    if(!dll_it_end(it))
        it->place = it->place->prev;
}

struct hti_t {
    void *key;
    void *value;
    size_t key_size;
    size_t val_size;
};

struct hti_t *ht_create_item(const void *key, size_t key_size, const void *value, 
                size_t val_size)
{
    struct hti_t *hti = (struct hti_t*)(malloc(sizeof(struct hti_t)));
    hti->key = malloc(key_size);
    memcpy(hti->key, key, key_size);

    hti->value = malloc(val_size);
    memcpy(hti->value, value, val_size);

    hti->key_size = key_size;
    hti->val_size = val_size;
    return hti;
}

void ht_item_free(struct hti_t *hti)
{
    free(hti->value);
    free(hti->key);
    free(hti);
}

struct ht_t {
    struct hti_t **items;
    struct dll_t **overflow;
    size_t size;
    size_t max;
};

struct ht_t* ht_malloc(size_t max)
{
    struct ht_t *ht = (struct ht_t*)(malloc(sizeof(struct ht_t)));
    ht->max = max;
    ht->size = 0;
    ht->items = (struct hti_t**)(calloc(ht->max, sizeof(struct hti_t*)));
    for (int i = 0; i < max; i++) 
        ht->items[i] = NULL;
    ht->overflow = ht_create_overflow(ht);

    return ht;
}

void ht_free(struct ht_t* ht)
{
    for (int i = 0; i < ht->max; i++)
    {
        struct hti_t* hti = ht->items[i];
        if (hti && hti->key && hti->value) ht_item_free(hti);
    }

    ht_free_overflow(ht);
    free(ht->items);
    free(ht);
}

struct dll_t** ht_create_overflow(struct ht_t *ht) 
{
    struct dll_t** overflow = (struct dll_t**)(calloc(ht->max, sizeof(struct dll_t*)));
    for (int i = 0; i < ht->max; i++) overflow[i] = NULL;
    return overflow;
}

void ht_free_overflow(struct ht_t *ht) 
{
    struct dll_t** overflow = ht->overflow;
    for (int i = 0; i < ht->size; i++) {
        if (overflow[i])
            dll_free(overflow[i]);
    }
    free(overflow);
}

void ht_handle_collision(struct ht_t *ht, size_t hash, struct hti_t *hti)
{
    struct dll_it_t *it;
    struct dll_t *head = ht->overflow[hash];
    
    if (!head)
    {
        head = dll_malloc(10);
        dll_add_first(head, hti, &ht_item_free);
        ht->overflow[hash] = head;
    }
    else
    {
        dll_add_last(ht->overflow[hash], hti, &ht_item_free);
    }
}

// int ht_insert_item_double_hash(struct ht_t *ht, const void *key, size_t key_size, void *value,
//                 size_t val_size, hash_func hash_func)
// {
//     struct hti_t *hti = ht_create_item(key, key_size, value, val_size);
    
//     size_t hash = hash_func(key, key_size, ht->max);

//     while(ht->items[hash] != NULL
//         && ht->items[hash]->key != key
//         && ht->items[hash]->key != -1)
//     {
//         hashIn
//     }
// }

int ht_insert_item(struct ht_t *ht, const void *key, size_t key_size, void *value,
                size_t val_size, hash_func hash_func)
{
    struct hti_t *hti = ht_create_item(key, key_size, value, val_size);

    size_t hash = hash_func(key, key_size, ht->max);
    struct hti_t *curr = ht->items[hash];
    if (!curr)
    {
        if (ht->size == ht->max)
        {
            ht_item_free(hti);
            return 0;
        }

        ht->items[hash] = hti;
        ht->size++;
    }
    else
    {
        if (curr->key_size == key_size) 
            if (memcmp(key, curr->key, key_size) == 0) 
            {
                memcpy(ht->items[hash]->value, value, val_size);
                return 1;
            }
        ht_handle_collision(ht, hash, hti);
        return 2;
    }
}

void* ht_find_item(struct ht_t *ht, const void *key, size_t key_size, hash_func hash_func)
{
    size_t hash = hash_func(key, key_size, ht->max);
    struct hti_t *hti = ht->items[hash];
    struct dll_t *dll = ht->overflow[hash];
    struct dll_it_t *it = (struct dll_it_t*)(malloc(sizeof(struct dll_it_t)));
    if (hti)
    {
        if (key_size == hti->key_size)
        {
            if (!hti || !hti->key_size || !hti->key)
                return NULL;
            if (memcmp(key, hti->key, key_size) == 0)
                return hti->value;
        }       
    }
    if (dll != NULL && dll->head != NULL)
    {
        dll_it_start(it, dll);
        while (!dll_it_end(it))
        {
            hti = (struct hti_t*)it->place->element;
            if (!hti || !hti->key_size || !hti->key)
                return NULL;
            if (key_size == hti->key_size)
                if (memcmp(key, hti->key, key_size) == 0)
                    return hti->value;
            dll_it_next(it);
        }
    }
    return NULL;
}

void* ht_find_double_hashed_item(struct ht_t *ht, const void *key, size_t key_size, hash_func hash_func1, hash_func hash_func2)
{
    size_t hash = hash_func1(key, key_size, ht->max);
    struct hti_t *hti = ht->items[hash];

    size_t index = (size_t)(hash & (uint64_t)(table->capacity - 1));

    // Loop till we find an empty entry.
    while (table->entries[index].key != NULL) {
        if (strcmp(key, table->entries[index].key) == 0) {
            // Found key, return value.
            return table->entries[index].value;
        }
        // Key wasn't in this slot, move to next (linear probing).
        index++;
        if (index >= table->capacity) {
            // At end of entries array, wrap around.
            index = 0;
        }
    }
    return NULL;
}

int ht_remove_item(struct ht_t *ht, const void *key, size_t key_size, hash_func hash_func)
{
    size_t hash = hash_func(key, key_size, ht->max);
    struct dll_t *dll = ht->overflow[hash];
    struct hti_t *hti = ht->items[hash];
    struct dll_it_t *it = (struct dll_it_t*)(malloc(sizeof(struct dll_it_t)));
    if (hti == NULL)
        return 1;
        
    if((hti->key_size == key_size) && memcmp(hti->key, key, key_size) == 0)
    {
        if (dll != NULL && dll->head != NULL)
        {
            if (dll->head->next != NULL)
            {
                dll_ln_free(dll_remove_ln(dll, dll->head));
                ht->items[hash] = (struct hti_t*)dll->head->element;
            }
            else
            {
                ht->items[hash] = (struct hti_t*)dll->head->element;
                ht->overflow[hash] = NULL;
            }
        }
        else 
        {
            ht->items[hash] = NULL;
            ht_item_free(hti);
            ht->size--;
        }
        return 0;
    }
    else if (dll != NULL && dll->head != NULL)
    {
        dll_it_start(it, dll);
        while (!dll_it_end(it))
        {
            hti = (struct hti_t*)it->place->element;
            if (key_size == hti->key_size) 
                if (memcmp(key, hti->key, key_size) == 0)
                {
                    if (it->place != dll->tail)
                    {
                        dll_ln_free(dll_remove_ln(dll, it->place->element));
                    }
                    else
                    {
                        dll_ln_free(dll_remove_ln(dll, dll->tail));
                    }
                    return 0;
                }
            dll_it_next(it);
        }
    }
    return 1;
}

#endif