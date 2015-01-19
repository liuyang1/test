import sh
git = sh.git.bake(_cwd='/home/liuy/wiki')

print(git.branch())
print(git('--no-pager', 'log'))
