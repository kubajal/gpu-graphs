FROM intel/oneapi-basekit:2025.2.0-0-devel-ubuntu22.04

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update; \
    apt-get install -y \
      sudo \
      unzip \
      libdrm-dev \
      cmake-curses-gui \
      python3-dev \
      python3-pip \
      python3.10-venv; \
    rm -rf /var/lib/apt/lists/*;


RUN set -eux; \
    wget -O metrics-discovery-1.12.172.zip \
      https://github.com/intel/metrics-discovery/archive/refs/tags/metrics-discovery-1.12.172.zip; \
    unzip -q metrics-discovery-1.12.172.zip; \
    cd metrics-discovery-metrics-discovery-1.12.172; \
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Release; \
    cmake --build build -j"$(nproc)"; \
    cmake --install build; \
    rm -rf /tmp/*
RUN set -eux; \
    mkdir -p /etc/sudoers.d; \
    chmod 0750 /etc/sudoers.d; \
    groupadd --gid 1000 vscode; \
    useradd --uid 1000 --gid 1000 -m -s /bin/bash vscode; \
    echo "vscode ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/vscode; \
    chmod 0440 /etc/sudoers.d/vscode


COPY dummy_keys/id_rsa /home/vscode/.ssh/id_rsa
COPY dummy_keys/id_rsa.pub /home/vscode/.ssh/id_rsa.pub
COPY dummy_keys/id_rsa.pub /home/vscode/.ssh/authorized_keys
RUN chmod 600 /home/vscode/.ssh/id_rsa && \
    chmod 644 /home/vscode/.ssh/id_rsa.pub && \
    printf "Host *\n\tStrictHostKeyChecking no\n" > /home/vscode/.ssh/config && \
    chmod 600 /home/vscode/.ssh/config

RUN mkdir /run/sshd

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
RUN apt-get update && apt-get install -y \
    clinfo

WORKDIR /workspace
USER vscode
CMD ["/bin/bash"]

