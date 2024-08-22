#ifndef __QWISTYS_MACROS_H
#define __QWISTYS_MACROS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ANSI color codes for logging
#define QWISTYS_NRM "\x1B[0m"
#define QWISTYS_RED "\x1B[31m"
#define QWISTYS_GRN "\x1B[32m"
#define QWISTYS_YLW "\x1B[33m"
#define QWISTYS_BLU "\x1B[34m"
#define QWISTYS_MAG "\x1B[35m"
#define QWISTYS_CYN "\x1B[36m"
#define QWISTYS_WHT "\x1B[37m"

#define QWISTYS_MACRONAME "[QWISTYS MACRO]"
#define QWISTYS_TAG_TODO "\x1B[36mTODO: \x1B[33m"
#define QWISTYS_TAG_DEBUG "\x1B[32m[DEBUG] \x1B[33m"
#define QWISTYS_TAG_IMPLEMENTED "NOT IMPLEMENTED "
#define QWISTYS_TAG_HALT "\x1B[31mHALT: \x1B[34m"
#define QWISTYS_TAG_SE "SET EQUAL"
#define QWISTYS_TAG_AB "ARRAY BOUND"
#define QWISTYS_TAG_NULL "NULL VALUE"

// Log levels
typedef enum {
  QWISTYS_LOG_LEVEL_DEBUG,
  QWISTYS_LOG_LEVEL_INFO,
  QWISTYS_LOG_LEVEL_WARN,
  QWISTYS_LOG_LEVEL_ERROR,
} qwistys_log_level_t;

// Log file pointer
static FILE *log_file = NULL;

// Function to set the log file
static inline void set_log_file(const char *file_path) {
  if (log_file) {
    fclose(log_file);
  }
  log_file = fopen(file_path, "a");
  if (!log_file) {
    fprintf(stderr, "Failed to open log file: %s\n", file_path);
    exit(EXIT_FAILURE);
  }
}

static inline void qwistys_default_log(qwistys_log_level_t level, const char *file,
                                const char *function, int line, const char *tag,
                                const char *fmt, ...) {
  const char *color;
  switch (level) {
  case QWISTYS_LOG_LEVEL_DEBUG:
    color = QWISTYS_CYN;
    break;
  case QWISTYS_LOG_LEVEL_INFO:
    color = QWISTYS_GRN;
    break;
  case QWISTYS_LOG_LEVEL_WARN:
    color = QWISTYS_YLW;
    break;
  case QWISTYS_LOG_LEVEL_ERROR:
    color = QWISTYS_RED;
    break;
  default:
    color = QWISTYS_NRM;
  }
  FILE *output = log_file ? log_file : stderr;
  fprintf(output, "%s%s %s%s:%s:%d %s", color, QWISTYS_MACRONAME, QWISTYS_MAG,
          file, function, line, tag);

  va_list args;
  va_start(args, fmt);
  vfprintf(output, fmt, args);
  va_end(args);

  fprintf(output, "%s\n", QWISTYS_NRM);
  fflush(output);
}

// Pointer to the logging function (can be overridden)
static void (*qwistys_log)(qwistys_log_level_t level, const char *file,
                           const char *function, int line, const char *tag,
                           const char *fmt, ...) = qwistys_default_log;

