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

# ch4, Memory management

# ch5, More memory management

Arena-based algo

# ch6, List

# ch7, Table

# comment

- 2016-10-09

最开始看这本书，是想学习接口设计，因为觉得这点在去耦合和良好地软件结构设计中，比较重要．
但是现在看了这本书，发现其他方面也收益良多．比如关于错误处理，比如well-defined的概念，都很重要．

会继续看完．

# TODO

- [ ] remove duplicate code
    - union align
    - ROUNDUP
