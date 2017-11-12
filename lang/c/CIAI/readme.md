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
| user error    | expected, errnoeous user input               | handle it by program    |
| runtime error | never expected, always indicate program bugs | should halt, use assert |
| exception     | unexpcted, but may recovery from it.         | signal, raise exception(`setjmp` in C) |

# ch4, Memory management

# ch5, More memory management

Arena-based algo

# ch6, List

# ch8, Table

# comment

- 2016-10-09

最开始看这本书，是想学习接口设计，因为觉得这点在去耦合和良好地软件结构设计中，比较重要．
但是现在看了这本书，发现其他方面也收益良多．比如关于错误处理，比如well-defined的概念，都很重要．

会继续看完．

- 2017-11-12
Q: 为什么创建和返回的时候，对于结构要返回的是heap分配的指针，而不是外部传入的指针？

A: 如果是外部传入的指针，则必然需要外部知道结构的大小。这就要求暴露结构的形式。加强了上层对于结构的耦合。

# TODO

- [ ] remove duplicate code
    - union align
    - ROUNDUP
