# use dict in python

dct = {}
while True:
    l = raw_input()
    if l == "":
        break
    l = l.split()
    dct[l[1]] = l[0]

while True:
    try:
        l = raw_input()
    except EOFError:
        break
    # old code
    # it use two times search, change to try-except to save one time
    # if l in dct.keys():
    #     print dct[l]
    # else:
    #     print 'eh'
    # new code use 5.6s,
    # however, the limit is 10s. so we just pass in line
    try:
        w = dct[l]
    except KeyError:
        w = 'eh'
    print w

