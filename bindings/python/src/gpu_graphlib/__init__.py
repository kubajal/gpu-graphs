import ctypes as ct

from bindings.python.src.gpu_graphlib._ctypes import Attribute, Graph, Edge, Logger
from ._loader import load_gpu_graphlib

LOG_LEVEL_ERROR = 0
LOG_LEVEL_INFO = 1
LOG_LEVEL_DEBUG = 2

_lib = load_gpu_graphlib()

_lib.malloc_graph.argtypes = [ct.c_uint, ct.c_uint]
_lib.malloc_graph.restype = ct.POINTER(Graph)
def malloc_graph(nodes_n: int, edges_n: int) -> ct._Pointer:
    return _lib.malloc_graph(nodes_n, edges_n)

_lib.free_graph.argtypes = [ct.POINTER(Graph)]
_lib.free_graph.restype = None
def free_graph(graph: ct._Pointer) -> None:
    _lib.free_graph(graph)

_lib.bfs.argtypes = [ct.POINTER(Graph), ct.c_uint, ct.c_uint]
_lib.bfs.restype = ct.c_int
def bfs(graph: ct._Pointer, source: int, target: int) -> int:
    return int(_lib.bfs(graph, source, target))

_lib.add_edge.argtypes = [ct.POINTER(Graph), ct.c_uint, ct.c_uint, ct.c_uint, ct.c_int, ct.c_uint]
_lib.add_edge.restype = None
def add_edge(graph: ct._Pointer, edge_id: int, source: int, target: int, attribute_type: int, attribute_value: int) -> None:
    _lib.add_edge(graph, edge_id, source, target, attribute_type, attribute_value)

_lib.add_node.argtypes = [ct.POINTER(Graph), ct.c_uint, ct.c_int, ct.c_uint]
_lib.add_node.restype = None
def add_node(graph: ct._Pointer, node_id: int, attribute_type: int, attribute_value: int) -> None:
    _lib.add_node(graph, node_id, attribute_type, attribute_value)

_lib.set_log_level.argtypes = [ct.POINTER(Logger), ct.c_uint]
_lib.set_log_level.restype = None
def set_log_level(logger: ct._Pointer, log_level: int) -> None:
    _lib.set_log_level(logger, log_level)

_lib.get_static_logger.argtypes = None
_lib.get_static_logger.restype = ct.POINTER(Logger)
def get_static_logger() -> ct._Pointer:
    return _lib.get_static_logger()

static_logger = _lib.get_static_logger()
_lib.init_logger(static_logger)
