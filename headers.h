#include <nss.h>
#include <pwd.h>
#include <grp.h>
#include <shadow.h>
#include <aws/iam/model/GetUserRequest.h>
#include <aws/iam/model/GetUserResult.h>

Aws::IAM::Model::User get_iam_user(char *username);
unsigned long hash(const char *str);
enum nss_status get_posix_iam_user(char *buffer, int buflen, struct passwd *p);

