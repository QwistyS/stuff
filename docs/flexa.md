# NAME
flexa - A dynamic flexible array implementation

# SYNOPSIS
```c
#include "qwistys_flexa.h"

void *flexa_create(size_t initial_capacity, size_t element_size);
void flexa_destroy(void *array);
void *flexa_append(void *array, void *element);
size_t flexa_size(void *array);
size_t flexa_capacity(void *array);
void *get_raw_array(void *array);
```
## DESCRIPTION
The flexa library provides a dynamic array implementation that allows for flexible resizing and easy management of elements.

Functions
flexa_create
```c
void *flexa_create(size_t initial_capacity, size_t element_size);
```
Creates a new dynamic array with the specified initial capacity and element size. The array can hold any type of element as long as the size is specified.

flexa_destroy
```c
void flexa_destroy(void *array);
```
Frees the memory allocated for the array and its elements.

flexa_append
```c
void *flexa_append(void *array, void *element);
```
Appends an element to the dynamic array. If the array's capacity is exceeded, it automatically resizes to accommodate the new element.

flexa_size
```c
size_t flexa_size(void *array);
```
Returns the current number of elements in the array.

flexa_capacity
```c
size_t flexa_capacity(void *array);
```
Returns the current capacity of the array, which is the number of elements it can hold before needing to resize.

get_raw_array
```c
void *get_raw_array(void *array);
```
Returns a pointer to the underlying raw array. This allows direct access to the memory block, bypassing the dynamic array's management functions.

## RETURN VALUE
flexa_create: On success, returns a pointer to the newly created array. On failure, returns NULL.
flexa_destroy: Returns no value.
flexa_append: On success, returns a pointer to the array with the new element appended. If the array needed to be resized, the pointer may change. On failure, returns NULL.
flexa_size: Returns the size of the array.
flexa_capacity: Returns the capacity of the array.
get_raw_array: Returns a pointer to the raw memory block of the array.

##EXAMPLES
Example 1: Basic Dynamic Array Usage
```c
// define QWISTYS_*_IMPLEMENTATION or implement this functions in *.c file
#include "qwistys_flexa.h"

int main() {
    int *array;

    array = flexa_create(10, sizeof(int));  // Create an array with an initial capacity of 10 ints
    if (array == NULL) {
        // Handle allocation failure
    }

    int value = 42;
    flexa_append(array, &value);  // Append value to the array

    size_t size = flexa_size(array);  // Get the current size
    size_t capacity = flexa_capacity(array);  // Get the current capacity

    int *raw_array = get_raw_array(array);  // Get the raw array pointer

    flexa_destroy(array);  // Destroy the array

    return 0;
}
```
## NOTES
The array automatically resizes when its capacity is exceeded during appending.
The array is generic and can hold any type of element as long as the correct size is specified during creation.
The get_raw_array function provides direct access to the underlying array, which can be useful for performance-critical code but should be used with caution as it bypasses the safety mechanisms of the dynamic array.
## SEE ALSO

