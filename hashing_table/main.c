#include "include/hash_table.h"
#include <stdio.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 16

/** USAGE **/
typedef struct {
    int a;
    int b;
    struct {
        double aaa;
        double bbb;
    } some_huge_struct;
} some_data_t;

int my_hash_func(void *entry) {
    some_data_t *data = (some_data_t *)entry;
    return (data->a * 31) + data->b; 
}

bool my_cmp_func(void *entry1, void *entry2) {
    some_data_t *data1 = (some_data_t *)entry1;
    some_data_t *data2 = (some_data_t *)entry2;
    return (data1->a == data2->a) && (data1->b == data2->b);
}

void my_data_action(void *entry) {
    some_data_t *data = (some_data_t *)entry;
    printf("Foreach action: aaa = %f\n", data->some_huge_struct.aaa);
}

int main() {
    hash_table_t *table = NULL;

    dsa_status_t status = hash_table_create(
        INITIAL_CAPACITY,
        my_hash_func,
        my_cmp_func,
        &table
    );

    if (status != DSA_OK) {
        fprintf(stderr, "Failed to create hash table\n");
        return 1;
    }
    printf("Hash table created.\n");

    /* Insert */
    some_data_t *data = malloc(sizeof(some_data_t));
    data->a = 10;
    data->b = 20;
    data->some_huge_struct.aaa = 1000.11;
    data->some_huge_struct.bbb = 999.99;

    status = hash_table_insert(table, data);
    if (status == DSA_OK) {
        printf("Inserted entry (a=10, b=20)\n");
    }

    status = hash_table_insert(table, data);
    if (status == DSA_EXISTS) {
        printf("Correctly detected duplicate.\n");
    }

    /* Find */
    printf("Finding entry...\n");
    some_data_t key_to_find = {.a = 10, .b = 20};
    some_data_t *entry_found = hash_table_find(table, &key_to_find);
    
    if (entry_found) {
        printf("Found entry: aaa = %f\n", entry_found->some_huge_struct.aaa);
    } else {
        printf("Entry (a=10, b=20) not found!\n");
    }

    /* Foreach */
    printf("Running foreach...\n");
    hash_table_foreach(table, my_data_action);

    /* Remove entry */
    printf("Removing entry (a=10, b=20)...\n");
    status = hash_table_remove(table, &key_to_find);
    if (status == DSA_OK) {
        printf("Entry removed.\n");
    } else if (status == DSA_NOT_FOUND) {
        printf("Entry not found, cannot remove.\n");
    }

    entry_found = hash_table_find(table, &key_to_find);
    if (entry_found == NULL) {
        printf("Entry correctly removed (find returned NULL).\n");
    }

    free(data);
    printf("Freed user data.\n");

    /* Destroy table */
    hash_table_destroy(table);
    printf("Hash table destroyed.\n");

    return 0;
}