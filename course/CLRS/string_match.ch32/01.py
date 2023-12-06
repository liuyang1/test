def sm_naive(T, P, n, m):
    for s in range(n - m + 1):
        if P == T[s: s + m]:
            print "find with shift=", s
        else:
            print P, T[s: s + m]


T = "0010010001"
P = "0001"
sm_naive(T, P, len(T), len(P))
