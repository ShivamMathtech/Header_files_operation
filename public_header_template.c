/* project_module.h
 * Public API for Project Module
 */

#ifndef PROJECT_MODULE_H
#define PROJECT_MODULE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

/* Version */
#define PROJECT_MODULE_VERSION_MAJOR 1
#define PROJECT_MODULE_VERSION_MINOR 0

/* Opaque handle */
typedef struct project_module project_module_t;

/* Error codes (negative) */
enum {
    PM_OK = 0,
    PM_ERR_INVALID = -1,
    PM_ERR_NOMEM = -12,
    PM_ERR_IO = -5
};

/**
 * pm_create
 * Create module context.
 * Returns: non-NULL on success, NULL on failure (ENOMEM).
 * Thread-safety: caller must not call concurrently from multiple threads.
 */
project_module_t *pm_create(void);

/**
 * pm_destroy
 * Destroy context and free resources.
 * After this call, ctx is no longer valid.
 */
void pm_destroy(project_module_t *ctx);

/**
 * pm_process
 * Process data in-place. Returns 0 on success or negative error code.
 * Thread-safety: reentrant, but ctx must not be used by multiple threads concurrently.
 */
int pm_process(project_module_t *ctx, uint8_t *buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* PROJECT_MODULE_H */
