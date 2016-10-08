# ch1
## Programming Style

- declare var at its scope / chunk
- shorter var name at smaller scope.

# ch2

# ch3, Atoms

This atom is concept from lisp. Unique, immutable value. Use it to represent symbol.

# ch3, Exceptions and Assertions

| error type    |                                              | solution                |
|---------------|----------------------------------------------|-------------------------|
| user error    |                                              | handle it by program    |
| runtime error | never expected, always indicate program bugs | should halt, use assert |
| exception     | unexpcted, but may recovery from it.         | signal, raise exception(`setjmp` in C) |
