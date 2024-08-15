#ifndef QWISTYS_ALLOC_H
#define QWISTYS_ALLOC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "qwistys_macros.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>

// Error codes
typedef enum {
    QWISTYS_ALLOC_SUCCESS = 0,
    QWISTYS_ALLOC_ERROR_INVALID_SIZE,
    QWISTYS_ALLOC_ERROR_OUT_OF_MEMORY,
    QWISTYS_ALLOC_ERROR_INVALID_POINTER,
    QWISTYS_ALLOC_ERROR_CORRUPTED_MEMORY
} qwistys_alloc_error_t;

// Global error variable
extern qwistys_alloc_error_t qwistys_alloc_error;

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
void* qwistys_malloc(size_t num_of_bytes, user_canary_settings callback);
void qwistys_free(void* pointer);
void* qwistys_calloc(size_t num, size_t size, user_canary_settings callback);
void* qwistys_realloc(void* ptr, size_t new_size, user_canary_settings callback);
size_t qwistys_get_allocated_size(void* ptr);
void qwistys_print_memory_stats(void);

#ifdef QWISTYS_ALLOC_IMPLEMENTATION

#include <stdlib.h>

// Global variables for memory tracking
static size_t total_allocated = 0;
static size_t total_freed = 0;
static size_t current_usage = 0;
static size_t peak_usage = 0;

qwistys_alloc_error_t qwistys_alloc_error = QWISTYS_ALLOC_SUCCESS;

static void* qwistys_alloc_internal(size_t num_of_bytes, user_canary_settings callback) {
    QWISTYS_TELEMETRY_START();
    QWISTYS_ASSERT(num_of_bytes != 0);
    QWISTYS_DEBUG_MSG("Trying to allocate %zu bytes", num_of_bytes);

    size_t total_size = sizeof(qwistys_alloc_header_t) + QWISTYS_ALLOC_ALIGN(num_of_bytes) + sizeof(qwistys_alloc_footer_t);
    char* block = (char*)malloc(total_size);
    if (block == NULL) {
        qwistys_alloc_error = QWISTYS_ALLOC_ERROR_OUT_OF_MEMORY;
        QWISTYS_DEBUG_MSG("Failed to allocate memory");
        QWISTYS_TELEMETRY_END();
        return NULL;
    }

    qwistys_alloc_header_t* header = (qwistys_alloc_header_t*)block;
    qwistys_alloc_footer_t* footer = (qwistys_alloc_footer_t*)(block + sizeof(qwistys_alloc_header_t) + QWISTYS_ALLOC_ALIGN(num_of_bytes));

    if (!callback) {
        header->canary = QWISTYS_ALLOC_CANARY;
        header->size = num_of_bytes;
        footer->canary = QWISTYS_ALLOC_CANARY;
    } else {
        QWISTYS_DEBUG_MSG("Calling user callback on canary");
        callback(header, footer);
    }

    total_allocated += num_of_bytes;
    current_usage += num_of_bytes;
    if (current_usage > peak_usage) {
        peak_usage = current_usage;
    }

    QWISTYS_DEBUG_MSG("Successfully allocated %zu bytes", num_of_bytes);
    QWISTYS_TELEMETRY_END();
    return block + sizeof(qwistys_alloc_header_t);
}

void* qwistys_malloc(size_t num_of_bytes, user_canary_settings callback) {
    return qwistys_alloc_internal(num_of_bytes, callback);
}

void qwistys_free(void* pointer) {
    QWISTYS_TELEMETRY_START();
    if (!pointer) {
        QWISTYS_DEBUG_MSG("Attempt to free NULL pointer");
        QWISTYS_TELEMETRY_END();
        return;
    }

    char* block = (char*)pointer - sizeof(qwistys_alloc_header_t);
    qwistys_alloc_header_t* header = (qwistys_alloc_header_t*)block;
    qwistys_alloc_footer_t* footer = (qwistys_alloc_footer_t*)(block + sizeof(qwistys_alloc_header_t) + QWISTYS_ALLOC_ALIGN(header->size));

    QWISTYS_ASSERT(header->canary == QWISTYS_ALLOC_CANARY && footer->canary == QWISTYS_ALLOC_CANARY);

    if (header->canary != QWISTYS_ALLOC_CANARY || footer->canary != QWISTYS_ALLOC_CANARY) {
        qwistys_alloc_error = QWISTYS_ALLOC_ERROR_CORRUPTED_MEMORY;
        QWISTYS_DEBUG_MSG("Corrupted memory detected at %p", pointer);
        QWISTYS_HALT("Corrupted memory detected");
    }

    total_freed += header->size;
    current_usage -= header->size;

    QWISTYS_DEBUG_MSG("Successfully freed %zu bytes at %p", header->size, pointer);
    free(block);
    QWISTYS_TELEMETRY_END();
}

