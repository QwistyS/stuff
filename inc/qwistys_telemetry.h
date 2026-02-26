#ifndef QWISTYS_TELEMETRY_H
#define QWISTYS_TELEMETRY_H

/**
 * Portable telemetry / profiling macros.
 *
 * Priority order (first matching define wins):
 *   1. TRACY_ENABLE  — uses Tracy RAII zones (ZoneScoped / ZoneScopedN).
 *                      Tracy's zones are scope-based, so TELEMETRY_END() is a
 *                      no-op: the zone closes automatically on any return path.
 *   2. ENABLE_QWISTYS_TELEMETRY — clock-based callback backend. Registers a
 *                      start/end handler pair via set_telemetry_handlers().
 *                      Note: only the final return path gets TELEMETRY_END()
 *                      coverage; early returns skip it (acceptable for now).
 *   3. (neither)     — both macros expand to ((void)0).
 *
 * To add a new profiler backend later, add a new #elif branch here and define
 * QWISTYS_TELEMETRY_START() / QWISTYS_TELEMETRY_END().
 */

#include <time.h>

/* ── C-compatible callback slots ─────────────────────────────────────────── */
#ifdef __cplusplus
extern "C" {
#endif

typedef void (*telemetry_start_handler_t)(const char* function);
typedef void (*telemetry_end_handler_t)(const char* function, double duration);

static telemetry_start_handler_t telemetry_start_handler = NULL;
static telemetry_end_handler_t   telemetry_end_handler   = NULL;

static inline void set_telemetry_handlers(telemetry_start_handler_t start_handler,
                                          telemetry_end_handler_t   end_handler) {
    telemetry_start_handler = start_handler;
    telemetry_end_handler   = end_handler;
}

typedef struct {
    const char* function;
    clock_t     start_time;
} telemetry_data_t;

#ifdef __cplusplus
}  /* extern "C" */
#endif

/* ── Telemetry macros ────────────────────────────────────────────────────── */

#ifdef TRACY_ENABLE
/* Tracy backend — RAII zones, header-only client assumed under tracy/Tracy.hpp */
#    include "tracy/Tracy.hpp"
#    define QWISTYS_TELEMETRY_START() ZoneScoped
#    define QWISTYS_TELEMETRY_END()   ((void)0)

#elif defined(ENABLE_QWISTYS_TELEMETRY)
/* Clock + callback backend */
#    define QWISTYS_TELEMETRY_START()                                      \
         telemetry_data_t telemetry_data = {__func__, clock()};            \
         if (telemetry_start_handler) telemetry_start_handler(__func__)

#    define QWISTYS_TELEMETRY_END()                                        \
         if (telemetry_end_handler) {                                      \
             clock_t _tel_end = clock();                                   \
             double  _tel_dur = (double)(_tel_end - telemetry_data.start_time) \
                                / CLOCKS_PER_SEC;                          \
             telemetry_end_handler(__func__, _tel_dur);                    \
         }

#else
/* No telemetry — strip everything */
#    define QWISTYS_TELEMETRY_START() ((void)0)
#    define QWISTYS_TELEMETRY_END()   ((void)0)
#endif

#endif  /* QWISTYS_TELEMETRY_H */
