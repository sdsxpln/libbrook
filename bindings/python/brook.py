from ctypes import *

brook = CDLL('/usr/lib/libbrook.so')

brook.b4r_lower.restype = c_char_p
brook.b4r_upper.restype = c_char_p

def py_b4r_lower(s):
    return brook.b4r_lower(c_char_p(s))

def py_b4r_upper(s):
    return brook.b4r_upper(c_char_p(s))