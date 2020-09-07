## 0.1
libnss-iam is built and packaged for ubuntu 16.04+:
* libnss-iam-bionic-0.1.deb
* libnss-iam-xenial-0.1.deb

Newer ubuntu LTS releases require separate builds:
```
ubuntu 18.04+ => curl 4, openssl 1.1
ubuntu 16.04  => curl 3, openssl 1.0
```

ubuntu 18.04+ compatible deb:
```bash
make clean; make docker-build docker-shell
$USER@aws-sdk-builder-ubuntu-18-04:~/libnss-iam$ make deps libnss_iam test deb

mv libnss-iam-0.1.deb libnss-iam-bionic-0.1.deb
shasum -a 512 libnss-iam-bionic-0.1.deb
```

ubuntu 16.04 compatible deb:
```bash
make clean; make DISTRO=ubuntu-16-04 docker-build docker-shell
$USER@aws-sdk-builder-ubuntu-16-04:~/libnss-iam$ make deps libnss_iam test deb

mv libnss-iam-0.1.deb libnss-iam-xenial-0.1.deb
shasum -a 512 libnss-iam-xenial-0.1.deb
```
