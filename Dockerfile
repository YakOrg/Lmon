FROM alpine:3.14.0
RUN apk update
RUN apk --no-cache add cmake git tar wget unzip libc-dev binutils gcc musl-dev

RUN wget -q https://curl.haxx.se/download/curl-7.66.0.tar.gz \
    && tar -xvzf curl-7.66.0.tar.gz > /dev/null \
    && rm -f curl-7.66.0.tar.gz \
    && cd ./curl-7.66.0/ \
    && ./configure CFLAGS='-O3' \
       --disable-shared \
       --disable-ares \
       --disable-cookies \
       --disable-crypto-auth \
       --disable-ipv6 \
       --disable-manual \
       --disable-proxy \
       --without-libidn \
       --without-librtmp \
       --without-brotli \
       --without-ssl \
       --without-nghttp2 \
       --without-libidn2 \
       --without-libpsl \
       --without-zlib \
       --disable-dict \
       --disable-file \
       --disable-ftp \
       --disable-gopher \
       --disable-imap \
       --disable-pop3 \
       --disable-rtsp \
       --disable-smtp \
       --disable-telnet \
       --disable-tftp \
       --disable-ldap \
       --disable-ldaps \
    && make install \
    && cd .. \
    && rm -rf ./curl-7.66.0

RUN wget -q https://ftp.gnu.org/gnu/libmicrohttpd/libmicrohttpd-0.9.70.tar.gz \
    && tar -xvzf libmicrohttpd-0.9.70.tar.gz > /dev/null \
    && rm -f libmicrohttpd-0.9.70.tar.gz \
    && cd ./libmicrohttpd-0.9.70/ \
    && ./configure --disable-https --disable-shared \
    && make install \
    && cd .. \
    && rm -rf ./libmicrohttpd-0.9.70

RUN wget -q http://www.digip.org/jansson/releases/jansson-2.13.1.tar.gz \
    && tar -xvzf jansson-2.13.1.tar.gz > /dev/null \
    && rm -f jansson-2.13.1.tar.gz \
    && cd ./jansson-2.13.1/ \
    && ./configure --disable-shared \
    && make install \
    && cd .. \
    && rm -rf ./jansson-2.13.1

ADD Libbrd.zip LibSysMetrics.zip ./
