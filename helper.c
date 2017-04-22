#include <aws/iam/model/GetUserRequest.h>
#include <aws/iam/model/GetUserResult.h>
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
 * iam to posix users retriever
 */
#define __LEN 64
enum nss_status get_posix_iam_user(char *buffer, int buflen, struct passwd *p) {

        Aws::IAM::Model::User iam_user = get_iam_user(buffer);
        if (!iam_user.GetUserId().empty()) {
                if (p) {
                        char home[] = "/home/iam-user";
                        char shell[] = "/bin/bash";
                        char pwd[] = "*";
                        int bytes = iam_user.GetUserName().size()+1 > __LEN ? __LEN : iam_user.GetUserName().size()+1, offset = 0;
                        p->pw_name = memcpy (buffer + offset, iam_user.GetUserName().c_str(), bytes);
                        offset += bytes;
                        p->pw_uid = hash(iam_user.GetUserId().c_str());
                        p->pw_gid = 0;
                        bytes = iam_user.GetArn().size()+1 > __LEN ? __LEN : iam_user.GetArn().size()+1;
                        p->pw_gecos = memcpy (buffer + offset, iam_user.GetArn().c_str(), bytes);
                        offset += bytes;
                        bytes = sizeof(home) > __LEN ? __LEN : sizeof(home);
                        p->pw_dir = memcpy (buffer + offset, home, bytes);
                        offset += bytes;
                        bytes = sizeof(shell) > __LEN ? __LEN : sizeof(shell);
                        p->pw_shell = memcpy (buffer + offset, shell, bytes);
                        offset += bytes;
                        bytes = sizeof(pwd) > __LEN ? __LEN : sizeof(pwd);
                        p->pw_passwd = memcpy (buffer + offset, "", buflen - offset);
                }
                return NSS_STATUS_SUCCESS;
        }
        return NSS_STATUS_NOTFOUND;
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
