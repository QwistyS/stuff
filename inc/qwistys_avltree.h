#ifndef QWISTYS_AVLTREE_H
#define QWISTYS_AVLTREE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "qwistys_macros.h"
#define QWISTYS_ALLOC_IMPLMENTATION
#include "qwistys_alloc.h"

// AVL tree node structure
typedef struct avlt_node_t {
    void *user_data;
    uint32_t height;
    struct avlt_node_t *left;
    struct avlt_node_t *right;
    struct avlt_node_t *parent;
} avlt_node_t;

// Function prototypes
avlt_node_t *avlt_create_node(size_t user_data_length_in_bytes);
uint32_t avlt_get_height(avlt_node_t *node);
int avlt_get_balance(avlt_node_t *node);
avlt_node_t *avlt_right_rotate(avlt_node_t *y);
avlt_node_t *avlt_left_rotate(avlt_node_t *x);
avlt_node_t *avlt_insert(avlt_node_t *node, void *user_data, size_t data_length, int (*cmp)(void *, void *));
avlt_node_t *avlt_min_value_node(avlt_node_t *node);
avlt_node_t *avlt_delete(avlt_node_t *root, void *user_data, int (*cmp)(void *, void *), void (*del_data)(void *));
void avlt_pre_order(avlt_node_t *root, void (*process_node)(void *));
void avlt_in_order(avlt_node_t *root, void (*process_node)(void *));
void avlt_post_order(avlt_node_t *root, void (*process_node)(void *));
void avlt_print(avlt_node_t *root, void (*print_node)(void *));

// Implementation section
#ifdef QWISTYS_AVLT_IMPLEMENTATION

