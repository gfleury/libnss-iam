[![Build Status](https://travis-ci.com/gfleury/libnss-iam.svg?branch=master)](https://travis-ci.com/gfleury/libnss-iam)
# libnss-iam
Lib NSS module to integrate AWS IAM users/groups to Linux NSS subsystem.

## Building
A docker ubuntu 16.04 environment is used to build the AWS c++ sdk:
```
# Create and launch the sdk build environment
libnss-iam$ make docker-build
libnss-iam$ make docker-shell

$USER@$aws-sdk-builder:~/libnss-iam$ make deps
```

Build libnss_iam.so.2:
```
# This can be done on an ubuntu 16.04 or 18.04 host
libnss-iam$ make
```

Build .deb package
```
# This can be done on an ubuntu 16.04 or 18.04 host
libnss-iam$ make deb
```

## Docker Simulator
A simulator is provided to test installing and using libnss_iam.so.2

AWS: You'll need an IAM user with ssh public key set
```
# The user level AWS shared creds file should have [default] set correctly in '~/.aws/credentials'
[default]
assumed_role = False
aws_access_key_id = ...
aws_secret_access_key = ...
aws_session_token = ...
aws_security_token = ...
expiration = ...
```

Start/Configure the simulator:
```
# Create and launch the simulator
libnss-iam/docker$ make docker-build
libnss-iam/docker$ make docker-shell

# install libnss_iam.so.2 (Makefile)
root@bastion-service:/libnss-iam# make install
or
# install libnss_iam.so.2 (.deb package)
root@bastion-service:/# dpkg -i /libnss-iam/libnss-iam-0.1.deb

# Start sshd
root@bastion-service:/libnss-iam# /usr/sbin/sshd -d
```

In another terminal:
```
# Connect to the simulator
$ ssh -p 2222 localhost
```

## Debugging
Running:
```
libnss-iam$ AWS_PROFILE=... ./iam <iam-user>
libnss-iam$ AWS_PROFILE=... ./iam <iam-user> <password> [<mfa-code>]
```

## Links
* https://github.com/aws/aws-sdk-cpp
* https://github.com/aws/aws-sdk-cpp/wiki/Building-the-SDK-from-source-on-EC2
* https://docs.aws.amazon.com/sdk-for-cpp/v1/developer-guide/welcome.html
* https://aws.amazon.com/blogs/developer/aws-sdk-for-c-simplified-configuration-and-initialization/

libc:
* https://www.gnu.org/software/libc/manual/html_node/Name-Service-Switch.html
* https://www.gnu.org/software/libc/manual/html_node/Extending-NSS.html#Extending-NSS
* https://www.gnu.org/software/libc/manual/html_node/NSS-Module-Function-Internals.html
* https://www.gnu.org/software/libc/manual/html_node/NSS-Modules-Interface.html
* https://www.gnu.org/software/libc/manual/html_node/NSS-Module-Names.html
* https://www.gnu.org/software/libc/manual/html_node/Actions-in-the-NSS-configuration.html
* https://www.gnu.org/software/libc/manual/html_node/Notes-on-NSS-Configuration-File.html
* https://www.gnu.org/software/libc/manual/html_node/Services-in-the-NSS-configuration.html
* https://www.gnu.org/software/libc/manual/html_node/Adding-another-Service-to-NSS.html
