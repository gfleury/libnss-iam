/*
   Copyright 2010-2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.

   This file is licensed under the Apache License, Version 2.0 (the "License").
   You may not use this file except in compliance with the License. A copy of
   the License is located at

    http://aws.amazon.com/apache2.0/

   This file is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied. See the License for the
   specific language governing permissions and limitations under the License.
*/
#include <aws/core/Aws.h>
#include <aws/iam/IAMClient.h>
#include <aws/iam/model/GetUserRequest.h>
#include <aws/iam/model/GetUserResult.h>

static const char* DATE_FORMAT = "%Y-%m-%d";

/**
 * iam user retriever
 */
Aws::IAM::Model::User get_iam_user(char *username) {
    Aws::SDKOptions options;
    Aws::InitAPI(options);

    {
        Aws::IAM::IAMClient iam;
	Aws::IAM::Model::GetUserRequest get_request;
	get_request.SetUserName(username);

            auto outcome = iam.GetUser(get_request);
            if (!outcome.IsSuccess()) {
                std::cout << "Failed to list iam users:" <<
                    outcome.GetError().GetMessage() << std::endl;
            }
    	Aws::ShutdownAPI(options);
	return outcome.GetResult().GetUser();

    }
}

