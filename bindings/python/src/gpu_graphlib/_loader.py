from __future__ import annotations
import ctypes as ct
import os
from typing import Optional

_LIBNAME = "libgpu_graphlib.so"  # linux only

def _iter_pythonpath_dirs() -> list[str]:
    pp = os.environ.get("PYTHONPATH", "")
    if not pp:
        return []
    # Keep order; ignore empty entries
    return [p for p in pp.split(os.pathsep) if p]

def load_gpu_graphlib() -> ct.CDLL:
    tried: list[str] = []

    for d in _iter_pythonpath_dirs():
        cand = os.path.join(d, _LIBNAME)
        tried.append(cand)
        if os.path.isfile(cand):
            # Load by absolute path so we don't depend on LD_LIBRARY_PATH
            return ct.CDLL(cand)

    raise FileNotFoundError(
        f"Could not find {_LIBNAME} in PYTHONPATH.\n"
        f"PYTHONPATH={os.environ.get('PYTHONPATH','')}\n"
        "Tried:\n  - " + "\n  - ".join(tried)
    )
