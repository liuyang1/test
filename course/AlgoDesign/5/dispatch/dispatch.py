# 2012-05-01 13:07:44 liuyang1 @ 3318
# homework 5-15 best dispatcher problem @ AlgoDesign
# --------------------------------------------------------------
# GREEDY is enough?

def dispatch(time,k):
	lst=[0 for i in range(k)]
	for t in time:
		lst[0]+=t
		lst.sort()
	return max(lst)

def InputIntArray():
	line=raw_input()
	line=line.split()
	return [int(item) for item in line]

def main():
	[n,k]=InputIntArray()
	time=InputIntArray()
	time.sort(reverse=True)
	ret=dispatch(time,k)
	print ret

if __name__=="__main__":
	main()
