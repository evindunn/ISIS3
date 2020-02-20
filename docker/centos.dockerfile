# vim: set ft=dockerfile:

FROM usgsastro/centos:latest

SHELL ["/bin/bash", "-lc"]

RUN yum install cmake gcc git ninja -y && yum clean all

RUN curl -sLo /tmp/miniconda.sh https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh &&    \
    chmod +x /tmp/miniconda.sh &&                                                                           \
    /tmp/miniconda.sh -b -p /opt/conda &&                                                                   \
    rm /tmp/miniconda.sh &&                                                                                 \
    printf '#!/bin/bash\nexport PATH="/opt/conda/condabin:$PATH"' > /etc/profile.d/conda.sh

RUN useradd -m conda
COPY build.sh /home/conda/build.sh
RUN chmod +x /home/conda/build.sh

USER conda
WORKDIR /home/conda

RUN conda init &&                                                   \
    conda config --set ssl_verify /etc/ssl/certs/ca-bundle.crt &&   \
    conda config --set always_yes true &&                           \
    git config --global http.sslCAInfo /etc/ssl/certs/ca-bundle.crt

ENTRYPOINT ["/bin/bash", "-lc"]
CMD ["/home/conda/build.sh"]

