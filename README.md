[![Build Status](https://travis-ci.com/gfleury/libnss-iam.svg?branch=master)](https://travis-ci.com/gfleury/libnss-iam)
# libnss-iam
Lib NSS module to integrate AWS IAM users/groups to Linux NSS subsystem.

## Building
A docker ubuntu 18.04 environment is used to build the AWS c++ sdk for ubuntu 18.04+.

For a ubuntu 16.04 compatible .deb use ubuntu 16.04 to build the AWS c++ sdk.

Create and launch the sdk build environment:
```bash
libnss-iam$ make docker-build
libnss-iam$ make docker-shell
```

Compile the AWS c++ sdk:
```bash 
$USER@$aws-sdk-builder:~/libnss-iam$ make deps
```

Build libnss_iam.so.2:
```
libnss-iam$ make
```

Build .deb package
```
libnss-iam$ make test
libnss-iam$ make deb
```

## Integration Tests
* [OpenSSHd](integration-tests/sshd)


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
