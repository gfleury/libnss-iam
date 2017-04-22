#include <nss.h>
#include <pwd.h>
#include <grp.h>
#include <shadow.h>

unsigned long hash(const char *str);
enum nss_status get_posix_iam_user(char *buffer, int buflen, struct passwd *p);

