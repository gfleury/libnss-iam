INCLUDES = -Ithird-part/aws-sdk-cpp/aws-cpp-sdk-core/include -Ithird-part/aws-sdk-cpp/aws-cpp-sdk-iam/include -Ithird-part/aws-sdk-cpp/aws-cpp-sdk-sts/include
LIBS = third-part/aws-sdk-cpp/aws-cpp-sdk-sts/libaws-cpp-sdk-sts.a third-part/aws-sdk-cpp/aws-cpp-sdk-core/libaws-cpp-sdk-core.a third-part/aws-sdk-cpp/aws-cpp-sdk-iam/libaws-cpp-sdk-iam.a -Lthird-part/aws-sdk-cpp/aws-cpp-sdk-core/ -Lthird-part/aws-sdk-cpp/aws-cpp-sdk-iam/ -laws-cpp-sdk-core -lssl -lcrypto -lcurl
CC = gcc
CPP = g++
DEBUG = -g
INSTALL = /usr/bin/install
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644

prefix = ""

all:	libnss_iam 

libnss_iam: iam libnss_iam.c
	${CC} ${INCLUDES} ${DEBUG} -fPIC -Wall -shared -o libnss_iam.so.2 -Wl,-soname,libnss_iam.so.2 libnss_iam.c iam.o helper.o ${LIBS}

iam: helper.c iam.cpp
	${CPP} ${INCLUDES} ${DEBUG} -c -O3 helper.c iam.cpp -std=c++11 -fno-exceptions helper.c -fpermissive -fPIC

pam: pam.cpp
	${CPP} ${INCLUDES} ${DEBUG} -c -O3 pam.cpp -std=c++11 -fno-exceptions helper.c -fpermissive -fPIC

test: iam pam
	${CC} -O3 -o iam test.c -fno-exceptions -fPIC -DTEST pam.o iam.o helper.o ${LIBS} -lstdc++

install:	
	# remeber  /lib/libnss_compat.so.2 -> libnss_compat-2.3.6.so
	${INSTALL_DATA} libnss_iam.so.2 ${prefix}/lib64/libnss_iam-2.3.6.so
	cd ${prefix}/lib64 && ln -fs libnss_iam-2.3.6.so libnss_iam.so.2
	ldconfig -n

clean:
	rm -f libnss_iam.so.2 libnss_iam_test iam
	rm -rf debian/libnss-iam
	rm -f build-stamp
	rm -rf BUILD BUILDROOT RPMS SRPMS SOURCES SPECS

rpm: libnss_iam
	rm -rf BUILD BUILDROOT RPMS SRPMS SOURCES SPECS
	rpmbuild -ba rpm/libnss-iam.spec --define "_topdir $$(pwd)"
