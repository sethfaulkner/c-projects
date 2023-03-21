#ifndef TREE_H
#define TREE_H

typedef void (*del_f)(void *data);
typedef int (*cmp_f)(void *lhs, void *rhs);
typedef void (*print_f)(void *data);

typedef struct tree_node_t {
    void *data;
    struct tree_node_t *left;
    struct tree_node_t *right;
} tree_node_t;

typedef struct tree_t {
    struct tree_node_t *head;
    void (*print_f)(void *data);
    void (*del_f)(void *data);
} tree_t;


tree_t *tree_create(print_f print, del_f del);
int tree_add(tree_t *tree, void *data, cmp_f cmp);
void tree_remove_node(tree_t *tree, void *data, cmp_f cmp);
int tree_destroy(tree_t *tree);

// Helpers
void node_add(tree_node_t *root, tree_node_t *n, cmp_f cmp);
void nodes_add(tree_node_t *root, tree_node_t *node, cmp_f cmp);
tree_node_t *node_delete(tree_node_t *root, void *data, cmp_f cmp, del_f del);
void nodes_delete(tree_node_t* n, del_f del);
int cmp_aliases(void *lhs, void *rhs);
tree_node_t *node_extract_smallest(tree_node_t *node);

#endif
