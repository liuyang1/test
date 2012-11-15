#! /usr/bin/python
# 2012-04-16 00:04:09,liuyang1@dorm
# homework AlgoImpl 3-1
# independtent task optimize manger
# usage:./task.py < input.txt > output.txt
# ---------------------------------------------------------------
# greedy algo is wrong,
# but preprocess so that,first handle job which cost most time
# so can get an approximate best answer
# ---------------------------------------------------------------
# dynamic programming algo :
# cost time T finish task 0~i-1
# so, cost time T+min(la[i],la[i]) finish task 0~i
# ---------------------------------------------------------------
# enum algo
# O(2^n)
import sys
import copy

import copy
# GREEDY is TOTALY WRONG
def greedytask(la,lb):
	if(len(la)==0):
		return [0,0]
	ret=greedytask(la[:-1],lb[:-1])
	t1=ret[0]+la[-1]
	t2=ret[1]+lb[-1]
	if(t1<t2):
		return [t1,ret[1]]
	else:
		return [ret[0],t2]

def pregreedy(la,lb):
	n=len(la)
	m=[max(la[i],lb[i]) for i in range(n)]
	l=[[m[i],la[i],lb[i]] for i in range(n)]
	l.sort(reverse=True)
	la=[l[i][1] for i in range(n)]
	lb=[l[i][2] for i in range(n)]
	return [la,lb]

def gretask(la,lb):
	l=pregreedy(la,lb)
	la=l[0]
	lb=l[1]
	ret=greedytask(la,lb)
	return min(ret)+1

def dyntask(la,lb):
	n=len(la)
	tsize=sum([min(la[i],lb[i]) for i in range(n)])+1
	time=[[0 for i in range(tsize)] for j in range(tsize)]
	cand=[[0 for i in range(tsize)] for j in range(tsize)]
	for i in range(tsize):
		for j in range(tsize):
			if(i>la[1] or j>lb[1]):
				cand[i][j]=1
	for k in range(2,n):
		for i in range(tsize):
			for j in range(tsize):
				if i>la[k] and 1==cand[i-la[k]][j]:
					time[i][j]=1
				elif j>lb[k] and 1==cand[i][j-lb[k]]:
					time[i][j]=1
				else:
					time[i][j]=0
		cand=copy.deepcopy(time)
	retlst=[]
	for i in range(tsize):
		for j in range(tsize):
			if time[i][j]==1:
				retlst.append(max([i,j]))
				break
	return min(retlst)

def enumtask(la,lb):
	time=[[0,0]]
	for i in range(len(la)):
		tmp=copy.deepcopy(time)
		for t in time:
			t[0]=t[0]+la[i]
		for t in tmp:
			t[1]=t[1]+lb[i]
		for j in tmp:
			time.append(j)
	time=[max(t[0],t[1]) for t in time]
	return min(time)

def task(la,lb):
	#return gretask(la,lb)
	return enumtask(la,lb)
	#return dyntask(la,lb)

def main():
	n=int(raw_input())
	la=raw_input()
	la=la.split()
	la=[int(item) for item in la]
	lb=raw_input()
	lb=lb.split()
	lb=[int(item) for item in lb]
	ret=task(la,lb)
	print ret

if __name__=="__main__":
	main()
