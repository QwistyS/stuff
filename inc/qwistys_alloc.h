#ifndef QWISTYS_ALLOC_H
#define QWISTYS_ALLOC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "qwistys_api.h"

#include <stdint.h>
#include <stddef.h>

// Error codes
typedef enum {
    QWISTYS_ALLOC_SUCCESS = 0,
    QWISTYS_ALLOC_ERROR_INVALID_SIZE,
    QWISTYS_ALLOC_ERROR_OUT_OF_MEMORY,
    QWISTYS_ALLOC_ERROR_INVALID_POINTER,
    QWISTYS_ALLOC_ERROR_CORRUPTED_MEMORY
} qwistys_alloc_error_t;

// Global error variable
static qwistys_alloc_error_t qwistys_alloc_error;

// Alignment
#define QWISTYS_ALLOC_ALIGNMENT 16
#define QWISTYS_ALLOC_ALIGN(size) (((size) + (QWISTYS_ALLOC_ALIGNMENT - 1)) & ~(QWISTYS_ALLOC_ALIGNMENT - 1))

// Canary value
#define QWISTYS_ALLOC_CANARY 0xFFFFFACAUL

// Memory header and footer
typedef struct {
    uintptr_t canary;
    size_t size;
} qwistys_alloc_header_t;

typedef struct {
    uintptr_t canary;
} qwistys_alloc_footer_t;

// Callback type for custom canary settings
typedef void (*user_canary_settings)(qwistys_alloc_header_t *, qwistys_alloc_footer_t *);

// Function prototypes 
API_IMPL void* qwistys_malloc(size_t num_of_bytes, user_canary_settings callback);
API_IMPL void qwistys_free(void* pointer);
API_IMPL void* qwistys_calloc(size_t num, size_t size, user_canary_settings callback);
API_IMPL void* qwistys_realloc(void* ptr, size_t new_size, user_canary_settings callback);
API_IMPL size_t qwistys_get_allocated_size(void* ptr);
API_IMPL void qwistys_print_memory_stats(void);

#ifdef __cplusplus
}
#endif

#endif // QWISTYS_ALLOC_H
