FROM ubuntu:20.04

RUN apt-get update && apt-get install -y \
    build-essential \
    clang \
    libstdc++-9-dev \
    toilet

WORKDIR /app

COPY . /app
ENV TERM xterm
RUN make

EXPOSE 6667

CMD ["./ircserv", "0.0.0.0", "8080"]