#define QWISTYS_MSG(level, tag, fmt, ...)                                      \
  qwistys_log(level, __FILE__, __func__, __LINE__, tag, fmt, ##__VA_ARGS__)

#define QWISTYS_HALT(tag)                                                      \
  {                                                                            \
    QWISTYS_MSG(QWISTYS_LOG_LEVEL_ERROR, QWISTYS_TAG_HALT, tag);               \
    exit(EXIT_FAILURE);                                                        \
  }

#define QWISTYS_DEFER(x)                                                       \
  do {                                                                         \
    result = (x);                                                              \
    goto defer;                                                                \
  } while (0)

#define QWISTYS_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define QWISTYS_MAX(a, b) (((a) > (b)) ? (a) : (b))

#define QWISTYS_TODO_ENABLE
#ifdef QWISTYS_TODO_ENABLE
#ifdef QWISTYS_TODO_FORCE
#define QWISTYS_TODO_MSG(msg) QWISTYS_HALT(msg)
#else
#define QWISTYS_TODO_MSG(msg)                                                  \
  QWISTYS_MSG(QWISTYS_LOG_LEVEL_WARN, QWISTYS_TAG_TODO, msg)
#endif /* QWISTYS_TODO_FORCE */
#else
#define QWISTYS_TODO_MSG(msg)
#endif /*QWISTYS_TODO*/

#define QWISTYS_UNIMPLEMENTED()                                                \
  do {                                                                         \
    QWISTYS_MSG(QWISTYS_LOG_LEVEL_WARN, QWISTYS_TAG_IMPLEMENTED, "YET")        \
    QWISTYS_HALT(QWISTYS_TAG_IMPLEMENTED)                                      \
  } while (0)

#define QWISTYS_UNREACH_VIM()                                                  \
  do {                                                                         \
    char cmd[500];                                                             \
    char line[5];                                                              \
    snprintf(line, 5, "%d", __LINE__);                                         \
    strcpy(cmd, "vim +");                                                      \
    strcat(cmd, line);                                                         \
    strcat(cmd, " ");                                                          \
    strcat(cmd, __FILE__);                                                     \
    system(cmd);                                                               \
    exit(1);                                                                   \
  } while (0)

#define QWISTYS_SET_EQ(a, b)                                                   \
  do {                                                                         \
    a = b;                                                                     \
    if (a == NULL)                                                             \
      QWISTYS_HALT(QWISTYS_TAG_SE)                                             \
  } while (0)

#define QWISTYS_ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))
#define QWISTYS_ARRAY_ACCESS(array, index)                                     \
  (QWISTYS_ASSERT((index) >= 0 && (index) < QWISTYS_ARRAY_LEN(array)),         \
   array[index])

// Bitwise operations
#define QWISTYS_BIT_SET(where, which) ((where) |= (1UL << (which)))
#define QWISTYS_BIT_CLEAR(where, which) ((where) &= ~(1UL << (which)))
#define QWISTYS_BIT_TOGGLE(where, which) ((where) ^= (1UL << (which)))
#define QWISTYS_BIT_CHECK(where, which) (!!((where) & (1UL << (which))))

// Custom assertion handler
typedef void (*assert_handler_t)(const char *expr, const char *file,
                                 const char *function, int line);
static assert_handler_t custom_assert_handler = NULL;

static inline void set_assert_handler(assert_handler_t handler) {
  custom_assert_handler = handler;
}

// Default assertion handler
static void default_assert_handler(const char *expr, const char *file,
                                   const char *function, int line) {
  fprintf(stderr, "Assertion failed: %s, function %s, file %s, line %d.\n",
          expr, function, file, line);
  abort();
}

// Define the QWISTYS_ASSERT macro only if not in a release build
#ifndef NDEBUG
#ifdef _MSC_VER
// MSVC compiler
#define QWISTYS_ASSERT(value)                                                  \
  do {                                                                         \
    if (!(value))                                                              \
      (custom_assert_handler                                                   \
           ? custom_assert_handler(#value, __FILE__, __func__, __LINE__)       \
           : __debugbreak());                                                  \
  } while (0)
#elif defined(__GNUC__) || defined(__clang__)
// GCC or Clang on Linux
#define QWISTYS_ASSERT(value)                                                  \
  do {                                                                         \
    if (!(value))                                                              \
      (custom_assert_handler                                                   \
           ? custom_assert_handler(#value, __FILE__, __func__, __LINE__)       \
           : __builtin_trap());                                                \
  } while (0)
#else
// Unknown compiler/platform
#define QWISTYS_ASSERT(value)                                                  \
  do {                                                                         \
    if (!(value))                                                              \
      (custom_assert_handler                                                   \
           ? custom_assert_handler(#value, __FILE__, __func__, __LINE__)       \
           : abort());                                                         \
  } while (0)
#endif
#else
// In release builds, define QWISTYS_ASSERT as a no-op
#define QWISTYS_ASSERT(value) ((void)0)
#endif

#ifndef NDEBUG
#define QWISTYS_DEBUG_MSG(msg, ...)                                            \
  QWISTYS_MSG(QWISTYS_LOG_LEVEL_DEBUG, QWISTYS_TAG_DEBUG, msg, ##__VA_ARGS__)
#else
#define QWISTYS_DEBUG_MSG(msg, ...) ((void)0)
#endif

// Telemetry macros
typedef void (*telemetry_start_handler_t)(const char *function);
typedef void (*telemetry_end_handler_t)(const char *function, double duration);

static telemetry_start_handler_t telemetry_start_handler = NULL;
static telemetry_end_handler_t telemetry_end_handler = NULL;

static inline void set_telemetry_handlers(telemetry_start_handler_t start_handler,
                            telemetry_end_handler_t end_handler) {
  telemetry_start_handler = start_handler;
  telemetry_end_handler = end_handler;
}

typedef struct {
  const char *function;
  clock_t start_time;
} telemetry_data_t;

#ifdef ENABLE_QWISTYS_TELEMETRY
#define QWISTYS_TELEMETRY_START()                                              \
  telemetry_data_t telemetry_data = {__func__, clock()};                       \
  if (telemetry_start_handler)                                                 \
    telemetry_start_handler(__func__);

#define QWISTYS_TELEMETRY_END()                                                \
  if (telemetry_end_handler) {                                                 \
    clock_t end_time = clock();                                                \
    double duration =                                                          \
        (double)(end_time - telemetry_data.start_time) / CLOCKS_PER_SEC;       \
    telemetry_end_handler(__func__, duration);                                 \
  }
#else
#define QWISTYS_TELEMETRY_START() ((void)0)
#define QWISTYS_TELEMETRY_END() ((void)0)
#endif

// Bounds check macro
#define QWISTYS_BOUNDS_CHECK(index, size)                                      \
  if ((index) >= (size)) {                                                     \
    QWISTYS_HALT("Index out of bounds");                                       \
    QWISTYS_HALT(QWISTYS_TAG_AB);                                              \
  }

#ifdef __cplusplus
}
#endif

#endif /* __QWISTYS_MACROS_H */
