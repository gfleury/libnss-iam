FROM ubuntu:18.04

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=UTC

RUN apt-get update \
 && apt-get install -y \
      openssh-server \
      sudo \
      awscli \
      curl \
      git \
      jq \
      vim

RUN apt-get update \
 && apt-get install -y build-essential zlib1g libssl1.0-dev libcurl3 libcurl-openssl1.0-dev

RUN mkdir /var/run/sshd

EXPOSE 22

CMD ["/opt/sshd-entrypoint.sh"]