void* qwistys_calloc(size_t num, size_t size, user_canary_settings callback) {
    QWISTYS_TELEMETRY_START();
    
    size_t total_size;
    if (__builtin_mul_overflow(num, size, &total_size)) {
        qwistys_alloc_error = QWISTYS_ALLOC_ERROR_INVALID_SIZE;
        QWISTYS_DEBUG_MSG("Integer overflow in calloc: num=%zu, size=%zu", num, size);
        QWISTYS_TELEMETRY_END();
        return NULL;
    }

    void* ptr = qwistys_alloc_internal(total_size, callback);
    if (ptr) {
        memset(ptr, 0, total_size);
    }
    
    QWISTYS_TELEMETRY_END();
    return ptr;
}

void* qwistys_realloc(void* ptr, size_t new_size, user_canary_settings callback) {
    QWISTYS_TELEMETRY_START();
    
    if (!ptr) {
        QWISTYS_TELEMETRY_END();
        return qwistys_malloc(new_size, callback);
    }

    if (new_size == 0) {
        qwistys_free(ptr);
        QWISTYS_TELEMETRY_END();
        return NULL;
    }

    qwistys_alloc_header_t* header = (qwistys_alloc_header_t*)((char*)ptr - sizeof(qwistys_alloc_header_t));
    QWISTYS_ASSERT(header->canary == QWISTYS_ALLOC_CANARY);

    if (header->canary != QWISTYS_ALLOC_CANARY) {
        qwistys_alloc_error = QWISTYS_ALLOC_ERROR_CORRUPTED_MEMORY;
        QWISTYS_DEBUG_MSG("Corrupted memory detected at %p", ptr);
        QWISTYS_HALT("Corrupted memory detected");
    }

    void* new_ptr = qwistys_malloc(new_size, callback);
    if (!new_ptr) {
        QWISTYS_TELEMETRY_END();
        return NULL;
    }

    size_t copy_size = (new_size < header->size) ? new_size : header->size;
    memcpy(new_ptr, ptr, copy_size);
    qwistys_free(ptr);

    QWISTYS_TELEMETRY_END();
    return new_ptr;
}

size_t qwistys_get_allocated_size(void* ptr) {
    QWISTYS_TELEMETRY_START();
    
    if (!ptr) {
        qwistys_alloc_error = QWISTYS_ALLOC_ERROR_INVALID_POINTER;
        QWISTYS_DEBUG_MSG("Attempt to get size of NULL pointer");
        QWISTYS_TELEMETRY_END();
        return 0;
    }

    qwistys_alloc_header_t* header = (qwistys_alloc_header_t*)((char*)ptr - sizeof(qwistys_alloc_header_t));
    QWISTYS_ASSERT(header->canary == QWISTYS_ALLOC_CANARY);

    if (header->canary != QWISTYS_ALLOC_CANARY) {
        qwistys_alloc_error = QWISTYS_ALLOC_ERROR_CORRUPTED_MEMORY;
        QWISTYS_DEBUG_MSG("Corrupted memory detected at %p", ptr);
        QWISTYS_HALT("Corrupted memory detected");
    }

    QWISTYS_TELEMETRY_END();
    return header->size;
}

void qwistys_print_memory_stats(void) {
    QWISTYS_TELEMETRY_START();
    
    QWISTYS_DEBUG_MSG("Memory Statistics:");
    QWISTYS_DEBUG_MSG("Total Allocated: %zu bytes", total_allocated);
    QWISTYS_DEBUG_MSG("Total Freed: %zu bytes", total_freed);
    QWISTYS_DEBUG_MSG("Current Usage: %zu bytes", current_usage);
    QWISTYS_DEBUG_MSG("Peak Usage: %zu bytes", peak_usage);
    
    QWISTYS_TELEMETRY_END();
}

#endif // QWISTYS_ALLOC_IMPLEMENTATION

#ifdef __cplusplus
}
#endif

#endif // QWISTYS_ALLOC_H
