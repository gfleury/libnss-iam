/*
 * dan bernstein at comp.lang.c
 * http://www.cse.yorku.ca/~oz/hash.html
 */
unsigned long hash(unsigned char *str) {
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}
