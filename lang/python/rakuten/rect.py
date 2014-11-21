def isCross(a1, a2, b1, b2):
    w1 = a2 - a1
    w2 = b2 - b1
    m1 = (a1 + a2) / 2
    m2 = (b1 + b2) / 2
    dm = abs(m1 - m2)
    ww = (w1 + w2) / 2
    if dm <= ww:
        return True
    else:
        return False

def isCrossAll(K, L, M, N, P, Q, R, S):
    if isCross(K, M, P, R) and isCross(L, N, Q, S):
        return True
    else:
        return False


def CrossArea(K, L, M, N, P, Q, R, S):
    xc1 = max(K, P)
    yc1 = max(L, Q)
    xc2 = min(M, R)
    yc2 = min(N, S)
    return (xc2 - xc1) * (yc2 - yc1)

def rect(x1, y1, x2, y2):
    return (x2 - x1) * (y2 - y1)

def solution(K, L, M, N, P, Q, R, S):
    a1 = rect(K, L, M, N)
    a2 = rect(P, Q, R, S)
    if isCrossAll(K, L, M, N, P, Q, R, S):
        return a1 + a2 - CrossArea(K, L, M, N, P, Q, R, S)
    else:
        return a1 + a2
