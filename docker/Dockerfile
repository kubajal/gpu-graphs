FROM intel/oneapi-basekit:2025.2.0-0-devel-ubuntu22.04

# ----------------------------------------------------------------------
# Intel Arc (A770) user-space GPU compute stack (NEO + IGC) from GitHub.
# These are the exact versions you provided.
# Notes:
# - The kernel driver (i915) and /dev/dri device nodes must be provided by the HOST.
# - We install only .deb files (skip the optional *dbgsym*.ddeb packages).
# ----------------------------------------------------------------------
WORKDIR /tmp/neo

RUN set -eux; \
    wget -q https://github.com/intel/intel-graphics-compiler/releases/download/igc-1.0.17537.20/intel-igc-core_1.0.17537.20_amd64.deb; \
    wget -q https://github.com/intel/intel-graphics-compiler/releases/download/igc-1.0.17537.20/intel-igc-opencl_1.0.17537.20_amd64.deb; \
    wget -q https://github.com/intel/compute-runtime/releases/download/24.35.30872.22/libigdgmm12_22.5.0_amd64.deb; \
    wget -q https://github.com/intel/compute-runtime/releases/download/24.35.30872.22/intel-level-zero-gpu_1.3.30872.22_amd64.deb; \
    wget -q https://github.com/intel/compute-runtime/releases/download/24.35.30872.22/intel-level-zero-gpu-legacy1_1.3.30872.22_amd64.deb; \
    wget -q https://github.com/intel/compute-runtime/releases/download/24.35.30872.22/intel-opencl-icd_24.35.30872.22_amd64.deb; \
    wget -q https://github.com/intel/compute-runtime/releases/download/24.35.30872.22/intel-opencl-icd-legacy1_24.35.30872.22_amd64.deb; \
    dpkg -i ./*.deb || true; \
    apt-get update; \
    apt-get install -y -f; \
    rm -rf /var/lib/apt/lists/*; \
    cd /; \
    rm -rf /tmp/neo
    
# Use clang by default inside the container
ENV CC=icx
ENV CXX=icpx

RUN apt-get update && apt-get install -y \
    clinfo

RUN apt-get install cmake-curses-gui

ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=1000

RUN set -eux; \
    apt-get update; \
    apt-get install -y --no-install-recommends sudo; \
    rm -rf /var/lib/apt/lists/*; \
    mkdir -p /etc/sudoers.d; \
    chmod 0750 /etc/sudoers.d; \
    groupadd --gid 1000 vscode; \
    useradd --uid 1000 --gid 1000 -m -s /bin/bash vscode; \
    echo "vscode ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/vscode; \
    chmod 0440 /etc/sudoers.d/vscode

RUN apt-get update && apt-get install -y \
    python3-dev \
    python3-pip \
    python3.10-venv

COPY dummy_keys/id_rsa /home/vscode/.ssh/id_rsa
COPY dummy_keys/id_rsa.pub /home/vscode/.ssh/id_rsa.pub
COPY dummy_keys/id_rsa.pub /home/vscode/.ssh/authorized_keys
RUN chmod 600 /home/vscode/.ssh/id_rsa && \
    chmod 644 /home/vscode/.ssh/id_rsa.pub && \
    printf "Host *\n\tStrictHostKeyChecking no\n" > /home/vscode/.ssh/config && \
    chmod 600 /home/vscode/.ssh/config
    
COPY metrics-discovery-api/lib/* /usr/lib/x86_64-linux-gnu
COPY metrics-discovery-api/package/libigdmd.pc /usr/lib/x86_64-linux-gnu/pkgconfig/libigdmd.pc
COPY metrics-discovery-api/include/metrics_discovery_api.h /usr/include/metrics_discovery_api.h

RUN rm /usr/lib/x86_64-linux-gnu/libmd.so.0 && \
    rm /usr/lib/x86_64-linux-gnu/libmd.so.0.0.5

# needed for sshd process
RUN mkdir -p /run/sshd

ENV CC=gcc
ENV CXX=g++
RUN apt-get install -y wget unzip
RUN cd /tmp && \
    wget https://github.com/intel/metrics-discovery/archive/refs/tags/metrics-discovery-1.12.172.zip
RUN cd /tmp && \
    unzip metrics-discovery-1.12.172.zip
RUN cd /tmp/metrics-discovery-metrics-discovery-1.12.172/
RUN apt-get install -y cmake libdrm-dev
RUN cd /tmp/metrics-discovery-metrics-discovery-1.12.172/ && \
    mkdir build && cd build && cmake ..

RUN cd /tmp/metrics-discovery-metrics-discovery-1.12.172/build && \
    cmake --build . -j 8
RUN cd /tmp/metrics-discovery-metrics-discovery-1.12.172/build && \
    make install


WORKDIR /workspace
CMD ["/bin/bash"]

