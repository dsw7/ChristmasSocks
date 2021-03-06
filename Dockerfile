FROM debian

RUN apt-get update && \
    apt-get install -y \
    cmake \
    git \
    make \
    python3 \
    python3-pip

RUN python3 -m pip install --user click pytest

ENV PWD=/root

WORKDIR $PWD

CMD git clone https://github.com/dsw7/ChristmasSocks.git && cd ChristmasSocks && make full-release
