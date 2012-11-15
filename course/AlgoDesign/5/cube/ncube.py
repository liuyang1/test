# 2012-05-01 11:05:14 
# liuyang1,class 3318
# homework 5-7 N cube problem @ AlgoDesign
# -----------------------------------------------------------
# just depth Search
# enumerate,chck answer

def Chk(ans,cube):
	for item in ans:
		for i in range(4):
			if cube[i]==item[i]:
				return 0
	return 1

def RotateCube(cube,n2):
	if n2==0:
		return cube
	elif n2==1:
		return [cube[1],cube[2],cube[3],cube[0],cube[4],cube[5]]
	elif n2==2:
		return [cube[2],cube[3],cube[0],cube[1],cube[4],cube[5]]
	elif n2==3:
		return [cube[3],cube[0],cube[1],cube[2],cube[4],cube[5]]

def TranseCube(cube,n1,n2):
	if n1==0:
		return RotateCube(cube,n2)
	elif n1==1:
		tmp=[cube[2],cube[3],cube[4],cube[5],cube[0],cube[1]]
		return RotateCube(tmp,n2)
	elif n2==2:
		tmp=[cube[4],cube[5],cube[0],cube[1],cube[2],cube[3]]
		return RotateCube(tmp,n2)

def Search(cubes,ans,dep):
	for n1 in range(3):
		for n2 in range(4):
			tmp=TranseCube(cubes[dep],n1,n2)
			if Chk(ans,tmp):
				ans.append(tmp)
				if dep==len(cubes)-1:
					return 1
				else:
					return Search(cubes,ans,dep+1)
				ans.pop()
	return 0


def inputIntArray():
	line=raw_input()
	line=line.split()
	return [int(item) for item in line]

def num2color(colorlist,n):
	return colorlist[n]

def num2colorCubes(colorlist,cubes):
	return [[num2color(colorlist,item2) for item2 in item1] for item1 in cubes]

def dispColorCube(colorcube):
	for item1 in colorcube:
		for item2 in item1:
			print "\b"+item2,
		print

def main():
	n=inputIntArray()[0]
	colors=raw_input()
	cubes=[]
	for i in range(n):
		cubes.append(inputIntArray())
	ans=[cubes[0]]
	Search(cubes,ans,1)
	colorans=num2colorCubes(colors,ans)
	dispColorCube(colorans)

if __name__=="__main__":
	main()
