# need external library
import textwrap

s = "asdf asd asd asd asdf"
print(s)
ans = textwrap.fill(s, 10, initial_indent=': ', subsequent_indent='    ')
print(ans)
