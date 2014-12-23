"""
doctest COMPILE docstring to AST, then EXEC it.
when docstring contain backslash, it have to using raw string
or double backslash
"""
compile(r"bytearray(b'\x00')", 'test', 'single')

"""
when docstring not handled correctly, will throw a TypeError:
    ...
"""
compile("bytearray(b'\x00')", 'test', 'single')
