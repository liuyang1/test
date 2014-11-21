#! /usr/bin/python
# 2012-04-17 13:43:50 liuang1@dorm
# homework 4-7 @ AlgoDesign
# Usage: queue.py < input.txt > output.txt
# ----------------------------------------------------------------------
# multiService problem
# this is a problem of queuing theory
# cost time LEAST service FIRST

def queue(s,task):
	task.sort()
	l=[[0 for col in range(s)] for row in range(s)]
	nt=0
	for item in task:
		val=l[nt][-1]+item
		l[nt].append(val)
		m=l[0][-1]
		nt=0
		for i in range(1,s):
			if(m>l[i][-1]):
				nt=i
	Sum=sum([sum(li) for li in l])
	print Sum/(len(task)+0.0)

def inputIntArray():
	line=raw_input()
	line=line.split()
	return [int(item) for item in line]

def main():
	line=inputIntArray()
	n=line[0]
	s=line[1]
	task=inputIntArray()
	queue(s,task)

if __name__=="__main__":
	main()
