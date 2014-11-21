#! /usr/bin/python
# 2012-04-17 14:57:04 liuyang1 @ dorm
# homework 4-1 @ AlogDesing
#------------------------------------------------------------------------------
# 1,transfer to GRAPH COLORING problem
# 2,using max degree number of Node FISRT Welsh-Powell algo
# ref:http://hi.baidu.com/nkhzj/blog/item/761411306c918c9da9018e82.html

def chkmeeting(meeting1,meeting2):
	if(meeting2[0]>meeting1[0] and meeting2[0]<meeting1[1]):
		return 1
	if(meeting2[1]>meeting1[0] and meeting2[1]<meeting1[1]):
		return 1
	if(meeting1[0]>meeting2[0] and meeting1[0]<meeting2[1]):
		return 1
	if(meeting1[1]>meeting2[0] and meeting1[1]<meeting2[1]):
		return 1
	return 0

# construct Graph using boolean matrix
def graph(n,tlst):
	mat=[[0 for i in range(n)] for j in range(n)]
	for i in range(n):
		for j in range(n):
			if i==j:
				continue;
			if chkmeeting(tlst[i],tlst[j]) :
				mat[i][j]=1
	return mat

def inputIntArray():
	line=raw_input()
	line=line.split()
	return [int(item) for item in line]

def coloring(mat):
	# sort order
	n=len(mat)
	degree=[ [i,sum(mat[i])] for i in range(n)]
	degree.sort(key=lambda l:(l[1]),reverse=True)
	colorlst=[0 for i in range(n)]
	candcolorlst=[0 for i in range(n)]
	for i in range(n):
		index=degree[i][0]
		# coloring the index th node
		color=candcolorlst[index]
		colorlst[i]=color
		for j in range(n):
			if(i==index):
				continue
			if(mat[index][j]==1):
				candcolorlst[j]=color+1
	return max(colorlst)+1

def main():
	n=inputIntArray()[0]
	tlst=[]
	for i in range(n):
		tlst.append(inputIntArray())
	mat=graph(n,tlst)
	ret=coloring(mat)
	print ret

if __name__=="__main__":
	main()
