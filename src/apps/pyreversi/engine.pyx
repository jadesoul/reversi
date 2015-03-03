# cdef extern from "engine/engine.h":
cdef extern from "bitboard/engine.h":
    const char* server(const char*)

cpdef reversi(x):
    return server(x)