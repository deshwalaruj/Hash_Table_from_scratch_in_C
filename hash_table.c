#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "hash_table.h"
#include "prime.h"

int PRIME_1 = 179;
int PRIME_2 = 181;
int START_BASE_SIZE = 61;

static item DELETED_ITEM = {NULL, NULL};

static item* create_new_item(const char* new_key, const char* new_val) {
    item* new_item = malloc(sizeof(item));
    new_item -> key = strdup(new_key);
    new_item -> value = strdup(new_val);
    return new_item;
}

static hash_table* create_sized_hash_table(const int base_size) {
    hash_table* ht = malloc(sizeof(hash_table)); // xmalloc

    ht -> base_size = base_size;
    ht -> size = next_prime(ht -> base_size);
    ht -> count = 0;
    ht -> arr = calloc((size_t)ht -> size, sizeof(item*)); // xcalloc
    return ht;
}

hash_table* create_hash_table() {
    return create_sized_hash_table(START_BASE_SIZE);
}

static void delete_item(item* del_item) {
    free(del_item -> key);
    free(del_item -> value);
    free(del_item);
}

void delete_hash_table(hash_table* del_ht) {
    for (int i = 0; i < del_ht -> size; i++) {
        item* cur_item = del_ht->arr[i];
        if (cur_item != NULL) {
            delete_item(cur_item);
        }
    }
    free(del_ht -> arr);
    free(del_ht);
}

int hash(const char* key, const int prime, const int num_buckets) {
    long hash_val = 0;
    int len = strlen(key);
    for (int i = len - 1; i >= 0; i--) {
        hash_val = (hash_val + (long)pow(prime, (long)i * key[i])) % num_buckets;
    }
    return (int)hash_val;
}


static int double_hash(const char* key, const int num_buckets, const int num_hits) {
    const int hash_1 = hash(key, PRIME_1, num_buckets);
    const int hash_2 = hash(key, PRIME_2, num_buckets);
    int double_hash_val = (hash_1 + (hash_2 + 1) * num_hits) % num_buckets;
    return double_hash_val;
}

void insert(hash_table* ht, const char* key, const char* value) {
    const int percent_filled = (ht -> count * 100)  / ht -> base_size;
    if (percent_filled > 70) {
        resize_up(ht);
    }

    item* new_item = create_new_item(key, value);
    int index = double_hash(key, ht -> size, 0);
    item* cur_item = ht -> arr[index];
    int i = 0;
    while (cur_item != NULL) {
        
        if (cur_item != &DELETED_ITEM) {
            if (strcmp(cur_item -> key, key) == 0) {
                delete_item(cur_item);
                ht -> arr[index] = new_item;
                return;
            }
        }
        i++;
        index = double_hash(key, ht -> size, i);
        cur_item = ht -> arr[index];
    }
    ht -> arr[index] = new_item;
    ht -> count++;
}

char* search(hash_table* ht, const char* key) {
    int index = double_hash(key, ht -> size, 0);
    item* cur_item = ht -> arr[index];
    int i = 0;
    while (cur_item != NULL) {
        i++;
        if (cur_item != &DELETED_ITEM) {
            if (strcmp(cur_item -> key, key) == 0) { // 0 signifies equality
                return cur_item -> value;
            }
        }
        index = double_hash(key, ht -> size, i);
        cur_item = ht -> arr[index];
    }
    return NULL; // if no match found
}

void delete(hash_table* ht, const char* key) {
    const int percent_filled = (ht -> count * 100)  / ht -> base_size;
    if (percent_filled < 10) {
        resize_down(ht);
    }

    int index = double_hash(key, ht -> size, 0);
    item* cur_item = ht -> arr[index];
    bool found = false;
    int num_hits = 0;

    while (cur_item != NULL) {
        num_hits++;
        if (cur_item != &DELETED_ITEM) {
            if (strcmp(cur_item -> key, key) == 0) {
                delete_item(cur_item);
                ht -> arr[index] = &DELETED_ITEM;
                found = true;
                break;
            }
        }
        index = double_hash(key, ht -> size, num_hits);
        cur_item = ht -> arr[index];
    }
    if (found) {
        ht -> count--;
    }
}

static void resize(hash_table* ht, const int base_size) {
    if (base_size < START_BASE_SIZE) { // ensures not resizing it below min threshold
        return;
    }
    hash_table* new_ht = create_sized_hash_table(base_size);
    for (int i = 0; i < ht -> size; i++) {
        item* cur_item = ht -> arr[i];
        if (cur_item != NULL && cur_item != &DELETED_ITEM) {
            insert(new_ht, cur_item -> key, cur_item -> value);
        }
    }
    ht -> base_size = new_ht -> base_size;
    ht -> count = new_ht -> count;

    const int placeholder_size = ht -> size;
    ht -> size = new_ht -> size;
    new_ht -> size = placeholder_size;

    item** placeholder_arr = ht -> arr;
    ht -> arr = new_ht -> arr;
    new_ht -> arr = placeholder_arr;

    delete_hash_table(new_ht);
}

static void resize_up(hash_table* ht) {
    const int increased_size = ht -> base_size * 2;
    resize(ht, increased_size);
}

static void resize_down(hash_table* ht) {
    const int reduced_size = ht -> base_size / 2;
    resize(ht, reduced_size);
}




