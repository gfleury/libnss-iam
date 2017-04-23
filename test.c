//#include "headers.h" 
#include <pwd.h>

/**
 * test iam users retrieve
 */
#ifdef TEST
#include <string.h>

int main(int argc, char** argv) {
    char buffer[1024];
    struct passwd p;
    memset(buffer, '0', 1024);
    if (argc > 1)
        strcpy(buffer, argv[1]);

    if (get_posix_iam_user(buffer, 1024, &p)) {
        printf("%s (%d)\n", p.pw_name, p.pw_uid);
        if (get_posix_iam_user_by_uid(p.pw_uid, buffer, 1024, &p))
            printf("%s (%d)\n", p.pw_name, p.pw_uid);
    }
    
    if (argc > 2) {
        int ret = iam_authenticate(argv[1], argv[2], argc > 2 ? argv[3] : NULL);
        printf("Ret: %d\n", ret);
    }
}
#endif
