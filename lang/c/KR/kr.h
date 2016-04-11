#ifndef _KR_H_
#define _KR_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define COLORFUL
#ifdef COLORFUL
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define RESET   "\033[0;m"
#else
#define RED
#define GREEN
#define RESET
#endif
static inline const char *expect(bool v) {
    return v ? (GREEN "pass"RESET) : (RED "FAIL"RESET);
}
#endif
