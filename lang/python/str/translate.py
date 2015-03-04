from string import maketrans
oldtbl = "\t\f\n"
newtbl = "  \n"
remap = maketrans(oldtbl, newtbl)
# None : delete
a = "python\tis\fawesome\r\n"
b = a.translate(remap)
print(b)
c = a.translate(remap, '\n')
print(c)
