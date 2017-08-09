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
	&& echo "$LOG_MARKED automake installing..." \
	&& apt-get install -y automake \
    \
# libtool
	&& echo "$LOG_MARKED libtool installing..." \
	&& apt-get install -y libtool \
# clean
	&& echo "$LOG_MARKED clean..." \
	&& rm -rf /var/lib/apt/lists/*

CMD ["/bin/bash"]