// Function to create a new node with user data
avlt_node_t *avlt_create_node(size_t user_data_length_in_bytes) {
    avlt_node_t *node = (avlt_node_t *)QWISTYS_ALLOC(sizeof(avlt_node_t));
    if (!node) return NULL;
    node->user_data = QWISTYS_ALLOC(user_data_length_in_bytes);
    if (!node->user_data) {
        QWISTYS_FREE(node);
        return NULL;
    }
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

// Function to get the height of a node
uint32_t avlt_get_height(avlt_node_t *node) {
    return node ? node->height : 0;
}

// Helper function to calculate the balance factor of a node
int avlt_get_balance(avlt_node_t *node) {
    if (!node) return 0;
    return avlt_get_height(node->left) - avlt_get_height(node->right);
}

// Right rotate the subtree rooted with y
avlt_node_t *avlt_right_rotate(avlt_node_t *y) {
    avlt_node_t *x = y->left;
    avlt_node_t *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = 1 + (avlt_get_height(y->left) > avlt_get_height(y->right) ? avlt_get_height(y->left) : avlt_get_height(y->right));
    x->height = 1 + (avlt_get_height(x->left) > avlt_get_height(x->right) ? avlt_get_height(x->left) : avlt_get_height(x->right));

    return x;
}

// Left rotate the subtree rooted with x
avlt_node_t *avlt_left_rotate(avlt_node_t *x) {
    avlt_node_t *y = x->right;
    avlt_node_t *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = 1 + (avlt_get_height(x->left) > avlt_get_height(x->right) ? avlt_get_height(x->left) : avlt_get_height(x->right));
    y->height = 1 + (avlt_get_height(y->left) > avlt_get_height(y->right) ? avlt_get_height(y->left) : avlt_get_height(y->right));

    return y;
}

// Function to insert a new node
avlt_node_t *avlt_insert(avlt_node_t *node, void *user_data, size_t data_length, int (*cmp)(void *, void *)) {
    if (!node) {
        avlt_node_t *new_node = avlt_create_node(data_length);
        if (new_node) {
            QWISTYS_MEMCPY(new_node->user_data, user_data, data_length);
        }
        return new_node;
    }

    int cmp_result = cmp(user_data, node->user_data);
    if (cmp_result < 0) {
        node->left = avlt_insert(node->left, user_data, data_length, cmp);
    } else if (cmp_result > 0) {
        node->right = avlt_insert(node->right, user_data, data_length, cmp);
    } else {
        // Duplicate data, handle as necessary
        return node;
    }

    // Update height of this ancestor node
    node->height = 1 + (avlt_get_height(node->left) > avlt_get_height(node->right) ? avlt_get_height(node->left) : avlt_get_height(node->right));

    // Get the balance factor of this ancestor node to check whether this node became unbalanced
    int balance = avlt_get_balance(node);

    // If the node becomes unbalanced, perform rotations
    // Left Left Case
    if (balance > 1 && cmp(user_data, node->left->user_data) < 0) {
        return avlt_right_rotate(node);
    }

    // Right Right Case
    if (balance < -1 && cmp(user_data, node->right->user_data) > 0) {
        return avlt_left_rotate(node);
    }

    // Left Right Case
    if (balance > 1 && cmp(user_data, node->left->user_data) > 0) {
        node->left = avlt_left_rotate(node->left);
        return avlt_right_rotate(node);
    }

    // Right Left Case
    if (balance < -1 && cmp(user_data, node->right->user_data) < 0) {
        node->right = avlt_right_rotate(node->right);
        return avlt_left_rotate(node);
    }

    return node;
}

// Function to find the node with the minimum value
avlt_node_t *avlt_min_value_node(avlt_node_t *node) {
    avlt_node_t *current = node;
    while (current->left) {
        current = current->left;
    }
    return current;
}

// Function to delete a node
avlt_node_t *avlt_delete(avlt_node_t *root, void *user_data, int (*cmp)(void *, void *), void (*del_data)(void *)) {
    if (!root) return NULL;

    int cmp_result = cmp(user_data, root->user_data);
    if (cmp_result < 0) {
        root->left = avlt_delete(root->left, user_data, cmp, del_data);
    } else if (cmp_result > 0) {
        root->right = avlt_delete(root->right, user_data, cmp, del_data);
    } else {
        if (!root->left || !root->right) {
            avlt_node_t *temp = root->left ? root->left : root->right;
            if (!temp) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }
            del_data(temp->user_data);
            QWISTYS_FREE(temp->user_data);
            QWISTYS_FREE(temp);
        } else {
            avlt_node_t *temp = avlt_min_value_node(root->right);
            QWISTYS_MEMCPY(root->user_data, temp->user_data, sizeof(temp->user_data));
            root->right = avlt_delete(root->right, temp->user_data, cmp, del_data);
        }
    }

    if (!root) return root;

    // Update height of the current node
    root->height = 1 + (avlt_get_height(root->left) > avlt_get_height(root->right) ? avlt_get_height(root->left) : avlt_get_height(root->right));

    // Get the balance factor
    int balance = avlt_get_balance(root);

    // If the node becomes unbalanced, perform rotations
    // Left Left Case
    if (balance > 1 && avlt_get_balance(root->left) >= 0) {
        return avlt_right_rotate(root);
    }

    // Left Right Case
    if (balance > 1 && avlt_get_balance(root->left) < 0) {
        root->left = avlt_left_rotate(root->left);
        return avlt_right_rotate(root);
    }

    // Right Right Case
    if (balance < -1 && avlt_get_balance(root->right) <= 0) {
        return avlt_left_rotate(root);
    }

    // Right Left Case
    if (balance < -1 && avlt_get_balance(root->right) > 0) {
        root->right = avlt_right_rotate(root->right);
        return avlt_left_rotate(root);
    }

    return root;
}

// Pre-order traversal
void avlt_pre_order(avlt_node_t *root, void (*process_node)(void *)) {
    if (root) {
        process_node(root->user_data);
        avlt_pre_order(root->left, process_node);
        avlt_pre_order(root->right, process_node);
    }
}

// In-order traversal
void avlt_in_order(avlt_node_t *root, void (*process_node)(void *)) {
    if (root) {
        avlt_in_order(root->left, process_node);
        process_node(root->user_data);
        avlt_in_order(root->right, process_node);
    }
}

// Post-order traversal
void avlt_post_order(avlt_node_t *root, void (*process_node)(void *)) {
    if (root) {
        avlt_post_order(root->left, process_node);
        avlt_post_order(root->right, process_node);
        process_node(root->user_data);
    }
}

// Function to print the tree using a user-defined print callback
void avlt_print(avlt_node_t *root, void (*print_node)(void *)) {
    if (root) {
        avlt_print(root->left, print_node);
        print_node(root->user_data);
        avlt_print(root->right, print_node);
    }
}

void avlt_free_tree(avlt_node_t *root, void (*del_data)(void *)) {
    if (root) {
        avlt_free_tree(root->left, del_data);  // Free left subtree
        avlt_free_tree(root->right, del_data); // Free right subtree
        if (del_data) {
            del_data(root->user_data); // Free user data if needed
        }
        QWISTYS_FREE(root); // Free the node itself
    }
}

#endif // QWISTYS_AVLT_IMPLEMENTATION

#ifdef __cplusplus
}
#endif

#endif // QWISTYS_AVLTREE_H

