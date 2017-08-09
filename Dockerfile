FROM ubuntu:16.04

ENV HOME /root
ENV DEBIAN_FRONTEND noninteractive
ENV LOG_MARKED "================================"

COPY ./sources.list /etc/apt/

RUN apt-get update \
	&& echo "$LOG_MARKED apt-get clean..." \
	&& apt-get clean \
	\
# automake
	&& echo "$LOG_MARKED automake installing..." \
    && apt-get install -y automake \
    \
# build-essential
	&& echo "$LOG_MARKED build-essential installing..." \
    && apt-get install -y build-essential \
    \
# gcc
	&& echo "$LOG_MARKED gcc installing..." \
    && apt-get install -y automake \
    \
# libtool
	&& echo "$LOG_MARKED libtool installing..." \
    && apt-get install -y libtool \
    \
# gyp
	&& echo "$LOG_MARKED gyp installing..." \
	&& apt-get install -y gyp \
	\
# git
	&& echo "$LOG_MARKED git installing..." \
	&& apt-get install -y git \
	\
# libuv
	&& echo "$LOG_MARKED libuv installing..." \
	&& cd /usr/local \
	&& git clone https://github.com/libuv/libuv.git \
	&& cd ./libuv \
	&& sh autogen.sh \
    && ./configure \
    && make \
    && make check \
    && make install \
    && ./gyp_uv.py -f make \
    && make -C out \
    \
# clean
	&& echo "$LOG_MARKED clean..." \
	&& rm -rf /var/lib/apt/lists/*

CMD ["/bin/bash"]
