FROM ubuntu:19.04

LABEL name="Rakhimgaliyev Temirlan"
LABEL email="rakhimgaliev56@gmail.com"

ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

USER root
ENV DEBIAN_FRONTEND 'noninteractive'

RUN apt-get update -y
RUN apt-get install -y --no-install-recommends apt-utils

RUN apt-get install -y gcc-c++
RUN apt-get install -y make

ADD . /app
WORKDIR /app

RUN make

CMD ./main.out 80 

EXPOSE 80
