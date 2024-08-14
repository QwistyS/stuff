
# NAME
Avl Tree - A balanced tree.

# SYNOPSIS
```c
#include "qwistys_avltree.h"
/**
* @note must to implement fallowing callbacks
* int (*compare)(void*, void*); To compare your custome data structure.
* void (*delete_data)(void*); To delete any dinamic alloc if has any, or anything need to be done MUST IMPLEMNT even is nothing should be done.
* void (*print_done)(void*): handle your custome data and pring for debug stuff.
*/


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
```
## DESCRIPTION

Functions
```c
avlt_node_t *avlt_create_node(size_t user_data_length_in_bytes);
```
Creating node with custom allocated mem for user data
@param size_t user data length in bytes
@note with custome allocator ... see qwistys_alloc.----------

```c
uint32_t avlt_get_height(avlt_node_t *node);
```
internal ... getting hight of the node.

```c
int avlt_get_balance(avlt_node_t *node);
```
internal ... is balanced or get balance.

```c
avlt_node_t *avlt_right_rotate(avlt_node_t *y);
```
internal ... Right rotate the node 
    //       (5)            4     
    //       / \           / \    
    //      4   D         /   \   
    //     / \           3     5  
    //    3   C    -->  / \   / \ 
    //   / \           A   B C   D
    //  A   B                     

```c
avlt_node_t *avlt_left_rotate(avlt_node_t *x);
```
internal ... Left rotate the node
    //    (3)               4     
    //    / \              / \    
    //   A   4            /   \   
    //      / \          3     5  
    //     B   5   -->  / \   / \ 
    //        / \      A   B C   D
    //       C   D  
## RETURN VALUE

##EXAMPLES
Example 1: Basic Avl tree Usage
```c
// define QWISTYS_*_IMPLEMENTATION or implement this functions in *.c file
#include "qwistys_avltree.h"

int main() {

    return 0;
}
```
## NOTES
## SEE ALSO

