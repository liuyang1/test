a = [[9,8,1,6],[9,6,4,3],[1,4,5,0],[9,5,4,8]]
b = [[3,6,5,2],[2,6,3,3],[0,5,5,2],[7,7,0,6]]
import numpy as np

b = np.matrix(b)
print np.dot(a,b.T)

a = [[4,7,6,6],[7,8,3,2],[6,6,3,8],[6,9,8,4]]
a = np.matrix(a)
print np.dot(a, a.T)
