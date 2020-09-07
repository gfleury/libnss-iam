#include <stdio.h>
#include <string.h>

#include "headers.h" 
#include "libnss_iam.h"

/*
 * NSS_STATUS_TRYAGAIN	EAGAIN	One of the functions used ran temporarily out of resources or a service is currently not available.
 *			ERANGE	The provided buffer is not large enough. The function should be called again with a larger buffer.
 * NSS_STATUS_UNAVAIL	ENOENT	A necessary input file cannot be found.
 * NSS_STATUS_NOTFOUND	ENOENT	The requested entry is not available.
 * NSS_STATUS_NOTFOUND	SUCCESS	There are no entries. Use this to avoid returning errors for inactive services which may be enabled at a later time. This is not the same as the service being temporarily unavailable.
 */

enum nss_status _nss_iam_setpwent(void) {
    printf("%s\n", __FUNCTION__);

    return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_iam_endpwent(void) {
    printf("%s\n", __FUNCTION__);

    return NSS_STATUS_NOTFOUND;
}

/*
 *            struct passwd {
 *              char   *pw_name;       username 
 *              char   *pw_passwd;      user password 
 *              uid_t   pw_uid;         user ID 
 *              gid_t   pw_gid;         group ID 
 *              char   *pw_gecos;       user information 
 *              char   *pw_dir;         home directory 
 *              char   *pw_shell;       shell program 
 *          };
 */
enum nss_status _nss_iam_getpwnam_r(const char *name, struct passwd *result, char *buffer, size_t buflen, int *errnop) {
    printf("%s %s %s %zd %d\n", __FUNCTION__, name, buffer, buflen, *errnop);

    strcpy(buffer, name);

    return get_posix_iam_user(buffer, buflen, result);
}

enum nss_status _nss_iam_getpwent_r(struct passwd *result, char *buffer, size_t buflen, int *errnop) {
    // Lista users, each call list one user sucessive
    printf("%s %s %zd %d\n", __FUNCTION__, buffer, buflen, *errnop);

    return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_iam_getpwbyuid_r(uid_t uid, struct passwd *result, char *buffer, size_t buflen, int *errnop) {
    printf("%s %d %s %zd %d\n", __FUNCTION__, uid, buffer, buflen, *errnop);

    return NSS_STATUS_SUCCESS;
}

enum nss_status _nss_iam_getpwbynam_r(const char *name, struct passwd *result, char *buffer, size_t buflen, int *errnop) {
    printf("%s %s %s %zd %d\n", __FUNCTION__, name, buffer, buflen, *errnop);

    strcpy(buffer, name);

    return get_posix_iam_user(buffer, buflen, result);
}

enum nss_status _nss_iam_getpwuid_r(uid_t uid, struct passwd* result_buf, char* buffer, size_t buflen, struct passwd** result) {
    printf("%s %d %s %zd \n", __FUNCTION__, uid, buffer, buflen);

    return get_posix_iam_user_by_uid(uid, buffer, buflen, result_buf);
}

/*
 *            struct spwd {
 *              char *sp_namp;     / Login name /
 *              char *sp_pwdp;     / Encrypted password /
 *              long  sp_lstchg;   / Date of last change
 *                                    (measured in days since
 *                                    1970-01-01 00:00:00 +0000 (UTC)) /
 *              long  sp_min;      / Min # of days between changes /
 *              long  sp_max;      / Max # of days between changes /
 *              long  sp_warn;     / # of days before password expires
 *                                    to warn user to change it /
 *              long  sp_inact;    / # of days after password expires
 *                                    until account is disabled /
 *              long  sp_expire;   / Date when account expires
 *                                    (measured in days since
 *                                    1970-01-01 00:00:00 +0000 (UTC)) /
 *              unsigned long sp_flag;  / Reserved /
 *          };
 */

enum nss_status _nss_iam_getspnam_r(const char *name, struct spwd *s, char *buffer, size_t buflen, int *errnop) {

    // return NSS_STATUS_TRYAGAIN;
    printf("%s %s %zd %d\n", __FUNCTION__, name, buflen, *errnop);

    return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_iam_getgrent_r(struct group* result_buf, char* buffer, size_t buflen, struct group** result) {
    printf("%s %zd \n", __FUNCTION__, buflen);

    return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_iam_getgrgid_r(gid_t gid, struct group* result_buf, char* buffer, size_t buflen, struct group** result) {
    printf("%s %zd\n", __FUNCTION__, buflen);

    return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_iam_nss_iam_getgrnam_r(const char* name, struct group* result_buf, char* buffer, size_t buflen, struct group** result) {
    printf("%s %s %zd\n", __FUNCTION__, name, buflen);

    return NSS_STATUS_NOTFOUND;
}


