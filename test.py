
import gpu_graphlib as gpu

static_logger = gpu.get_static_logger()
gpu.set_log_level(static_logger, gpu.LOG_LEVEL_DEBUG)
g = gpu.malloc_graph(1, 2)
g = gpu.free_graph(g)
