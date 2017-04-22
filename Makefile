# Makefile for libnss-iam

#### Start of system configuration section. ####
INCLUDES = -Ithird-part/aws-sdk-cpp/aws-cpp-sdk-core/include -Ithird-part/aws-sdk-cpp/aws-cpp-sdk-iam/include
CC = gcc
CPP = g++
INSTALL = /usr/bin/install
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644

prefix = ""
exec_prefix = ${prefix}

# Where the installed binary goes.
bindir = ${exec_prefix}/bin
binprefix =

sysconfdir = /etc

# mandir = /usr/local/src/less-394/debian/less/usr/share/man
manext = 1
manprefix =

#### End of system configuration section. ####

all:	libnss_iam 

libnss_iam:	libnss_iam.c iam.cpp
	${CC} ${INCLUDES} -fPIC -Wall -shared -o libnss_iam.so.2 -Wl,-soname,libnss_iam.so.2 libnss_iam.c 

test:	iam.cpp
	${CPP} -O3 -o iam iam.cpp third-part/aws-sdk-cpp/aws-cpp-sdk-core/libaws-cpp-sdk-core.a third-part/aws-sdk-cpp/aws-cpp-sdk-iam/libaws-cpp-sdk-iam.a -Ithird-part/aws-sdk-cpp/aws-cpp-sdk-core/include -Ithird-part/aws-sdk-cpp/aws-cpp-sdk-iam/include -std=c++11 -lcurl -lcrypto -fno-exceptions -lssl -Lthird-part/aws-sdk-cpp/aws-cpp-sdk-core/ -Lthird-part/aws-sdk-cpp/aws-cpp-sdk-iam/ -laws-cpp-sdk-core helper.c -fpermissive -DTEST

install:	
	# remeber  /lib/libnss_compat.so.2 -> libnss_compat-2.3.6.so
	${INSTALL_DATA} libnss_iam.so.2 ${prefix}/lib64/libnss_iam-2.3.6.so
	cd ${prefix}/lib64 && ln -fs libnss_iam-2.3.6.so libnss_iam.so.2
	ldconfig -n

clean:
	rm -f libnss_iam.so.2 libnss_iam_test
	rm -rf debian/libnss-iam
	rm -f build-stamp
	rm -rf BUILD BUILDROOT RPMS SRPMS SOURCES SPECS

rpm: libnss_iam
	rm -rf BUILD BUILDROOT RPMS SRPMS SOURCES SPECS
	rpmbuild -ba rpm/libnss-iam.spec --define "_topdir $$(pwd)"
