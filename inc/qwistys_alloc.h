
#ifndef QWISTYS_ALLOC_H
#define QWISTYS_ALLOC_H

#ifdef __cplusplus
extern "C" {
#endif

#define CANARY 0xFFFFFACA

#include "qwistys_macros.h"
#include "string.h"

// ================================================
// Custume allocator.
// ================================================

/**
 * @brief Struct prefix to user pointer
 */
typedef struct {
  uint32_t canary;  // A canary value you want ot hold, suggest a hash func from
                    // add to 4 byte value;
  size_t size_user; // The users ammount of bytes.
} mem_header_t;

/**
 * @brief Struct postfix of user data, add stuff as you wish.
 */
typedef struct {
  uint32_t canary; // Value to validate is overflow happened.
  size_t size; // Size of data pointer in the struct !!! @Note Not tested not
               // calculated, if you want to be my gust to play with it .... tho
               // no need to do anything except to clear it somehow.
  char data[]; // Will in future add some data table to whom this mem was
               // allocated and more info like time(NULL) and stuff
} mem_footer_t;

/**
 * @brief a callback from user in case user want to edit the footer/header
 * pre/post fix
 */
typedef void (*user_canary_settings)(mem_header_t *, mem_footer_t *);

/**
 * @brief Allocating standart malloc func mem, adding pre post fix's.
 * @param num_of_bytes Lenght of desired mem in bytes
 * @param user_canary_settings A callback if needed access to header/footer
 * structures, pass NULL by default
 * @return void* pointer to chunk of mem. Cast it to needed type
 */
void *qwistys_malloc(size_t num_of_bytes, user_canary_settings callback);

/**
 * @brief freeing mem allocated by qwistys_malloc, IDK what will happen if
 * regular pointer will be passed so DONT!
 */
void qwistys_free(void *pointer);

#ifdef QWISTYS_ALLOC_IMPLEMENTATION

void *qwistys_malloc(size_t num_of_bytes, user_canary_settings callback) {
  QWISTYS_TELEMETRY_START();
  QWISTYS_ASSERT(num_of_bytes != 0);
  QWISTYS_DEBUG_MSG("Trying to allocate %d bytes", num_of_bytes);
  size_t ttl_size = sizeof(mem_header_t) + sizeof(mem_header_t) + num_of_bytes;
  char *tmp = (char *)malloc(ttl_size);
  if (tmp == NULL) {
    QWISTYS_DEBUG_MSG("Fail to allocate mem");
    return NULL;
  }

  mem_header_t *head = (mem_header_t *)tmp;
  mem_footer_t *footer =
      (mem_footer_t *)((char *)head + sizeof(mem_header_t) + num_of_bytes);
  if (!callback) {
    head->canary = CANARY;
    head->size_user = num_of_bytes;
    footer->canary = CANARY;
  } else {
    QWISTYS_DEBUG_MSG("Calling user callback on canary");
    callback(head, footer);
  }
  QWISTYS_TELEMETRY_END();
  QWISTYS_DEBUG_MSG("Success allocating %d bytes", num_of_bytes);
  return tmp + sizeof(mem_header_t);
}

void qwistys_free(void *pointer) {
  QWISTYS_ASSERT(pointer);
  QWISTYS_TELEMETRY_START();
  char *line = (char *)pointer;
  QWISTYS_DEBUG_MSG("Trying to free pointer %#08x", line);
  line = line - sizeof(mem_header_t);

  mem_header_t *head = (mem_header_t *)line;

  if (head->canary != CANARY) {
    QWISTYS_HALT("header cannay of pointer is corupt");
  }

  mem_footer_t *footer =
      (mem_footer_t *)((char *)line + sizeof(mem_header_t) + head->size_user);
  if (footer->canary != CANARY) {
    QWISTYS_HALT("footer cannay of pointer is corupt");
  }

  free(line);
  QWISTYS_DEBUG_MSG("Success to free pointer %#08x", line);
  QWISTYS_TELEMETRY_END();
}

#endif // QWISTYS_ALLOC_IMPLEMENTATION

#ifdef __cplusplus
}
#endif

#endif // QWISTYS_ALLOC_H
