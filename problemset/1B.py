def n2AA(n):
    """
    >>> n2AA(1)
    'A'
    >>> n2AA(25)
    'Y'
    >>> n2AA(26)
    'Z'
    >>> n2AA(27)
    'AA'
    >>> n2AA(52)
    'AZ'
    >>> n2AA(55)
    'BC'
    """
    s = ''
    while n != 0:
        n -= 1
        v = n % 26
        n = n // 26
        s = chr(ord('A') + v) + s
    return s

def AA2n(s):
    """
    >>> AA2n('AA')
    27
    >>> AA2n('A')
    1
    >>> AA2n('BC')
    55
    """
    v = 0
    for i in s:
        v *= 26
        n = ord(i) - ord('A')
        n += 1
        v += n
    return v
def toExcel(a, b):
    """
    >>> toExcel(55, 23)
    'BC23'
    """
    return "%s%d" % (n2AA(a), b)

def Excelto(s):
    """
    >>> Excelto('BC23')
    (55, 23)
    """
    for i in range(len(s)):
        if s[i].isdigit():
            idx0 = i
            break
    return (AA2n(s[:idx0]), int(s[idx0:]))


def toRXCY(a, b):
    """
    >>> toRXCY(55, 23)
    'R23C55'
    """
    return "R%dC%d" % (b, a)

def RXCYto(s):
    """
    >>> RXCYto('R23C55')
    (55, 23)
    """
    idxc = s.index('C')
    b = int(s[1:idxc])
    a = int(s[idxc+1:])
    return (a, b)


def isRXCY(s):
    """
    >>> isRXCY('RRCC233')
    False
    >>> isRXCY('R22C22')
    True
    """
    try:
        idx0 = s.index('R')
        idx1 = s.index('C')
    except:
        return False
    if idx0 != -1 and idx1 != -1:
        return any([str(i) in s[idx0+1:idx1] for i in range(10)])
    return False


def main():
    n = int(input())
    for i in range(n):
        line = input()
        if isRXCY(line):
            a, b = RXCYto(line)
            print(toExcel(a, b))
        else:
            a, b = Excelto(line)
            print(toRXCY(a, b))

main()
