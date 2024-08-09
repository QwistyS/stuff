#ifndef QWISTYS_AVLTREE_H
#define QWISTYS_AVLTREE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "qwistys_macros.h"
#include "string.h"
#define QWISTYS_ALLOC_IMPLEMENTATION
#include "qwistys_alloc.h"

// ================================================
// AVL tree generic implementation
// ================================================

/**
 * @brief
 */
typedef struct avlt_node_t {
  void *user_data;
  uint32_t height;
  struct avlt_node_t *left;
  struct avlt_node_t *right;
  struct avlt_node_t *parent;
} avlt_node_t;

// Calbacks user defined
/**
 * @brief Usewr must to provide a callback for 2 nodes compear
 * Example: Node A == Node B 0 True -1 false;
 */
typedef int (*compare_func)(void *, void *);

/**
 * @brief User defined cleanup ... in case the user stuff have allocated mem.
 */
typedef void (*delete_data_func)(void *);

/**
 * @brief
 */
avlt_node_t *avlt_create_node(size_t user_data_length_in_bytes); // :D

/**
 * @brief
 */
uint32_t avlt_get_hight(avlt_node_t *node);

/**
 * @brief
 */
int avlt_get_balance(avlt_node_t *node);

/**
 * @brief
 */
avlt_node_t *avlt_right_rotate(avlt_node_t *y_node);

/**
 * @brief
 */
avlt_node_t *avlt_left_rotate(avlt_node_t *x_node);

#ifdef QWISTYS_ALVTREE_IMPLEMENTATION

avlt_node_t *avlt_create_node(size_t user_data_length_in_bytes) {
  avlt_node_t *tmp;

  QWISTYS_ASSERT(user_data_length_in_bytes != 0);
  tmp->user_data = (void *)qwistys_malloc(user_data_length_in_bytes, NULL);
  QWISTYS_TODO_MSG("Add cannary at the end of the user_data pointer");

  return tmp;
}

uint32_t avlt_get_hight(avlt_node_t *node) {
  if (node == NULL) {
    return 0;
  }
  return node->height;
}

int avlt_get_balance(avlt_node_t *node) {
  if(node == NULL) {
    return 0;
  }
  // if left and right same level then 0 which this node is balanced. else not
  return avlt_get_hight(node->left) - avlt_get_hight(node->right);
}

avlt_node_t *avlt_right_rotate(avlt_node_t *y_node) {
  // Temps
  avlt_node_t* x_node = y_node->left;
  avlt_node_t* tmp_node = x_node->right;

  // Rotation
  x_node->right = y_node;
  y_node->left = tmp_node;

  // Update parent
  x_node->parent = y_node->parent;
  y_node->parent = x_node;
  if (tmp_node != NULL) {
    tmp_node->parent = y_node;
  }

  // Update heightgs
  y_node->height = 1 + (avlt_get_hight(y_node->left) > avlt_get_hight(y_node->right)) ? avlt_get_hight(y_node->left) : avlt_get_hight(y_node->right);
  x_node->height = 1 + (avlt_get_hight(x_node->left) > avlt_get_hight(x_node->right)) ? avlt_get_hight(x_node->left) : avlt_get_hight(x_node->right);

  return x_node;
}


#endif // QWISTYS_ALVTREE_IMPLEMENTATION

#ifdef __cplusplus
}
#endif

#endif // QQWISTYS_AVLTREE_H
