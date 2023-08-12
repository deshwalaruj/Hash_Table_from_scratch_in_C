#include <stdio.h>

#include "hash_table.h"

int main() {
    printf("running\n");
    hash_table* my_table = create_hash_table();
    insert(my_table, "Aruj", "val");
    char* result = search(my_table, "Aruj");
    printf("%s\n", result);
    delete(my_table, "Aruj");
    char* new_result = search(my_table, "Aruj");
    if (new_result == NULL) {
        printf("Deletion worked\n");
    }
    printf("Ran Successfully\n");
    return 0;
}