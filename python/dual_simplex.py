#!/usr/bin/env python


import cvxpy as cvx
import numpy as np


#dual simplex 
l = 6
M = np.array([[7,-7,2,1,6,0,-1,0],
	[3,-1,1,-2,0,0,0,-3],
	[2,1, 0, 1,1,0,0,4],
	[-1,3,0,-3,0,1,0,12]])

'''
l = 2
M = np.array([[13,23, 0,0,0, -1,0],
			 [ 5,15, 1, 0,0, 0,480],
			 [4, 4, 0, 1, 0, 0,160],
			 [35, 20, 0, 0, 1,0, 1190]])
'''

M = M.astype(float)
rows,cols = M.shape
times = 0
while(1):
	times += 1
	cnt = 0
	for i in range(l):
		if M[0][i] <= 0:
			cnt += 1
	if cnt == l: #stop
		break
	temp = M[0][0:l]
	index_col = temp.argmax()
	min_val = 0
	index_row = 1
	for j in range(1,rows):
		if abs(M[j][index_col]) > 1e-6: 
			if min_val > M[j][cols-1]/M[j][index_col]:
				min_val = M[j][cols-1]/M[j][index_col]
				index_row = j			
	for i in range(rows):
		if not i == index_row:
			M[i] += -M[i][index_col]* M[index_row]/M[index_row][index_col]			
		else:
			M[i] = M[i]/M[i][index_col]
	if times > 100:	
		break

print 'optimal value:', M[0][cols-1]
print M





#using tool --cvxpy
#construct the problem
x = cvx.Variable(6)
a = [-7,7,-2,-1,-6,0]
objective = cvx.Minimize(a*x)
constraints = [ [3,-1,1,-2,0,0]*x == -3, 
	[2,1,0,1,1,0]*x == 4,
	[-1,3,0,-3,0,1]*x == 12,
	x >= 0]
prob = cvx.Problem(objective, constraints)

#optimal
result = prob.solve()
print 'using tool cvxpy:'
print 'optimal value:', result
print 'optimal x:',x.value






