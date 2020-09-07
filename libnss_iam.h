enum nss_status get_posix_iam_user(char *buffer, int buflen, struct passwd *p);
enum nss_status get_posix_iam_user_by_uid(uid_t uid, char *buffer, int buflen, struct passwd *p);

enum nss_status _nss_iam_setpwent(void);
enum nss_status _nss_iam_endpwent(void);
enum nss_status _nss_iam_getpwent_r(struct passwd *result, char *buffer, size_t buflen, int *errnop);
enum nss_status _nss_iam_getpwbyuid_r(uid_t uid, struct passwd *result, char *buffer, size_t buflen, int *errnop);
enum nss_status _nss_iam_getpwbynam_r(const char *name, struct passwd *result, char *buffer, size_t buflen, int *errnop);
