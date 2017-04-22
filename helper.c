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


/**
 * test iam users retrieve
 */
#ifdef TEST
int main(int argc, char** argv)
{
        char buffer[1024];
        struct passwd p;
        strcpy(buffer, argv[1]);

        if(get_posix_iam_user(buffer, 1024, &p))
                printf("%s (%d)\n", p.pw_name, p.pw_uid);
}
#endif
