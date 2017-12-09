#!/usr/bin/env python


import cvxpy as cvx
import numpy as np


#dual simplex
#construct dual problem



l = 6
M = np.array([
	[7,-7,2, 1,6,0, 0,0,0,0,0,0, -1,0],
	[-3,1,-1,2,0,0,-1,0,0,0,0,0,  0,3],
	[2, 1,0, 1,1,0,0,-1,0,0,0,0,  0,4],
	[-1,3,0,-3,0,1,0,0,-1,0,0,0,  0,12],
	[-3,1,-1,2,0,0,0,0,0,1,0,0,  0,3],
	[2, 1,0, 1,1,0,0,0,0,0,1,0,  0,4],
	[-1,3,0,-3,0,1,0,0,0,0,0,1,  0,12],
])


M = M.astype(float)
rows,cols = M.shape
inf = 99999999
opt_status = True
last_row = []

for times in range(100):
	#check whether to stop
	cnt = 0
	for i in range(l):
		if M[0][i] <= 0:
			cnt += 1
	if cnt == l: 
		break
	#first row 
	temp = M[0][:l]
	index_col = temp.argmax()
	#min ratio rule
	#if fails, set 0
	min_val = inf
	index_row = 1
	for j in range(1,rows):
		if abs(M[j][index_col]) > 1e-6: #!= 0
			if min_val > M[j][cols-1]/M[j][index_col] and M[j][cols-1]/M[j][index_col] > 0:
				if j not in last_row:
					min_val = M[j][cols-1]/M[j][index_col]
					index_row = j	
					last_row.append(j)

	if min_val == inf:
		print 'unbounded!', M 
		print 'not fisible!'
		opt_status = False
		break

	for i in range(rows):
		if not(i == index_row):
			M[i] += -M[i][index_col]* M[index_row]/M[index_row][index_col]			
		else:
			M[i] = M[i]/M[index_row][index_col]

	print 'choose :{} {}'.format(index_row, index_col)
	print 'M:', M

if opt_status == True:
	print 'optimal value:', M[0][cols-1]
	print M





#using tool --cvxpy
#construct the problem
x = cvx.Variable(6)
c = np.array([-7,7,-2,-1,-6,0])
A = np.array([[3,-1,1,-2,0,0],
	 [2,1,0,1,1,0],
	 [-1,3,0,-3,0,1]])
b = np.array([-3,4,12])
obj = c*x
objective = cvx.Minimize(obj)

constraints = [ 
	A*x == b,
	x >= 0
	]
#solve
prob = cvx.Problem(objective, constraints)

#optimal
result = prob.solve()
print 'using tool cvxpy:'
print 'optimal value:', result
print 'optimal x:',x.value






