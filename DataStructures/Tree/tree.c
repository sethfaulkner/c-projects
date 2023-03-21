#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "tree.h"

tree_t *tree_create(print_f print, del_f del){
    tree_t* t = malloc(sizeof(tree_t));
    t->head = NULL;
    t->print_f = print;
    t->del_f = del;
    return t;
}

int tree_add(tree_t *tree, void *data, cmp_f cmp){
    if (NULL == tree)
        return -1;

    tree_node_t* n = malloc(sizeof(tree_node_t));
    if (NULL == n){
        perror("malloc");
        return -1;
    }

    n->data = data;
    n->right = NULL;
    n->left = NULL;

    if (NULL == tree->head){
        tree->head = n;
        return 0;
    }

    node_add(tree->head, n, cmp);
    return 0;
}

void tree_remove_node(tree_t *tree, void *data, cmp_f cmp){
    if (NULL == tree)
        return;
        
    if (NULL == data){
        printf("[ERROR] Address not found\n");
        return;
    }

    tree->head = node_delete(tree->head, data, cmp, tree->del_f);
}

tree_node_t *node_delete(tree_node_t *root, void *data, cmp_f cmp, del_f del){
    if (NULL == root){
        return NULL;
    }

    if (NULL == data){
        printf("[ERROR] 'data' cannot be NULL\n");
        return root;
    }

    if (0 > cmp(data, root->data)){
        root->left = node_delete(root->left, data, cmp, del);
    }else if (0 < cmp(data, root->data)){
        root->right = node_delete(root->right, data, cmp, del);
    }else{
        if (NULL == root->left){
            tree_node_t *temp = root->right;
            if (NULL != del)
                del(root->data);
            free(root);
            return temp;
        }else if (NULL == root->right){
            tree_node_t *temp = root->left;
            if (NULL != del)
                del(root->data);
            free(root);
            return temp;
        }else{
            tree_node_t *temp = node_extract_smallest(root->right);
            if (NULL == temp){
                temp = root->right;
            }

            nodes_add(root, temp->right, cmp);
            temp->left = root->left;

            del(root->data);
            free(root);

            return temp;
        }
    }
    return root;

}

tree_node_t *node_extract_smallest(tree_node_t *root){
    if (NULL == root){
        return NULL;
    }

    if (NULL == root->left){
        return NULL;
    }

    if (NULL == root->left->left){
        tree_node_t *temp = root->left;
        root->left = NULL;
        return temp;
    }

    return node_extract_smallest(root->left);
}

int tree_destroy(tree_t *tree){
    if (tree == NULL)
        return -1;
    nodes_delete(tree->head, tree->del_f);
    free(tree);
    return 0;
}

// Helpers

void node_add(tree_node_t* root, tree_node_t* n, cmp_f cmp){
    if (0 < cmp(n->data, root->data)){
        if (root->right == NULL){
            root->right = n;
            n->left = NULL;
            n->right = NULL;
            return;
        }
        node_add(root->right, n, cmp);
    }else if (0 > cmp(n->data, root->data)){
        if (root->left == NULL){
            root->left = n;
            n->left = NULL;
            n->right = NULL;
            return;
        }
        node_add(root->left, n, cmp);
    }else{
        printf("[ERROR] Attempting to add duplicate entry\n");
    }
}

void nodes_add(tree_node_t *root, tree_node_t *node, cmp_f cmp){
    if (NULL == root || NULL == node)
        return;
    nodes_add(root, node->right, cmp);
    nodes_add(root, node->left, cmp);

    node_add(root, node, cmp);
}

void nodes_delete(tree_node_t* n, del_f del){
    if (n == NULL)
        return;
    nodes_delete(n->left, del);
    nodes_delete(n->right, del);
    
    del(n->data);
    free(n);
}