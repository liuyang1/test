import re
# replace MM/DD/YYYY -> YYYY-MM-DD format
text = 'Today is 3/4/2015'
ans = re.sub(r'(\d+)/(\d+)/(\d+)', r'\3-\1-\2', text)
print(ans)
