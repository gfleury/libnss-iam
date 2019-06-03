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
