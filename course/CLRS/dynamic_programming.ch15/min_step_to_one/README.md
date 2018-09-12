# Problem : Minimum Steps To One

Problem Statement:
On a positive integer, you can perform any one of the following 3 steps.

1. Subtract 1 from it. (n = n - 1)
1. If its divisible by 2, divide by 2. (if n % 2 == 0 , then n = n / 2)
1. If its divisible by 3, divide by 3. (if n % 3 == 0 , then n = n / 3).

Now the question is,
given a positive integer n, find the minimum number of steps that takes n to 1

eg:

1. For n = 1 , output: 0
1. For n = 4 , output: 2  ( 4 / 2 = 2  /2 = 1 )
1. For n = 7 , output: 3    7 - 1 = 6  /3 = 2  /2 = 1 )

## Solution

We have implement 3 solutions:

- naive recursion way
- recursion way with memo (DP)
    - This way will calculate all result in [1, N].
    - It's recursion way.
        - It's slow.
        - It may stack-overflow
            use command to workaround this issue:

            ulimit -s unlimited; ./a.out

- DP bottom to up
    This way will calc 33% ~ 85% result, in general.
    In average, it calc 60% result, (save 40%)

so This bottom-to-up way is fastest.

ref: https://www.codechef.com/wiki/tutorial-dynamic-programming
