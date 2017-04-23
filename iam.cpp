/*
 * CPP IAM Code linked to aws-sdk-cpp and exporting C symbols
 */
#include <aws/core/Aws.h>
#include <aws/iam/IAMClient.h>
#include <aws/iam/model/GetUserRequest.h>
#include <aws/iam/model/GetUserResult.h>
#include <aws/iam/model/ListUsersRequest.h>
#include <aws/iam/model/ListUsersResult.h>

#include <nss.h>
#include <pwd.h>
#include <grp.h>
#include <shadow.h>

Aws::IAM::Model::User get_iam_user(char *username);
Aws::IAM::Model::User get_iam_uid(uid_t uid);

static const char* DATE_FORMAT = "%Y-%m-%d";

unsigned long hash(const char *str);

int __was_initialized = 0;

/**
 * iam to posix users retriever
 */
#define __LEN 64

extern "C" enum nss_status get_posix_iam_user(char *buffer, int buflen, struct passwd *p) {

    Aws::IAM::Model::User iam_user = get_iam_user(buffer);
    if (!iam_user.GetUserId().empty()) {
        if (p) {
            Aws::String home = "/home/" + iam_user.GetUserName();
            char shell[] = "/bin/bash";
            char pwd[] = "*";
            int bytes = iam_user.GetUserName().size() + 1 > __LEN ? __LEN : iam_user.GetUserName().size() + 1, offset = 0;
            p->pw_name = memcpy(buffer + offset, iam_user.GetUserName().c_str(), bytes);
            offset += bytes;
            p->pw_uid = hash(iam_user.GetUserId().c_str());
            p->pw_gid = 10; /* wheel harded */
            bytes = iam_user.GetArn().size() + 1 > __LEN ? __LEN : iam_user.GetArn().size() + 1;
            p->pw_gecos = memcpy(buffer + offset, iam_user.GetArn().c_str(), bytes);
            offset += bytes;
            bytes = home.size() + 1 > __LEN ? __LEN : home.size() + 1;
            p->pw_dir = memcpy(buffer + offset, home.c_str(), bytes);
            offset += bytes;
            bytes = sizeof (shell) > __LEN ? __LEN : sizeof (shell);
            p->pw_shell = memcpy(buffer + offset, shell, bytes);
            offset += bytes;
            bytes = sizeof (pwd) > __LEN ? __LEN : sizeof (pwd);
            p->pw_passwd = memcpy(buffer + offset, pwd, buflen - offset);
        }
        return NSS_STATUS_SUCCESS;
    }
    return NSS_STATUS_NOTFOUND;
}

/**
 * iam uid user retriever
 */
extern "C" enum nss_status get_posix_iam_user_by_uid(uid_t uid, char *buffer, int buflen, struct passwd *p) {

    Aws::IAM::Model::User iam_user = get_iam_uid(uid);
    if (!iam_user.GetUserId().empty()) {
        if (p) {
            Aws::String home = "/home/" + iam_user.GetUserName();
            char shell[] = "/bin/bash";
            char pwd[] = "*";
            int bytes = iam_user.GetUserName().size() + 1 > __LEN ? __LEN : iam_user.GetUserName().size() + 1, offset = 0;
            p->pw_name = memcpy(buffer + offset, iam_user.GetUserName().c_str(), bytes);
            offset += bytes;
            p->pw_uid = hash(iam_user.GetUserId().c_str());
            p->pw_gid = 10; /* wheel harded */
            bytes = iam_user.GetArn().size() + 1 > __LEN ? __LEN : iam_user.GetArn().size() + 1;
            p->pw_gecos = memcpy(buffer + offset, iam_user.GetArn().c_str(), bytes);
            offset += bytes;
            bytes = home.size() + 1 > __LEN ? __LEN : home.size() + 1;
            p->pw_dir = memcpy(buffer + offset, home.c_str(), bytes);
            offset += bytes;
            bytes = sizeof (shell) > __LEN ? __LEN : sizeof (shell);
            p->pw_shell = memcpy(buffer + offset, shell, bytes);
            offset += bytes;
            bytes = sizeof (pwd) > __LEN ? __LEN : sizeof (pwd);
            p->pw_passwd = memcpy(buffer + offset, pwd, buflen - offset);
        }
        return NSS_STATUS_SUCCESS;
    }
    return NSS_STATUS_NOTFOUND;
}

/**
 * iam user retriever
 */
Aws::IAM::Model::User get_iam_uid(uid_t uid) {
    Aws::SDKOptions options;
    Aws::IAM::Model::User user;
    if (!__was_initialized) {
        Aws::InitAPI(options);
        __was_initialized = 1;
    }
    {
        Aws::IAM::IAMClient iam;
        Aws::IAM::Model::ListUsersRequest request;

        bool done = false;
        while (!done) {
            auto outcome = iam.ListUsers(request);
            if (!outcome.IsSuccess()) {
                std::cout << "Failed to list iam users:" <<
                        outcome.GetError().GetMessage() << std::endl;
                break;
            }

            const auto &users = outcome.GetResult().GetUsers();
            for (const auto &user : users) {
                if (hash(user.GetUserId().c_str()) == uid) {
                    //    			Aws::ShutdownAPI(options);
                    return user;
                }
            }

            if (outcome.GetResult().GetIsTruncated()) {
                request.SetMarker(outcome.GetResult().GetMarker());
            } else {
                done = true;
            }
        }
    }
    //    Aws::ShutdownAPI(options);
    return user;
}

/**
 * iam user retriever
 */
Aws::IAM::Model::User get_iam_user(char *username) {
    Aws::SDKOptions options;
    if (!__was_initialized) {
        Aws::InitAPI(options);
        __was_initialized = 1;
    }
    {
        Aws::IAM::IAMClient iam;
        Aws::IAM::Model::GetUserRequest get_request;
        get_request.SetUserName(username);

        auto outcome = iam.GetUser(get_request);
        if (!outcome.IsSuccess()) {
            std::cout << "Failed to list iam users:" <<
                    outcome.GetError().GetMessage() << std::endl;
        }
        //    	Aws::ShutdownAPI(options);
        return outcome.GetResult().GetUser();

    }
}

