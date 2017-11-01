#! /usr/bin/env python2
"""
demo best way to write auto-generated file by python
- use `with` in python to auto close file after write
- use `format` with dict to write many variables to string
- use multiple-line raw string to pretty show without verbosed string and '\n'
"""

filename = "1.txt"
dct = {}
dct["var"] = "def"
dct["topdir"] = "top...."
lst = ["A", "B"]
dct["list"] = " ".join(lst)

fmt = """### thsi file is auto genereated, dont's edit it
export topdir={topdir}

def: {list}
\t{var}"""

s = fmt.format(**dct)

with open(filename, "w") as f:
    print >> f, s
