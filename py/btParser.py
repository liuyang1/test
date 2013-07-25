# bencode @ bittorrent 解码方法
# 参考wiki中关于bencode的解码方法

import codecs

def parser(s):
    if s[0]=='d':
        return parserDct(s)
    elif s[0]=='l':
        return parserLst(s)
    elif s[0]=='i':
        return parserInt(s)
    elif s[0].isdigit():
        return parserStr(s)

def parserInt(s):
    s=s[1:]# skip "i"
    i=0
    n=0
    while s[i].isdigit():
        n=10*n + int(s[i])
        i = i + 1
    if s[i]!='e':
        return (False)
    return (n,s[i+1:])


def str2int(s):
    i,n=0,0
    while s[i].isdigit():
        n = 10*n + int(s[i])
        i = i + 1
    return n


def parserStr(s):
    n=str2int(s)
    pos=s.find(':')
    return (s[pos+1:pos+1+n],s[pos+n+1:])


def parserLst(s):
    s=s[1:]
    lst=[]
    while 1:
        v,other = parser(s)
        lst.append(v)
        if other[0] == 'e':
            break
        s=other
    return lst,other[1:]

def parserDct(s):
    s=s[1:]
    dct={}
    while 1:
        k,other = parserStr(s)
        v,other = parser(other)
        dct[k]=v
        if other[0] == 'e':
            break
        s=other
    return dct,other[1:]


def testParserInt():
    print parserInt("i1234e")
    print parserInt("i1234e1")


def testParserStr():
    print parserStr("4:spam")
    print parserStr("4:spami")
    print parserStr("8:http://a")


def testParserLst():
    print parserLst("li1234e4:spame")
    print parserLst("li1234e4:spamee")


def testParserDct():
    print parserDct("d4:spami123e1:k4:spame")
    print parserDct("d4:spami123e1:k4:spamee")


def printDct(d,fp,prefix=""):
    for k,v in d.iteritems():
        if isinstance(v,dict):
            fp.write(prefix + str(k) + " :\n")
            printDct(v,fp,prefix+"\t")
        elif isinstance(v,list):
            fp.write(prefix + str(k) + " :\n")
            for l in v:
                fp.write(prefix+ "\t" + str(l)+"\n")
        elif isinstance(v,str):
            fp.write(prefix + str(k) + " : ")
            fp.write(prefix + v + "\n")
        else:
            fp.write(prefix + str(k) + " : " + str(v) + "\n")

def testParserFile(filename):
    fp = open(filename,"r")
    txt = fp.read()
    fo = open("out","w")
    fo.encoding="utf-8"
    res,other = parser(txt)
    printDct(res,fo)
    fp.close()
    fo.close()

if __name__=="__main__":
    testParserFile("1.torrent")
