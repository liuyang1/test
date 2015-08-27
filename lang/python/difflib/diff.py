import difflib

print difflib.get_close_matches('appel', ['ape', 'apple', 'peach', 'puppy'])


a = 'one\ntwo\nthree\n'.splitlines(1)
b = 'ore\ntree\nemu\n'.splitlines(1)
print a
print b
ret = difflib.ndiff(a, b)
for i in ret:
    print i,
