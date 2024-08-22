#ifndef QWISTYS_FLEXA_H
#define QWISTYS_FLEXA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "qwistys_macros.h"
#include "string.h"

// ================================================
// Dynamic Array
// ================================================

typedef struct {
  size_t item_size; // Size of each item
  size_t capacity;  // Allocated memory in number of items
  size_t size;      // Number of items currently in the array
  void *data;       // Pointer to the data
} flexa_t;

/**
 * @brief Initialize thh dinamic array
 * @param item_size lenght of data in bytes_to_move
 * @param initial_capacity amount of items to start with
 * @return pointer to structure of the flexa or NULL in case of failed
 */
flexa_t *flexa_init(size_t item_size, size_t initial_capacity);

/**
 * @brief Release the mem of the flexa
 */
void flexa_free(flexa_t *array);

/**
 * @brief add item to the array
 * @param array the struct of flexa
 * @param item the user data to put in
 * @return 0 on success -1 on fail
 */
int flexa_add(flexa_t *array, const void *item);

/**
 * @brief get address to item by index
 * @param array the array struct
 * @param index item index
 * @return pointer to mem of the item
 */
void *flexa_get(flexa_t *array, size_t index);

/**
 * @brief get the current size of the array
 * @note will return the ammount of items in the array and not how much was
 * Allocated
 * @return int effective size
 */
size_t flexa_size(flexa_t *array);

/**
 * @brief Remove the item from an array
 * @param array the struct to array
 * @param index the index of the item to remove
 * @return int 0 on success -1 on fail
 */
int flexa_remove(flexa_t *array, size_t index);

/**
 * @brief Getting access to raw array
 * @note to use in standart for/while loop for CPU caching
 * advantage
 * @note cast it to your data type
 * @param array the struct to array
 * @return pointer to first address of the actuall array
 */
void *flexa_get_raw_data(flexa_t *array);

#ifdef __cplusplus
}
#endif

#endif // QWISTYS_FLEXA_H
