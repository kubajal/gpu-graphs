# src/graphlib/_ctypes.py
from __future__ import annotations
import ctypes as ct
import enum

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
    def _edges(self) -> list[Edge]:
        return self._edges

    @property
    def _nodes(self) -> list[Edge]:
        return self._nodes

class Logger(ct.Structure):
    _fields_ = [
        ("log_level", ct.c_int),
        ("print_location", ct.c_int),
        # to do: the rest of C Logger fields
    ]
