with open("1.txt") as f:
    dct = {}
    dct["var"] = "def"
    dct["topdir"] = "top...."
    lst = ["A", "B"]
    dct["list"] = " ".join(lst)
    fmt = """#$## thsi file is auto
dont's edit it
export topdir=${topdir}
def
${var}
"""
    s = fmt.format(**dct)
    print s >> f
