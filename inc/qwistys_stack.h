#ifndef QWISTYS_STACK_H
#define QWISTYS_STACK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "qwistys_api.h"
#include "qwistys_flexa.h"

// Stack structure using flexa
typedef struct {
    flexa_t *data;
} qwistys_stack_t;

// Function prototypes
API_IMPL qwistys_stack_t *qwistys_stack_init(size_t item_size, size_t initial_capacity);
API_IMPL void qwistys_stack_free(qwistys_stack_t *stack);
API_IMPL int qwistys_stack_push(qwistys_stack_t *stack, const void *item);
API_IMPL int qwistys_stack_pop(qwistys_stack_t *stack, void *item);
API_IMPL int qwistys_stack_peek(qwistys_stack_t *stack, void *item);
API_IMPL size_t qwistys_stack_size(qwistys_stack_t *stack);
API_IMPL int qwistys_stack_is_empty(qwistys_stack_t *stack);

#ifdef __cplusplus
}
#endif

#endif // QWISTYS_STACK_H
