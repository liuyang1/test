# ch1.01 范畴:复合的本质

## 用你最喜欢的语言（如果你最喜欢的是 Haskell，那么用你第二喜欢的语言）尽力实现一个恒等函数。

```
This is easy for dynamic language or functional language, but hard for imperative language as it have no general type.

check id.py, id.c
```

## 用你最喜欢的语言实现函数的复合，它接受两个函数作为参数值，返回一个它们的复合函数。

```
check compose.py, compose.cpp

I don't have solution to compose function in pure C. :(
```

## 写一个程序，测试你写的可以复合函数的函数是否能支持恒等函数。

```
check compose.py, compose.cpp
```

## 互联网是范畴吗？链接是态射吗？

```
No.

If google link to Facebook, Facebook link to Twitter, we cannot enuse hava link from google to Twitter.
```

## 脸书是一个以人为对象，以朋友关系为态射的范畴吗？

```
No.

我的附庸的附庸,不是我的附庸.
```

## 一个有向图，在什么情况下是一个范畴？

```
- 完全没有任何连接
- 只存在A -> B的链接
- ... 这个问题,还没有想清楚
```
