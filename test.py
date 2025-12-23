import sys
import ctypes as ct

print("Python:", sys.version)

import gpu_graphlib as gpu

g = gpu.malloc_graph(1, 2)
g = gpu.free_graph(g)