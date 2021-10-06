#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef void (*del_f)(void *data);

/**
 * @brief function stub for comparison
 * @param lhs left hand side of comparison
 * @param rhs right hand side of comparison
 *
 * @return 1 if lhs > rhs, 0 if lhs == rhs, -1 if lhs < rhs
 */
typedef int (*cmp_f)(void *lhs, void *rhs);

typedef void (*action_f)(void *item, void *ctx);
typedef void (*print_f)(void *item);

typedef struct ll_node_t {
    void *data;
    struct ll_node_t *next;
} ll_node_t;

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct linked_list_t {
    struct ll_node_t *head;
    int (*cmp_f)(void *lhs, void *rhs);
    void (*del_f)(void *data);
} linked_list_t;

linked_list_t *linked_list_new(del_f del, cmp_f cmp);
size_t linked_list_size(linked_list_t *list);
void * linked_list_at(linked_list_t *list, size_t index);
void linked_list_append(linked_list_t *list, void *data);
void linked_list_insert(linked_list_t *list, size_t index, void *data);
void linked_list_delete_at(linked_list_t *list, size_t index);
void linked_list_for_each(linked_list_t *list, action_f action, void *context);
size_t linked_list_find(linked_list_t *list, void *data);
void linked_list_print(linked_list_t *list, print_f print);
void linked_list_delete(linked_list_t *list);
bool linked_list_index_in_range(linked_list_t *list, size_t index);

size_t nodes_count(ll_node_t *n);
void nodes_delete(ll_node_t *n, del_f del);


#endif
