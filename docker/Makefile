docker-build:
	docker build -t "sshd-worker" --file ./Dockerfile .

docker-shell:
	sudo chown -R root:root "$(CURDIR)/opt/sshd-service"
	-docker run --rm -it                                                  \
	    --hostname "bastion-service"                                      \
        -p 2222:22                                                        \
	    --volume "$(HOME)/.aws:/root/.aws"                                \
	    --volume "/dev/log:/dev/log"                                      \
	    --volume "$(CURDIR)/opt/sshd-service:/opt:ro"                     \
	    --volume "$(CURDIR)/etc/ssh/sshd_config:/etc/ssh/sshd_config:ro"  \
	    --volume "$(CURDIR)/etc/nsswitch.conf:/etc/nsswitch.conf:ro"      \
	    --volume "$(CURDIR)/..:/libnss-iam:ro"                            \
	    --entrypoint "/bin/bash"                                          \
	    "sshd-worker"
	sudo chown -R $(USER):$(USER) "$(CURDIR)/opt/sshd-service"
