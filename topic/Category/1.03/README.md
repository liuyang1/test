# 1
- 从下面的东西生成自由范畴：(1) 有一个结点，没有边的图；(2) 有一个结点并且有一条边（有方向）的图；(3) 有两个结点并且二者之间有一条边的图；(4) 有一个结点，有 26 个箭头并且每个箭头标记着字母表上的一个字母的图。

(1) 生成之后,依然只有一个结点,没有边;
(2) 那么这个边,必然是从该结点出发到自己
(3) 不变
(4) ~不变,没有复合箭头~
    一个结点,带有无数(字典个数)的箭头

# 2
- 这是哪种序？(1) 伴随着包含关系的一组集合的集合；(2) 伴随着子类型关系的 C++ 的类型构成的集合。

(1) 偏序
check image Hasse_diagram_of_powerset_of_3.svg

(2) 偏序,C++支持多重继承

# 3
- Bool 是两个值的集合，看看它能不能分别与 && 与 || 构成幺半群（集合理论中的）。

    ```
    Bool = True | False

    True && True = True
    False && True = False
    => True is left identify; same logic, True is right identify of `&&` operation

    True || False = True
    False || False = False
    => False is left identify; same logic, False is right identify of `||` operation
    ```

# 4
- 用 AND 运算表示 Bool 幺半群：给出态射以及它们的复合法则。

    ```
    +---+   (and false)  +----+
    ^  True --------> False   ^ (and true), (and false)
    +---+                +----+
    (and true)
    ```

# 5
- 将 (模 3）的加法 的复合表示为幺半群范畴.

    ```
          (+ 1)             (+ 1)
    0 <-------------> 1 <-----------> 2
    ^     (+ 2)             (+ 2)     ^
    |            (+ 2                 |
    +---------------------------------+
                 (+ 1)

    ```
