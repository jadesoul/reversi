cdef extern from "engine.h":
    const char* server(const char*)

cpdef reversi(x):
    return server(x)


