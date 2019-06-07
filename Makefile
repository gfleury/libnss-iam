SHELL=/bin/bash

user_name=$(shell whoami)
user_id=$(shell id -u)

INCLUDES=\
-Iaws-sdk-cpp/aws-cpp-sdk-core/include                      \
-Iaws-sdk-cpp/aws-cpp-sdk-iam/include                       \
-Iaws-sdk-cpp/aws-cpp-sdk-sts/include                       \
-Iaws-sdk-cpp/build/.deps/install/include

LIBS=\
aws-sdk-cpp/build/aws-cpp-sdk-iam/libaws-cpp-sdk-iam.a      \
aws-sdk-cpp/build/aws-cpp-sdk-sts/libaws-cpp-sdk-sts.a      \
aws-sdk-cpp/build/aws-cpp-sdk-core/libaws-cpp-sdk-core.a    \
aws-sdk-cpp/build/.deps/install/lib/libaws-c-event-stream.a \
aws-sdk-cpp/build/.deps/install/lib/libaws-checksums.a      \
aws-sdk-cpp/build/.deps/install/lib/libaws-c-common.a       \
-lssl                                                       \
-lcrypto                                                    \
-lcurl                                                      \
-lpthread

CC=gcc
CPP=g++
DEBUG=-g
INSTALL=/usr/bin/install
INSTALL_PROGRAM=${INSTALL}
INSTALL_DATA=${INSTALL} -m 644

prefix=""

libnss_iam: iam libnss_iam.c
	${CC} ${INCLUDES} ${DEBUG} -fPIC -Wall -shared -o libnss_iam.so.2 -Wl,-soname,libnss_iam.so.2 libnss_iam.c iam.o helper.o ${LIBS} -lstdc++

iam: helper.c iam.cpp
	${CPP} ${INCLUDES} ${DEBUG} -c -O3 helper.c iam.cpp -std=c++11 -fno-exceptions helper.c -fpermissive -fPIC

pam: pam.cpp
	${CPP} ${INCLUDES} ${DEBUG} -c -O3 pam.cpp -std=c++11 -fno-exceptions helper.c -fpermissive -fPIC

test: iam pam
	${CC} -O3 -o iam test.c -fno-exceptions -fPIC -DTEST pam.o iam.o helper.o ${LIBS} -lstdc++

install:	
	# /lib/libnss_compat.so.2 -> libnss_compat-2.3.6.so
	${INSTALL_DATA} libnss_iam.so.2 ${prefix}/lib/libnss_iam-2.3.6.so
	cd ${prefix}/lib && ln -fs libnss_iam-2.3.6.so libnss_iam.so.2
	ldconfig -n

clean:
	rm -f libnss_iam.so.2 libnss_iam_test iam

docker-build:
	docker build -t "sdk-builder"             \
	    --build-arg nonroot_user=$(user_name) \
	    --build-arg nonroot_uid=$(user_id)    \
	    --file ./Dockerfile .

docker-shell:
	@docker run -it --rm                                  \
	    --hostname "aws-sdk-builder"                      \
	    --volume $(HOME)/.aws:/home/$(user_name)/.aws     \
	    --volume $(HOME)/.ssh:/home/$(user_name)/.ssh     \
	    --volume $(CURDIR):/home/$(user_name)/libnss-iam  \
	    --user $(user_name)                               \
	    --workdir=/home/$(user_name)/libnss-iam           \
	    --env PS1='\u:\w\$$ '                             \
	    --entrypoint /bin/bash                            \
	    "sdk-builder"                                     \

deps:
	if [[ ! -d aws-sdk-cpp ]]; then git clone https://github.com/aws/aws-sdk-cpp.git; fi
	mkdir -p aws-sdk-cpp/build
	(cd aws-sdk-cpp/build; cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_ONLY="monitoring;sts;iam" -DBUILD_SHARED_LIBS=false)
	(cd aws-sdk-cpp/build; make -j 1)

DPKG_VERSION=0.1
DPKG_ROOT=libnss-iam-$(DPKG_VERSION)
deb:
	mkdir -p "$(DPKG_ROOT)/DEBIAN"
	
	echo -e \
Package: libnss-iam                                       \\n\
Version: $(DPKG_VERSION)                                  \\n\
Architecture: amd64                                       \\n\
Maintainer: George Fleury \<gfleury@gmail.com\>           \\n\
Homepage: https://github.com/gfleury/libnss-iam           \\n\
Description: Lib NSS module to integrate IAM users/groups \\n\
> "$(DPKG_ROOT)/DEBIAN/control"

	echo -e \
\#!/bin/sh   \\n\
set -e       \\n\
ldconfig -n  \\n\
exit 0       \\n\
> "$(DPKG_ROOT)/DEBIAN/postinst" && chmod 755 "$(DPKG_ROOT)/DEBIAN/postinst"
	
	mkdir -p "$(DPKG_ROOT)/lib/"
	cp "libnss_iam.so.2" "$(DPKG_ROOT)/lib/libnss_iam-$(DPKG_VERSION).so"
	chmod 644 "$(DPKG_ROOT)/lib/libnss_iam-$(DPKG_VERSION).so"
	cd "$(DPKG_ROOT)/lib" && ln -fs "libnss_iam-$(DPKG_VERSION).so" "libnss_iam.so.2"

	dpkg-deb --build "$(DPKG_ROOT)"
