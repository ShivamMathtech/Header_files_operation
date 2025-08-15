#include "mathlib.h"

/* Very small library to show separation of API and server wrapper.
 * In real projects this file will include error handling, logging, more functions.
 */

long math_add(long a, long b) {
    return a + b;
}

long math_sub(long a, long b) {
    return a - b;
}

long math_mul(long a, long b) {
    return a * b;
}

long math_div(long a, long b, int *err) {
    if (b == 0) {
        if (err) *err = 1;
        return 0;
    }
    if (err) *err = 0;
    return a / b;
}
