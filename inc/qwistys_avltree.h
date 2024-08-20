#ifndef QWISTYS_AVLTREE_H
#define QWISTYS_AVLTREE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef STATIC_API
#define API_IMPL static inline
#else
#define API_IMPL extern
#endif

#include "qwistys_macros.h"

// AVL tree node structure
typedef struct avlt_node_t {
    void *user_data;
    uint32_t height;
    struct avlt_node_t *left;
    struct avlt_node_t *right;
    struct avlt_node_t *parent;
} avlt_node_t;

// Thread-safe related declarations
typedef struct qwistys_mutex_t qwistys_mutex_t;

typedef void (*qwistys_mutex_init_fn)(qwistys_mutex_t *mutex);
typedef void (*qwistys_mutex_destroy_fn)(qwistys_mutex_t *mutex);
typedef void (*qwistys_mutex_lock_fn)(qwistys_mutex_t *mutex);
typedef void (*qwistys_mutex_unlock_fn)(qwistys_mutex_t *mutex);

typedef struct {
    avlt_node_t *root;
    qwistys_mutex_t *mutex;
    qwistys_mutex_init_fn mutex_init;
    qwistys_mutex_destroy_fn mutex_destroy;
    qwistys_mutex_lock_fn mutex_lock;
    qwistys_mutex_unlock_fn mutex_unlock;
} avl_tree_t;

// Function prototypes
API_IMPL avlt_node_t *avlt_create_node(size_t user_data_length_in_bytes);
API_IMPL uint32_t avlt_get_height(avlt_node_t *node);
API_IMPL int avlt_get_balance(avlt_node_t *node);
API_IMPL avlt_node_t *avlt_right_rotate(avlt_node_t *y);
API_IMPL avlt_node_t *avlt_left_rotate(avlt_node_t *x);
API_IMPL avlt_node_t *avlt_insert(avlt_node_t *node, void *user_data, size_t data_length, int (*cmp)(void *, void *));
API_IMPL avlt_node_t *avlt_min_value_node(avlt_node_t *node);
API_IMPL avlt_node_t *avlt_delete(avlt_node_t *root, void *user_data, int (*cmp)(void *, void *), void (*del_data)(void *));
API_IMPL void avlt_pre_order(avlt_node_t *root, void (*process_node)(void *));
API_IMPL void avlt_in_order(avlt_node_t *root, void (*process_node)(void *));
API_IMPL void avlt_post_order(avlt_node_t *root, void (*process_node)(void *));
API_IMPL void avlt_print(avlt_node_t *root, void (*print_node)(void *));
API_IMPL void avlt_free_tree(avlt_node_t *root, void (*del_data)(void *));

// Function prototypes for thread-safe operations
API_IMPL void avl_tree_init(avl_tree_t *tree, qwistys_mutex_t *mutex,
                   qwistys_mutex_init_fn init_fn,
                   qwistys_mutex_destroy_fn destroy_fn,
                   qwistys_mutex_lock_fn lock_fn,
                   qwistys_mutex_unlock_fn unlock_fn);
API_IMPL avlt_node_t *avl_tree_insert(avl_tree_t *tree, void *user_data, size_t data_length, int (*cmp)(void *, void *));
API_IMPL avlt_node_t *avl_tree_delete(avl_tree_t *tree, void *user_data, int (*cmp)(void *, void *), void (*del_data)(void *));
API_IMPL void avl_tree_free(avl_tree_t *tree, void (*del_data)(void *));

#ifdef __cplusplus
}
#endif

#endif // QWISTYS_AVLTREE_H
