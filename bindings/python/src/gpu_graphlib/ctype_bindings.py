# src/graphlib/_ctypes.py
from __future__ import annotations
import ctypes as ct

class AttributeType(ct.c_int):
    ATTR_UNSIGNED = 0
    ATTR_INT      = 1
    ATTR_FLOAT    = 2
    ATTR_PTR      = 3

class AttributeValue(ct.Union):
    _fields_ = [
        ("_u", ct.c_uint)
        # ("i", ct.c_int),
        # ("f", ct.c_float),
        # ("ptr", ct.c_void_p),
    ]
    @property
    def u(self) -> int:
        return self._u

class Attribute(ct.Structure):
    _fields_ = [
        ("_type", AttributeType),
        ("_value", AttributeValue),
    ]
    @property
    def type(self) -> AttributeType:
        return self._type
    @property
    def value(self) -> AttributeValue:
        return self._value

class Edge(ct.Structure):
    _fields_ = [
        ("_source", ct.c_int),
        ("_target", ct.c_int),
        ("_data", Attribute),
    ]
    @property
    def source(self) -> int:
        return self._source
    @property
    def target(self) -> int:
        return self._target
    @property
    def data(self) -> Attribute:
        return self._data

class Graph(ct.Structure):
    _fields_ = [
        ("_nodes_n", ct.c_int),
        ("_edges_n", ct.c_int),
        ("_edges", ct.POINTER(Edge)),
        ("_nodes", ct.POINTER(Attribute)),
    ]
    @property
    def nodes_n(self) -> int:
        return self._nodes_n
    @property
    def edges_n(self) -> int:
        return self._edges_n
    @property
    def edges(self) -> list[Edge]:
        return self._edges
    @property
    def nodes(self) -> list[Edge]:
        return self._nodes

class Logger(ct.Structure):
    _fields_ = [
        ("log_level", ct.c_int),
        ("print_location", ct.c_int),
        # to do: the rest of C Logger fields
    ]

import ctypes as ct
from importlib.resources import files

_lib_path = files(__package__).joinpath(".lib/libgpu_graphlib.so")
_lib = ct.CDLL(str(_lib_path))

_lib.malloc_graph.argtypes = [ct.c_uint, ct.c_uint]
_lib.malloc_graph.restype = ct.POINTER(Graph)
def malloc_graph(nodes_n: int, edges_n: int) -> ct._Pointer:
    return _lib.malloc_graph(nodes_n, edges_n)

_lib.free_graph.argtypes = [ct.POINTER(Graph)]
_lib.free_graph.restype = None
def free_graph(graph: ct._Pointer) -> None:
    _lib.free_graph(graph)

_lib.bfs.argtypes = [ct.POINTER(Graph), ct.c_uint, ct.c_uint, ct.POINTER(Logger)]
_lib.bfs.restype = ct.c_int
def bfs(graph: ct._Pointer, source: int, target: int, logger: ct._Pointer) -> int:
    return int(_lib.bfs(graph, source, target, logger))

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

_lib.print_graph.argtypes = [ct.POINTER(Graph), ct.c_uint, ct.c_uint]
_lib.print_graph.restype = None
def print_graph(graph: ct._Pointer, current: int, indent: int) -> ct._Pointer:
    return _lib.print_graph(graph, current, indent)

static_logger = _lib.get_static_logger()
_lib.init_logger(static_logger)