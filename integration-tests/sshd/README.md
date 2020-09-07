## Purpose
OpenSSH Server integration tests installing and using libnss_iam.so.2 with OpenSSH(d).

## Setup
Requires an AWS IAM user with ssh public key set.

Ensure the `[default]` profile is a non-expired shared credentials file entry:
```ini
[default]
assumed_role = False
aws_access_key_id = ...
aws_secret_access_key = ...
aws_session_token = ...
aws_security_token = ...
expiration = ...
```

## Testing
Build the target system docker image, run sshd + configs:
```bash
libnss-iam/integration-tests/sshd$ make docker-build
libnss-iam/integration-tests/sshd$ make docker-sshd
```

In another terminal connect using ssh client:
```bash
# ssh-add ~/.ssh/id_rsa
$ ssh -p 2222 -o IdentitesOnly=yes -i ~/.ssh/id_rsa <iam-user>@localhost
```

## Debugging
Run an interactive shell with the sshd configs:
```bash
libnss-iam/integration-tests/sshd$ make docker-shell
root@sshd-worker-ubuntu-16-04:/# dpkg -i /libnss-iam/libnss-iam-0.1.deb
root@sshd-worker-ubuntu-16-04:/# ldd /lib/libnss_iam.so.2

root@sshd-worker-ubuntu-16-04:/# getent passwd <iam-user>
_nss_iam_getpwnam_r <iam-user> sshd 1024 0
fvogt:*:65295:10:arn aws iam  <acct-id> user/<iam-user>:/home/<iam-user>:/bin/bash
```
