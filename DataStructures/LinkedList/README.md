# Linked List

## Getting started

```c
#include <stdlib.h>
#include "linked_list.h"

int main(){
    // simple compare int function. Found in tests
    int cmp_int(void *lhs, void *rhs){
        if (*(int*)lhs > *(int*)rhs)
            return 1;
        if (*(int*)lhs == *(int*)rhs)
            return 0;
        return -1;
    }

    // Create the list
    linked_list_t *list = linked_list_new(free, (cmp_f)cmp_int);

    // add values to the list
    for (size_t i = 0; i < 10; i++){
        void *data = malloc(sizeof(int));
        *(int*)data = i;

        linked_list_append(list, data);
    }

    // delete the list
    linked_list_delete(list);

    return 0;
}

```
