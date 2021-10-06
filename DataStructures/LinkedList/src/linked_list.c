//Simple linked list implementation
/**
 * @file
 * @brief linked_list.c is a library to implement a dynamic linked list
 */

#include "linked_list.h"

/**
 * @brief This function creates a new linked list
 * @param del the delete function fot the data type (free in most cases)
 * @param cmp the compare function for sorting and locating
 *
 * @return a new linked list (ref linked_list.h for data type), NULL on error
 */
linked_list_t * linked_list_new(del_f del, cmp_f cmp){
    linked_list_t *ll = malloc(sizeof(*ll));
    if (NULL == ll){
        perror("malloc");
        return NULL;
    }
    ll->head = NULL;
    ll->cmp_f = cmp;
    ll->del_f = del;
    return ll;
}

/**
 * @brief This function gets the number of nodes in the list
 * @param list the list to count
 *
 * @return the number of nodes, -1 on error
 */
size_t linked_list_size(linked_list_t *list){
    if (NULL == list)
        return -1;

    return nodes_count(list->head);
}

/**
 * @brief This function appends an item to the tail of the list
 * @param list the list
 * @param data the item to append
 */
void linked_list_append(linked_list_t *list, void *data){
    if (NULL == list)
        return;

    ll_node_t *node = malloc(sizeof(*node));
    if (NULL == node){
        perror("malloc");
        return;
    }

    node->data = data;
    node->next = NULL;

    if (NULL == list->head){
        list->head = node;
        return;
    }

    ll_node_t *temp = list->head;
    while(NULL != temp->next){
        temp = temp->next;
    }
    temp->next = node;
}

/**
 * @brief This function inserts an item into an index in the list
 * @param list the list
 * @param index the index at which to insert the item
 * @param data the item to insert
 */
void linked_list_insert(linked_list_t *list, size_t index, void *data){
    if (NULL == list)
        return;

    if (!linked_list_index_in_range(list, index))
        return;

    ll_node_t *node = malloc(sizeof(*node));
    node->data = data;
    node->next = NULL;

    if (index == 0){
        node->next = list->head;
        list->head = node;
        return;
    }

    ll_node_t *insert_after = list->head;

    for (size_t i = 0; i < index-1; i++){
        insert_after = insert_after->next;
    }
    node->next = insert_after->next;
    insert_after->next = node;
}

/**
 * @brief This function gets an item at a specified index
 * @param list the list
 * @param index the index the item is at
 *
 * @return the item at that index, NULL on error or invalid index
 */
void * linked_list_at(linked_list_t *list, size_t index){
    if (NULL == list)
        return NULL;

    if (!linked_list_index_in_range(list, index))
        return NULL;

    ll_node_t *node = list->head;
    for (int i = 0; i < index; i++){
        node = node->next;
    }
    return node->data;
}

/**
 * @brief This function deletes an item at a specified index
 * @param list the list
 * @param index the item to delete
 */
void linked_list_delete_at(linked_list_t *list, size_t index){
    if (NULL == list)
        return;

    if (!linked_list_index_in_range(list, index))
        return;

    ll_node_t *node = list->head;

    if (index == 0){
        list->head = node->next;
        list->del_f(node->data);
        free(node);
        return;
    }

    for (int i = 0; i < index-1; i++){
        node = node->next;
    }

    ll_node_t *temp = node->next;
    node->next = temp->next;
    list->del_f(temp->data);
    free(temp);
}

/**
 * @brief This function iterates through the list and applies the action_f to each item
 * @param list the list
 * @param action the function to be applied (stub is in linked_list.h)
 * @param context an optional argument if additional data is needed
 */
void linked_list_for_each(linked_list_t *list, action_f action, void *context){
    if (NULL == list)
        return;

    ll_node_t *node = list->head;
    while(NULL != node){
        action(node->data, context);
        node = node->next;
    }
}

/**
 * @brief This function finds an element in the list
 * @param list the list
 * @param data the item to find
 *
 * @return the index the item is located at
 */
size_t linked_list_find(linked_list_t *list, void *data){
    if (NULL == list || NULL == data)
        return -1;

    ll_node_t *node = list->head;
    size_t i = 0;
    while(NULL != node){
        if (0 == list->cmp_f(data, node->data))
            return i;
        node = node->next;
        i++;
    }
    return -1;
}

/**
 * @brief This function prints the linked list
 * @param list the list
 * @param print the function to print a node
 */
void linked_list_print(linked_list_t *list, print_f print){
    if (NULL == list)
        return;

    ll_node_t *node = list->head;
    while(NULL != node){
        print(node->data);
        node = node->next;
    }
}

/**
 * @brief This function deletes each element in the list and frees the list
 * @param list the list
 */
void linked_list_delete(linked_list_t *list){
    if (NULL == list)
        return;

    nodes_delete(list->head, list->del_f);

    free(list);
}

/**
 * @brief This function is a helper to see if an index is valid
 * @param list the list
 * @param data the index to check
 *
 * @return true if index is valid, false if not or error
 */
bool linked_list_index_in_range(linked_list_t *list, size_t index){
    if (NULL == list)
        return false;

    size_t num_nodes = nodes_count(list->head);
    if (index > num_nodes || index < 0)
        return false;
    
    return true;
}

/**
 * @brief Helper to count nodes recursively
 * @param n the node to start at
 *
 * @return the number of nodes after the starting node
 */
size_t nodes_count(ll_node_t *n){
    if (NULL == n)
        return 0;

    return 1 + nodes_count(n->next);
}

/**
 * @brief Helper to delete nodes recursively
 * @param n the node to start at
 * @param del the delete function
 */
void nodes_delete(ll_node_t *n, del_f del){
    if (NULL == n || NULL == del)
        return;

    if (NULL != n->next)
        nodes_delete(n->next, del);

    del(n->data);
    free(n);
}
