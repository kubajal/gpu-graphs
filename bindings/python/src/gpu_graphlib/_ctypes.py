# src/graphlib/_ctypes.py
from __future__ import annotations
import ctypes as ct

ATTR_UNSIGNED = 0
ATTR_INT      = 1
ATTR_FLOAT    = 2
ATTR_PTR      = 3

class AttributeValue(ct.Union):
    _fields_ = [
        ("u", ct.c_uint),
        ("i", ct.c_int),
        ("f", ct.c_float),
        ("ptr", ct.c_void_p),
    ]

class Attribute(ct.Structure):
    _fields_ = [
        ("type", ct.c_int),  # enum -> int
        ("value", AttributeValue),
    ]

class Edge(ct.Structure):
    _fields_ = [
        ("source", ct.c_int),
        ("target", ct.c_int),
        ("data", Attribute),
    ]

class Graph(ct.Structure):
    _fields_ = [
        ("nodes_n", ct.c_int),
        ("edges_n", ct.c_int),
        ("edges", ct.POINTER(Edge)),
        ("nodes", ct.POINTER(Attribute)),
    ]
