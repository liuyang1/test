def solution(A):
    l = 0
    r = sum(A)
    for i in range(len(A)):
        r -= A[i]
        if l == r:
            return i
        l += A[i]
    return -1


A = [-7, 1, 5, 2, -4, 3, 0]
print solution(A)
A = [0, 3, -4, 2, 5, 1, -7]
print solution(A)
#A = [0 for i in range(10**7)]
#print solution(A)
A = [1, 1, 1]
print solution(A)
print solution([])
