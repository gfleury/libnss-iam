SHELL=/bin/bash

DISTRO=ubuntu-16-04

docker-build:
	docker build -t "sshd-worker-$(DISTRO)" --file ./Dockerfile.$(DISTRO) .

docker-shell:
	-docker run --rm -it                                                  \
	    --hostname "sshd-worker-$(DISTRO)"                                \
	    -p 2222:22                                                        \
	    --volume "$(HOME)/.aws:/root/.aws"                                \
	    --volume "$(CURDIR)/opt:/opt:ro"                                  \
	    --volume "$(CURDIR)/etc/ssh/sshd_config:/etc/ssh/sshd_config:ro"  \
	    --volume "$(CURDIR)/etc/nsswitch.conf:/etc/nsswitch.conf:ro"      \
	    --volume "$(CURDIR)/../..:/libnss-iam:ro"                         \
	    --entrypoint "bin/bash"                                           \
	    "sshd-worker-$(DISTRO)"

docker-sshd:
	-docker run --rm -it                                                  \
	    --hostname "sshd-worker-$(DISTRO)"                                \
	    -p 2222:22                                                        \
	    --volume "$(HOME)/.aws:/root/.aws"                                \
	    --volume "$(CURDIR)/opt:/opt:ro"                                  \
	    --volume "$(CURDIR)/etc/ssh/sshd_config:/etc/ssh/sshd_config:ro"  \
	    --volume "$(CURDIR)/etc/nsswitch.conf:/etc/nsswitch.conf:ro"      \
	    --volume "$(CURDIR)/../..:/libnss-iam:ro"                         \
	    --entrypoint "bin/bash"                                           \
	    "sshd-worker-$(DISTRO)"                                           \
	    -c "dpkg -i /libnss-iam/libnss-iam-0.1.deb && /usr/sbin/sshd -d"
