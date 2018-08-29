# Max Subarray Sum

We have three solutions

## Naive solution

    Directly evaluate Max(Sum(A[i, j])) (0 <= i < j <= N)

From quick view, It is O(N^3).
However, we could optimize the internal SUM by update new value to previous SUM.
So the SUM need O(1).
By this way, this methold could be O(N^2)

## Divide & Conquer

CLRS introduce this algo. The key is sub-problem partition:

Original issue is P(low, high)
- eval P(low, mid)
- eval P(mid, high)
- eval Max(Sum(A[i, j])) (low <= i < mid < j <= high)

We focus on the #3 clause. As the SUM must include the MID item
so it's A[low, mid], A[mid, high]'s max suffix array/prefix array sum issue.
This sub-problem is O(N)

T(N) = 2 * T(N/2) + O(N)
T(N) = O(N*logN)

This algo is better.

We need consider subscript carefully with this algo.
C consider subscript with left-inclusive right-exclusive style.
This algo need consider it with inclusive style.


# Kadane's Algo

This is one smart Divide & Conquer solution.

Original issue is P, we could convert it to Max(Q(end)) (0 <= end <= N).
Q(end) means: evaluate max subarray which end with END sum.
For quick view, Q(end) is O(N).

We have one update style:
Q(end) = max(A[end], A[end] + Q(end-1))
With this way, Q(end) is O(1).
This original issue is O(N)

# Exerice

## 4.1.1 When does FindMaximumSubArray return if all elements in array are negative?

It will return the largest number in array.

## 4.1.2 Brute-force for naive solution

check MaxSubarray.hs

## 4.1.3

check

- max_subarray.c
- perf.txt
- perf.png

N0 = 8

After change, the N0 point don't change

## 4.1.4

support to return 0

## 4.1.5

check kadane's algo
