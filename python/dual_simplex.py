#!/usr/bin/env python


import cvxpy as cvx
import numpy as np
from pprint import pprint 
'''
==========================================
dual simplex method , compared with cvxpy
primary problem form:
min p = c'*x
s.t. A*x == b
	 x >= 0

the dual problem form:
max d = -b'y
s.t.  A'y + c >= 0	

==========================================

'''
DEBUG = False  #True
inf = 99999999.0

def transform(M, index_row, index_col):
	rows,cols = M.shape
	for i in range(rows):
		if not(i == index_row):
			M[i] += -M[i][index_col]* M[index_row]/M[index_row][index_col]			
		else:
			M[i] = M[i]/M[index_row][index_col]

def select_row(M, index_col, bounds, priority_index = []):
	global inf
	min_val = [inf,inf]
	index_row = [0,0]
	rows,cols = M.shape
	#priority first
	for j in priority_index: 
		if abs(M[j][index_col]) > 1e-6: #not zero
			temp = M[j][cols-1]/M[j][index_col]
			if min_val[0] > temp and temp > -1e-6: # >=0
				if abs(temp) < 1e-6:
					if M[j][index_col] < 0: #if RHS=0, M[j][index_col] must be positive
						continue

				min_val[0] = temp
				index_row[0] = j
	
	for j in bounds:
		if abs(M[j][index_col]) > 1e-6 :
			temp = M[j][cols-1]/M[j][index_col]
			if min_val[1] > temp and temp > -1e-6:
				if abs(temp) < 1e-6:
					if M[j][index_col] < 0:
						continue

				min_val[1] = temp
				index_row[1] = j
	
	out = []
	if abs(min_val[0] - inf) < 1e-6  and abs(min_val[1] - inf) < 1e-6: #min ratio test failed
		status = False
		out.append(status)
		out.append(-1)
	else:
		status = True
		out.append(status)
		if min_val[0] - min_val[1] < 1e-6: 
			out.append(index_row[0])
		else:
			out.append(index_row[1])

	return out

def update_var(M,x_var,lside):
	rows, cols = M.shape
	for i in range(rows-1):
		if abs(M[i][lside[i]] ) > 1e-6:  
			x_var[lside[i] ] = M[i][cols-1]/M[i][lside[i] ]
	


def simplex(c, A = [], b = [], Aeq = [], beq = [], prob = 'Maximize'):

	#check parameters type
	if isinstance(c,list):
		c = np.array(c)
	if isinstance(A,list):
		A = np.array(A)
	if isinstance(b,list):
		b = np.array(b)
	if isinstance(Aeq,list):
		Aeq = np.array(Aeq)
	if isinstance(beq,list):
		beq = np.array(beq)

	#convert to dual problem
	tar_flag = 1
	if prob == 'Maximize':
		c = -c
	elif prob == 'Minimize':
		tar_flag = -1
 
	#construct matrix
	#should add len(b) + len(beq)*2	slack variables
	#we put equalities, inequalities, objective
	len_b = len(b) 
	len_beq = len(beq)
	c = c.reshape(-1)
	len_var = len(c)
	rows = len_b + len_beq*2 + 1
	x_var = np.zeros(len_var + len_b + len_beq*2)

	c.shape = (1,len(c))
	left = c
	right = np.array([0])

	if len_beq > 0:
		beq = beq.reshape(-1)
		left = np.concatenate( (np.tile(Aeq, (2,1)), left ),axis=0)
		right= np.concatenate( (np.tile(beq, 2), right ), axis = 0)
	if len_b > 0:
		b = b.reshape(-1)
		left = np.concatenate( (A,left), axis=0)
		right = np.concatenate((b,right), axis=0) 


	middle = np.concatenate(( -np.ones(len_beq), np.ones(len_beq), np.ones(len_b) ),axis=0)
	middle = np.diag(middle)
	temp = np.zeros(len_beq*2 + len_b)
	temp.shape = (1, temp.shape[0])
	middle = np.concatenate( (middle, temp), axis = 0)
	
	p = np.zeros(rows)
	p[rows-1] = 1

	right.shape = (right.shape[0],1)
	p.shape = (p.shape[0],1)
	right = np.concatenate( (p, right), axis = 1)

	
	M = np.concatenate((left, middle, right), axis = 1)
	cols = M.shape[1]
	#RHS >= 0
	for i in range(rows):
		if M[i][cols-1] < 0:
			M[i] = -M[i]

	M.astype(float)
	
	opt_status = False
	max_iters = 10000

	lside = range(len_var, cols-2)
	update_var(M, x_var, lside)
	s_var = x_var[len_var:cols-2]
	star_index = np.where(s_var < 0)[0] + len_var
	if DEBUG == True:
		print 'initialize x_var:',x_var
		print 'initialize lside:',lside
		print 'initialize star_index:', star_index
	print  'problem has {} equalities and {} inequalities'.format(len_beq, len_b)
