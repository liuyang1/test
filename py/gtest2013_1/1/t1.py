#! /usr/bin/env python

char2strTbl = {"0":"zero","1":"one","2":"two","3":"three","4":"four", "5":"five","6":"six","7":"seven","8":"eight","9":"nine"}

freq2strTbl = {2:"double",3:"triple",4:"quadruple",5:"quintuple",
        6:"sextuple",7:"septuple",8:"octuple",9:"nonuple",10:"decuple"}

def char2str(val):
    return char2strTbl[val]

def freq2str(freq):
    return freq2strTbl[freq]

def trans0(val,freq):
    if freq == 0:
        return ""
    elif freq == 1:
        return char2str(val)
    else:
        return freq2str(freq) + " " + char2str(val) 


def trans(str):
    val = '1'
    freq = 0
    ret = ""
    for i in str:
        if i == val:
            freq +=1
        else:
            if freq>10:
                for i in range(freq):
                    ret += char2str(val) + " "
            elif freq!=0:
                ret += trans0(val,freq) + " "
            val = i
            freq = 1
    if freq>10:
        for i in range(freq):
            ret += char2str(val) + " "
    elif freq!=0:
        ret += trans0(val,freq) + " "
    return ret
        

def calc(num,fmt):
    cum = 0
    ans = ""
    for N in fmt:
        str = num[cum:cum+N]
        ret = trans(str)
        ans += ret
        cum += N
    ans = ans.strip()
    return ans


def testcase(seq):
    line = raw_input()
    line = line.split()
    num = line[0]
    fmt = line[1].split("-")
    fmt = [int(n) for n in fmt]
    ret = calc(num,fmt)
    print "Case #%d: %s" %(seq,ret)


if __name__ == "__main__":
    n = int(raw_input())
    for i in range(n):
        testcase(i+1)
