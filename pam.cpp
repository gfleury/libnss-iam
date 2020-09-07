#include <aws/core/Aws.h>
#include <aws/sts/STSClient.h>
#include <aws/sts/model/GetCallerIdentityResult.h>
#include <aws/sts/model/GetCallerIdentityRequest.h>
#include <iostream>

#include <stdio.h>
#include <curl/curl.h>

#include "pam.h"

int __pam_was_initialized = 0;

Aws::String get_account_id() {
   
    Aws::SDKOptions options;
    
    if (!__pam_was_initialized) {
        Aws::InitAPI(options);
        __pam_was_initialized = 1;
    }
    
    Aws::STS::STSClient sts;
    Aws::STS::Model::GetCallerIdentityRequest get_request;

    auto outcome = sts.GetCallerIdentity(get_request);
    
    if (!outcome.IsSuccess()) 
        std::cout << "Failed to get called account id:" << outcome.GetError().GetMessage() << std::endl;
    else
        std::cout << outcome.GetResult().GetAccount();

    return outcome.GetResult().GetAccount();
    
}


#define DATA "redirect_uri=https%%3A%%2F%%2Fus-west-2.console.aws.amazon.com&client_id=arn%%3Aaws%%3Aiam%%3A%%3A015428540659%%3Auser%%2Fhomepage&forceMobileApp=&forceMobileLayout=&isIAMUser=1&mfaLoginFailure=&mfaType=SW&Action=login&RemainingExpiryPeriod=&account=%s&username=%s&password=%s&mfacode=%s&next_mfacode="

int iam_authenticate(char *user, char *pass, char *token) {
   
    CURL *curl;
    CURLcode res;

    if (user == NULL || pass == NULL)
        return 500;

    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
    curl = curl_easy_init();
    
    if (curl) {
        struct curl_slist *chunk = NULL;
        char data[1024];

        /* Add a referer header  */
        chunk = curl_slist_append(chunk, "Referer: https://us-west-2.signin.aws.amazon.com/");

        /* set our custom set of headers */
        res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

        /* Using HTTPS with libcurl do check certificate validity */
        curl_easy_setopt(curl, CURLOPT_URL, "https://us-west-2.signin.aws.amazon.com/oauth");
    
        /* Now specify the POST data */
        snprintf(data, 1024, DATA, get_account_id().c_str(), user, pass, token == NULL ? "" : token);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        
        /* Check for errors */
        if (res != CURLE_OK) 
                fprintf(stderr, "curl_easy_perform() failed: %d\n", res);
                
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &res);
        
        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    
    curl_global_cleanup();
    return res;
}
