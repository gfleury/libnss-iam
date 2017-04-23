#include <aws/core/Aws.h>
#include <aws/sts/STSClient.h>
#include <aws/sts/model/GetCallerIdentityResult.h>
#include <aws/sts/model/GetCallerIdentityRequest.h>
#include <iostream>

int __was_initialized = 0;

Aws::String get_account_id() {
    Aws::SDKOptions options;
    if (!__was_initialized) {
        Aws::InitAPI(options);
        __was_initialized = 1;
    }
    {
        Aws::STS::STSClient sts;
        Aws::STS::Model::GetCallerIdentityRequest get_request;

        auto outcome = sts.GetCallerIdentity(get_request);
        if (!outcome.IsSuccess()) {
            std::cout << "Failed to get called account id:" <<
                    outcome.GetError().GetMessage() << std::endl;
        } else
            std::cout << outcome.GetResult().GetAccount();

        return outcome.GetResult().GetAccount();
    }



}

#include <stdio.h>
#include <curl/curl.h>
#define DATA "redirect_uri=https%%3A%%2F%%2Fus-west-2.console.aws.amazon.com&client_id=arn%%3Aaws%%3Aiam%%3A%%3A015428540659%%3Auser%%2Fhomepage&forceMobileApp=&forceMobileLayout=&isIAMUser=1&mfaLoginFailure=&mfaType=SW&Action=login&RemainingExpiryPeriod=&account=%s&username=%s&password=%s&mfacode=%s&next_mfacode="

int authenticate(char *user, char *pass, char *token) {
    CURL *curl;
    CURLcode res;
    if (user == NULL || pass == NULL)
        return 500;
    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *chunk = NULL;
        char data[1024];
        /* Modify a header curl otherwise adds differently */
        chunk = curl_slist_append(chunk, "Referer: https://us-west-2.signin.aws.amazon.com/");

        /* set our custom set of headers */
        res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

        /* First set the URL that is about to receive our POST. This URL can
           just as well be a https:// URL if that is what should receive the
           data. */
        curl_easy_setopt(curl, CURLOPT_URL, "https://us-west-2.signin.aws.amazon.com/oauth");
        /* Now specify the POST data */
        snprintf(data, 1024, DATA, get_account_id().c_str(), argv[1], argv[2], argv[3]);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return 0;
}
