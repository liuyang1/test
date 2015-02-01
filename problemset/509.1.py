# AC
def c(n):
    seq = [1 for i in range(n)]
    for i in range(n - 1):
        for i in range(n - 1):
            seq[i + 1] = seq[i] + seq[i + 1]
    return seq[-1]


if __name__ == "__main__":
    n = input()
    n = int(n)
    print(c(n))
