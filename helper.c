#include <stdio.h>

#include "headers.h"
/*
 * dan bernstein at comp.lang.c
 * http://www.cse.yorku.ca/~oz/hash.html
 */
unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash%65535;
}

