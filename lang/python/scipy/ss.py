import scipy.stats as ss

a = ['a', 'b', 'd', 'd', 'c', 'b', 'd']

print(ss.rankdata(a)) # default method is average
# [1, 2.5, 6, 6, 4, 2.5, 6]
# 注意：‘b'是并列2，3，因此平均为2.5名
# 'd'是并列5，6，7，因此平均值为7

print(ss.rankdata(a, method='min'))
# [1, 2, 5, 5, 4, 2, 5]
# 并列名次按照最小的排名
# 注意：这个是我们文化当中默认最常见的排名方法

print(ss.rankdata(a, method='dense'))
# [1, 2, 4, 4, 3, 2, 4]
# 并列排名，一起占用一个名次位置。这样在两个并列第2后，是排名第3的

print(ss.rankdata(a, method='ordinal'))
# [1, 2, 5, 6, 4, 3, 7]
# 并列的排名，按照出现序列的顺序来区分排名
