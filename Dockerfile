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
    \
    groupadd --gid 1000 vscode; \
    useradd --uid 1000 --gid 1000 -m -s /bin/bash vscode; \
    echo "vscode ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/vscode; \
    chmod 0440 /etc/sudoers.d/vscode

WORKDIR /workspace
CMD ["/bin/bash"]
