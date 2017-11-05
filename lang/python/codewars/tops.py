def tops(msg):
    j, k, s = 1, 1, ""
    while j < len(msg):
        s += msg[j]
        k += 4
        j += k
    return s[::-1]


print(tops(""), "")
print(tops("12"), "2")
print(tops("abcdefghijklmnopqrstuvwxyz12345"), "3pgb")
print(tops("abcdefghijklmnopqrstuvwxyz1236789ABCDEFGHIJKLMN"), "M3pgb")
