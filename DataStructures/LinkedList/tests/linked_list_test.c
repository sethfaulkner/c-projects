#include <stdlib.h>
#include <time.h>
#include "CUnit/Basic.h"

#include "../SRC/linked_list.h"

#define MIN_LIST_SIZE 5
#define MAX_LIST_SIZE 20
#define MAX_DATA 100

int cmp_int(void *lhs, void *rhs){
    if (*(int*)lhs > *(int*)rhs)
        return 1;
    if (*(int*)lhs == *(int*)rhs)
        return 0;
    return -1;
}

void mod_by_2(void *item){
    *(int*)item = *(int*)item % 2;
}

void print_int(void *item){
    printf("%d->", *(int*)item);
}

int init_suite(){
    srand(time(0));
    return 0;
}

int clean_suite(){
    return 0;
}

void test_linked_list_new(){
    linked_list_t *list = linked_list_new(free, (cmp_f)cmp_int);
    CU_ASSERT_PTR_NOT_NULL(list);
    linked_list_delete(list);
}

void test_linked_list_append(){
    linked_list_t *list = linked_list_new(free, (cmp_f)cmp_int);
    CU_ASSERT_PTR_NOT_NULL(list);

    size_t num_nodes = rand() % (MAX_LIST_SIZE + 1 - MIN_LIST_SIZE) + MIN_LIST_SIZE;
    for (size_t i = 0; i < num_nodes; i++){
        void *data = malloc(sizeof(int));
        *(int*)data = rand() % MAX_DATA;

        size_t prev_size = linked_list_size(list);
        linked_list_append(list, data);
        CU_ASSERT_TRUE(linked_list_size(list) > prev_size);
    }

    size_t nodes_added = linked_list_size(list);
    CU_ASSERT_EQUAL(nodes_added, num_nodes);

    linked_list_delete(list);
}

void test_linked_list_insert(){
    linked_list_t *list = linked_list_new(free, (cmp_f)cmp_int);
    CU_ASSERT_PTR_NOT_NULL(list);

    size_t num_nodes = rand() % (MAX_LIST_SIZE + 1 - MIN_LIST_SIZE) + MIN_LIST_SIZE;
    for (size_t i = 0; i < num_nodes; i++){
        void *data = malloc(sizeof(int));
        *(int*)data = rand() % MAX_DATA;

        linked_list_append(list, data);
    }

    void *insert = malloc(sizeof(int));
    *(int*)insert = rand() % MAX_DATA;

    size_t index = rand() % num_nodes;
    linked_list_insert(list, index, insert);

    CU_ASSERT_EQUAL(*(int*)linked_list_at(list, index), *(int*)insert);
    CU_ASSERT_EQUAL(linked_list_size(list), num_nodes + 1);

    linked_list_delete(list);
}

void test_linked_list_for_each(){
    linked_list_t *list = linked_list_new(free, (cmp_f)cmp_int);
    CU_ASSERT_PTR_NOT_NULL(list);

    size_t num_nodes = rand() % (MAX_LIST_SIZE + 1 - MIN_LIST_SIZE) + MIN_LIST_SIZE;
    for (size_t i = 0; i < num_nodes; i++){
        void *data = malloc(sizeof(int));
        *(int*)data = rand() % MAX_DATA;
        linked_list_append(list, data);
    }

    linked_list_for_each(list, (action_f)mod_by_2, NULL);

    for (size_t i = 0; i < num_nodes; i++){
        int value = *(int*)linked_list_at(list, i);
        CU_ASSERT_TRUE(0 == value || 1 == value);
    }

    linked_list_delete(list);
}

void test_linked_list_find(){
    linked_list_t *list = linked_list_new(free, (cmp_f)cmp_int);
    CU_ASSERT_PTR_NOT_NULL(list);

    size_t num_nodes = rand() % (MAX_LIST_SIZE + 1 - MIN_LIST_SIZE) + MIN_LIST_SIZE;
    for (size_t i = 0; i < num_nodes; i++){
        void *data = malloc(sizeof(int));

        do {
            *(int*)data = rand() % MAX_DATA;
        } while(-1 != linked_list_find(list, data));
        
        linked_list_append(list, data);
    }

    linked_list_print(list, (print_f)print_int);


    void *insert = malloc(sizeof(int));
    do {
        *(int*)insert = rand() % MAX_DATA;
    } while(-1 != linked_list_find(list, insert));

    size_t index = rand() % num_nodes;

    linked_list_insert(list, index, insert);

    size_t loc = linked_list_find(list, insert);

    CU_ASSERT_EQUAL(loc, index);

    linked_list_delete(list);
}

void test_linked_list_delete_at(){
    linked_list_t *list = linked_list_new(free, (cmp_f)cmp_int);
    CU_ASSERT_PTR_NOT_NULL(list);

    size_t num_nodes = rand() % (MAX_LIST_SIZE + 1 - MIN_LIST_SIZE) + MIN_LIST_SIZE;
    for (size_t i = 0; i < num_nodes; i++){
        void *data = malloc(sizeof(int));
        *(int*)data = rand() % MAX_DATA;
        linked_list_append(list, data);
    }

    size_t index = rand() % num_nodes;
    int target = *(int*)linked_list_at(list, index);

    linked_list_delete_at(list, index);

    CU_ASSERT_NOT_EQUAL(*(int*)linked_list_at(list, index), target);
    CU_ASSERT_EQUAL(linked_list_size(list), num_nodes - 1);

    linked_list_delete(list);
}

void test_linked_list_print(){
    linked_list_t *list = linked_list_new(free, (cmp_f)cmp_int);
    CU_ASSERT_PTR_NOT_NULL(list);

    size_t num_nodes = rand() % (MAX_LIST_SIZE + 1 - MIN_LIST_SIZE) + MIN_LIST_SIZE;
    for (size_t i = 0; i < num_nodes; i++){
        void *data = malloc(sizeof(int));
        *(int*)data = rand() % MAX_DATA;
        linked_list_append(list, data);
    }

    printf("\nPrinting list\n\n");
    linked_list_print(list, print_int);
    printf("\n");

    linked_list_delete(list);
}

int main(){

    CU_pSuite pSuite = NULL;

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    pSuite = CU_add_suite("Linked List Suite", init_suite, clean_suite);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if(
        (NULL == CU_add_test(pSuite, "test linked list new", test_linked_list_new)) ||
        (NULL == CU_add_test(pSuite, "test linked list append", test_linked_list_append)) ||
        (NULL == CU_add_test(pSuite, "test linked list insert", test_linked_list_insert)) ||
        (NULL == CU_add_test(pSuite, "test linked list find", test_linked_list_find)) ||
        (NULL == CU_add_test(pSuite, "test linked list for each", test_linked_list_for_each)) ||
        (NULL == CU_add_test(pSuite, "test linked list print", test_linked_list_print))
    ){
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
