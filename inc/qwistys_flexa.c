#include "qwistys_flexa.h"

static int flexa_resize(flexa_t *array, size_t new_capacity) {
  QWISTYS_DEBUG_MSG("Resizing array");
  QWISTYS_TELEMETRY_START();

  void *new_data = realloc(array->data, new_capacity * array->item_size);
  if (!new_data) {
    QWISTYS_HALT("Memory allocation failed during resize");
    return -1;
  }
  array->data = new_data;
  array->capacity = new_capacity;

  QWISTYS_DEBUG_MSG("Array resized successfully");
  QWISTYS_TELEMETRY_END();

  return 0;
}

flexa_t *flexa_init(size_t item_size, size_t initial_capacity) {
  QWISTYS_ASSERT(item_size > 0);
  QWISTYS_ASSERT(initial_capacity > 0);

  QWISTYS_TELEMETRY_START();

  flexa_t *array = (flexa_t*)malloc(sizeof(flexa_t));
  if (!array) {
    QWISTYS_HALT("Memory allocation failed during initialization");
    return NULL;
  }

  array->item_size = item_size;
  array->capacity = initial_capacity;
  array->size = 0;
  array->data = malloc(array->capacity * item_size);
  if (!array->data) {
    free(array);
    QWISTYS_HALT("Memory allocation failed during data initialization");
    return NULL;
  }

  QWISTYS_DEBUG_MSG("Array initialized successfully");
  QWISTYS_TELEMETRY_END();

  return array;
}

void flexa_free(flexa_t *array) {
  QWISTYS_ASSERT(array != NULL);

  QWISTYS_DEBUG_MSG("Freeing array");
  QWISTYS_TELEMETRY_START();

  if (array) {
    free(array->data);
    free(array);
  }

  QWISTYS_DEBUG_MSG("Array freed successfully");
  QWISTYS_TELEMETRY_END();
}

int flexa_add(flexa_t *array, const void *item) {
  QWISTYS_ASSERT(array != NULL);
  QWISTYS_ASSERT(item != NULL);

  QWISTYS_TELEMETRY_START();

  if (array->size >= array->capacity) {
    if (flexa_resize(array, array->capacity * 2) != 0) {
      return -1;
    }
  }

  void *destination = (char *)array->data + (array->size * array->item_size);
  memcpy(destination, item, array->item_size);
  array->size++;

  QWISTYS_DEBUG_MSG("Item added to array");
  QWISTYS_TELEMETRY_END();

  return 0;
}

void *flexa_get(flexa_t *array, size_t index) {
  QWISTYS_ASSERT(array != NULL);
  QWISTYS_BOUNDS_CHECK(index, array->size);

  QWISTYS_DEBUG_MSG("Fetching item from array");
  QWISTYS_TELEMETRY_START();
  void *item = (char *)array->data + (index * array->item_size);
  QWISTYS_TELEMETRY_END();

  return item;
}

size_t flexa_size(flexa_t *array) {
  QWISTYS_ASSERT(array != NULL);
  return array->size;
}

int flexa_remove(flexa_t *array, size_t index) {
  QWISTYS_ASSERT(array != NULL);
  QWISTYS_BOUNDS_CHECK(index, array->size);

  QWISTYS_TELEMETRY_START();

  void *destination = (char *)array->data + (index * array->item_size);
  void *source = (char *)array->data + ((index + 1) * array->item_size);
  size_t bytes_to_move = (array->size - index - 1) * array->item_size;
  memmove(destination, source, bytes_to_move);
  array->size--;

  QWISTYS_DEBUG_MSG("Item removed from array");
  QWISTYS_TELEMETRY_END();

  return 0;
}

void *flexa_get_raw_data(flexa_t *array) {
  QWISTYS_ASSERT(array != NULL);
  return array->data;
}