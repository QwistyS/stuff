#include "qwistys_stack.h"
#include "qwistys_alloc.h"

qwistys_stack_t *qwistys_stack_init(size_t item_size, size_t initial_capacity) {
    QWISTYS_TELEMETRY_START();
    qwistys_stack_t *stack = (qwistys_stack_t *)qwistys_malloc(sizeof(qwistys_stack_t), NULL);
    if (!stack) {
        QWISTYS_HALT("Memory allocation failed for stack");
        return NULL;
    }
    
    stack->data = flexa_init(item_size, initial_capacity);
    if (!stack->data) {
        qwistys_free(stack);
        QWISTYS_HALT("Memory allocation failed for stack data");
        return NULL;
    }
    
    QWISTYS_DEBUG_MSG("Stack initialized successfully");
    QWISTYS_TELEMETRY_END();
    return stack;
}

void qwistys_stack_free(qwistys_stack_t *stack) {
    QWISTYS_ASSERT(stack != NULL);
    QWISTYS_TELEMETRY_START();
    
    flexa_free(stack->data);
    qwistys_free(stack);
    
    QWISTYS_DEBUG_MSG("Stack freed successfully");
    QWISTYS_TELEMETRY_END();
}

int qwistys_stack_push(qwistys_stack_t *stack, const void *item) {
    QWISTYS_ASSERT(stack != NULL);
    QWISTYS_ASSERT(item != NULL);
    QWISTYS_TELEMETRY_START();
    
    int result = flexa_add(stack->data, item);
    
    QWISTYS_DEBUG_MSG(result == 0 ? "Item pushed to stack" : "Failed to push item to stack");
    QWISTYS_TELEMETRY_END();
    return result;
}

int qwistys_stack_pop(qwistys_stack_t *stack, void *item) {
    QWISTYS_ASSERT(stack != NULL);
    QWISTYS_ASSERT(item != NULL);
    QWISTYS_TELEMETRY_START();
    
    if (qwistys_stack_is_empty(stack)) {
        QWISTYS_DEBUG_MSG("Attempted to pop from empty stack");
        QWISTYS_TELEMETRY_END();
        return -1;
    }
    
    size_t last_index = flexa_size(stack->data) - 1;
    memcpy(item, flexa_get(stack->data, last_index), stack->data->item_size);
    int result = flexa_remove(stack->data, last_index);
    
    QWISTYS_DEBUG_MSG(result == 0 ? "Item popped from stack" : "Failed to pop item from stack");
    QWISTYS_TELEMETRY_END();
    return result;
}

int qwistys_stack_peek(qwistys_stack_t *stack, void *item) {
    QWISTYS_ASSERT(stack != NULL);
    QWISTYS_ASSERT(item != NULL);
    QWISTYS_TELEMETRY_START();
    
    if (qwistys_stack_is_empty(stack)) {
        QWISTYS_DEBUG_MSG("Attempted to peek at empty stack");
        QWISTYS_TELEMETRY_END();
        return -1;
    }
    
    size_t last_index = flexa_size(stack->data) - 1;
    memcpy(item, flexa_get(stack->data, last_index), stack->data->item_size);
    
    QWISTYS_DEBUG_MSG("Peeked at top item of stack");
    QWISTYS_TELEMETRY_END();
    return 0;
}

size_t qwistys_stack_size(qwistys_stack_t *stack) {
    QWISTYS_ASSERT(stack != NULL);
    return flexa_size(stack->data);
}

int qwistys_stack_is_empty(qwistys_stack_t *stack) {
    QWISTYS_ASSERT(stack != NULL);
    return flexa_size(stack->data) == 0;
}