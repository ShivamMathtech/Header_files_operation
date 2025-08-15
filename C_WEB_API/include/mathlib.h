#ifndef MATHLIB_H
#define MATHLIB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/* Simple math API - example of a clean C API (opaque state not needed here) */

/* Add two 64-bit integers. */
long math_add(long a, long b);

/* Subtract b from a. */
long math_sub(long a, long b);

/* Multiply two 64-bit integers. */
long math_mul(long a, long b);

/* Divide a by b. Returns 0 and sets *err on division-by-zero (err != 0). */
long math_div(long a, long b, int *err);

#ifdef __cplusplus
}
#endif

#endif /* MATHLIB_H */
