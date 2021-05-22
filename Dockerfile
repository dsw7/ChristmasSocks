FROM debian

RUN apt-get update && \
    apt-get install -y \
    cmake \
    git \
    make \
    python3 \
    python3-pip

ENV PWD=/root

WORKDIR $PWD

CMD git clone https://github.com/dsw7/ChristmasSocks.git \
 && python3 -m pip install click \
 && chmod +x ./ChristmasSocks/ChristmasSocks/configure.py \
 && ./ChristmasSocks/ChristmasSocks/configure.py compile \
 && ./ChristmasSocks/ChristmasSocks/configure.py test
