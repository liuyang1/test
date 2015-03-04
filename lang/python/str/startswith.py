a = ['http://aa', 'https://bb', 'ftp://cc']
ans = [i for i in a if i.startswith('http')]
print(ans)

# use tuple as param
ans = [i for i in a if i.startswith(('http', 'ftp'))]
print(ans)
