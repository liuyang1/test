#! /usr/bin/env python3
def isGift3(a, b, c):
    if a == b or b == c:
        return False
    if b - a > 1 or c - b > 1:
        return False
    return True


def isGift(lst):
    """
    >>> isGift([8, 2, 1])
    False
    """
    # my silly error, consider SET will keep order.
    # I definitely know this point, but miss it in contest
    lst = list(set(lst))
    lst = sorted(lst)
    if len(lst) < 3:
        return False
    for i in range(2, len(lst)):
        if isGift3(lst[i - 2], lst[i - 1], lst[i]):
            return True
    return False


def main():
    n = int(input())
    line = input()
    line = [int(i) for i in line.split()]
    line = line[0:n]
    r = "YES" if isGift(line) else "NO"
    print(r)


if __name__ == "__main__":
    main()
