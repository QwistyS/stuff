# NAME
qwistys_alloc - A custom memory allocator with canary values

# SYNOPSIS
```c
#include "qwistys_alloc.h"

void *qwistys_alloc(size_t size);
void qwistys_free(void *ptr);
```

## DESCRIPTION
qwistys_alloc provides functions for memory allocation with additional features such as canary values to detect buffer overflows.

Memory Allocation
The qwistys_alloc() function allocates a block of memory of size size bytes. The allocated memory is not initialized.

Memory Deallocation
The qwistys_free() function frees a block of memory previously allocated by qwistys_alloc().

RETURN VALUE
On success, qwistys_alloc() returns a pointer to the allocated memory. On failure, it returns NULL.

qwistys_free() returns no value.

## EXAMPLES
Example 1: Basic Memory Allocation

```c
#include "qwistys_alloc.h"

int main() {
    void *ptr;

    ptr = qwistys_alloc(1024);  // Allocate 1024 bytes
    if (ptr == NULL) {
        // Handle allocation failure
    }

    // Use allocated memory

    qwistys_free(ptr);  // Free allocated memory

    return 0;
}
```
## NOTES
The canary values used by qwistys_alloc help detect buffer overflows. If an overflow occurs, an error is logged, and the program may abort based on the configuration.

## SEE ALSO
malloc(3), free(3)