#	print 'construct tabular:'
#	pprint(M)
	print 'starting optimization...'
	#solving 


	for times in range(max_iters):	
		if len(star_index) > 0:
			#phase1 , get rid of negative slack variables 
			if DEBUG == True:
				print 'non-standard form...'		
			row = star_index[0] - len_var
			temp = M[row][:len(x_var)]
			index_col = temp.argmax()
			#min ratio 
			bounds = range(0,rows-1)
			priority = star_index - len_var
			out = select_row(M, index_col, bounds, priority) 
			if out[0] == False:
				print 'min ratio test failed!'
				print 'unbounded'
				opt_status = False
				break	

			index_row = out[1]
			if DEBUG == True:
				print 'current row:', row+1
				print 'choose:{} {} {}'.format(index_row+1, index_col+1, M[index_row][index_col])
			transform(M, index_row, index_col)

			#update value
			x_var[lside[index_row]] = 0
			lside[index_row] = index_col
			if DEBUG == True:
				print 'lside',lside	
			update_var(M, x_var, lside)
			s_var = x_var[len_var:cols-2]
			star_index = np.where(s_var < -1e-6)[0] + len_var
			if DEBUG == True:
				print 'update x_var:', x_var
				print star_index	
				np.set_printoptions(precision=3)
				print 'M:'
				pprint(M)
		else: #phase2 
			#check whether to stop
			#all coefficients of the bottom row must >= 0 
			cnt = 0
			for i in range(cols-2):
				if M[rows-1][i] >= -1e-6: #>=0
					cnt += 1
			if cnt == cols-2:
				opt_status = True
				break

			if DEBUG == True:
				print 'phase2:standard solving...'
			bottom = -M[rows-1][:cols-2]
			index_col = bottom.argmax()			
			bounds = range(0, rows-1)
			out = select_row(M,  index_col, bounds)
			if out[0] == False:
				print 'choose col {}'.format(index_col)
				print 'M:', M
				print 'min ratio test failed!'
				print 'unbounded!'
				opt_status = False
				break
			
			index_row = out[1]
			if DEBUG == True:
				print 'current row:', row+1
				print 'choose:{} {} {}'.format(index_row+1, index_col+1, M[index_row][index_col])
			transform(M, index_row, index_col)
			#update value
			x_var[lside[index_row]] = 0
			lside[index_row] = index_col
			if DEBUG == True:
				print 'lside',lside	
			update_var(M, x_var, lside)
			s_var = x_var[len_var:cols-2]
			star_index = np.where(s_var < -1e-6)[0] + len_var
			if DEBUG == True:
				print 'update x_var:', x_var
				print star_index	

				np.set_printoptions(precision=3)
				print 'M:'
				pprint(M)
				

 
	result = []
	result.append(opt_status)
	result.append(tar_flag*M[rows-1][cols-1])
	result.append(x_var[:len_var])

	return result


c = np.array([-7,7,-2,-1,-6,0])
Aeq = np.array([
	 [3,-1,1,-2,0,0],
	 [2, 1,0, 1,1,0],
	 [-1,3,0,-3,0,1]])
beq = np.array([-3,4,12])
len_c = len(c) 

print 'directly solving...'
result = simplex(c,Aeq = Aeq,beq = beq,prob = 'Minimize')
if result[0] == True:
	print 'status: feasible!'
	print 'optimal value:', result[1]
	print 'optimal x:', result[2]
else:
	print 'status: infeasible!'
#dual problem
d_c = -beq
d_A = -Aeq.T
d_b = c

print 'solving dual problem...'
result2 = simplex(c=d_c, A = d_A, b = d_b, prob = 'Maximize')
if result2[0] == True:
	print 'status: feasible!'
	print 'optimal value:', result2[1]
	print 'optimal y:', result2[2]
else:
	print 'status: infeasible!'
	

#using tool --cvxpy
#construct the problem

x = cvx.Variable(len_c)

obj = c*x
objective = cvx.Minimize(obj)

constraints = [ 
	Aeq*x == beq,
	x >= 0
	]
#solve
prob = cvx.Problem(objective, constraints)

#optimal
result = prob.solve()
print 'using tool cvxpy'
print 'optimal value:', result
print 'optimal x:',x.value

