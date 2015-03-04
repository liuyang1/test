import re


# spliting strings on any of multiple delimiters
line = 'a b;c,d'
ans = re.split(r'[;,\s]\s*', line)
print(ans)

# and delimiters
ans = re.split(r'(;|,|\s)\s*', line)
print(ans)

# reform with new delimiter
values = ans[::2]
# if we don's want delimiter after last token
delimiters = [';'] * (len(values) - 1) + ['']
ans = "".join([v + d for v, d in zip(values, delimiters)])
print(ans)

# same with [], but with parentheses and with noncapture group with ?:
ans = re.split(r'(?:;|,|\s)\s*', line)
print(ans)
