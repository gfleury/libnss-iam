FROM ubuntu:20.04

# Setup non-root user
ARG nonroot_user
ARG nonroot_uid

RUN groupadd -g ${nonroot_uid} ${nonroot_user}
RUN useradd -u ${nonroot_uid} -g ${nonroot_uid} -m ${nonroot_user}

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=UTC

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake git \
    zlib1g zlib1g-dev \
    libssl-dev \
    libcurl4-openssl-dev
