
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
Creating node with custom allocated mem for user data.
- param: *size_t* user data length in bytes
#note with custome allocator ... see qwistys_alloc.h

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

```c
avlt_node_t *avlt_insert(avlt_node_t *node, void *user_data, size_t data_length, int (*cmp)(void *, void *));
```
Indesting a node in place.
- param *avlt_node_t** pointer to node.
- param *void** pointer to user data.
- param *size_t* data length in bytes.
- param *callback* user defined callback to compare between nodes.



## RETURN VALUE

##EXAMPLES
Example 1: Basic Avl tree Usage
```c
// define QWISTYS_*_IMPLEMENTATION or implement this functions in *.c file
#include "qwistys_avltree.h"

int main() {

    typedef struct {
        int id;
        char name[100];
    } user_data_t;

    void print_user_data(void* data) {
        user_data_t* pd = (user_data_t*) data;
        QWISTYS_DEBUG_MSG("Id = %d Name = %s", pd->id, pd->name);
    }

    int compare(void* a, void* b) {
        user_data_t* pa = (user_data_t*) a;
        user_data_t* pb = (user_data_t*) b;
        return pa->id - pb->id;
    }
    
    void delet_data(void* data) {
    user_data_t* pdata = (user_data_t*)data;
    QWISTYS_DEBUG_MSG("Prepering to delete data {id=%d, Name=%s}", pdata->id, pdata->name);
  }

    user_data_t user_array[] = {{1, "Daniel Mor"},
    {2, "Valentina Yagmorov"},
    {3, "Yuval Mor"},
    {4, "some other dude"},
    {12, "some other dude"},
    {11, "A cool person"},
    {7, "Shmok person"},
    {14, "Poor person"},
    {7, "Rich person"},
  };

    avlt_node_t* root = NULL;

    for (int i = 0; i < QWISTYS_ARRAY_LEN(user_array); i++) {
        root = avlt_insert(root, &user_array[i], sizeof(user_data_t), compare);
    }

    avlt_print(root, print_user_data);

    avlt_delete(root, &user_array[2], compare, delet_data);

    avlt_print(root, print_user_data);
    QWISTYS_DEBUG_MSG("______________  AVL TREE END ______________________");

    return 0;
}
```
## NOTES
## SEE ALSO

