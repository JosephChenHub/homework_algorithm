#!/usr/bin/env python

import cvxpy as cvx
import numpy as np
import ecos

# check cvxpy version 

x = cvx.Variable(4, integer=True)
left = np.array([10,40,75,100])
right =np.array([20,60,80,120])

objective = cvx.Maximize(cvx.min(x[1:] - x[:3]))

constraints = [ x >= left, x <= right]

prob = cvx.Problem(objective,constraints)

prob.solve(solver = cvx.ECOS_BB)
print 'using solver ECOS_BB...'
print 'status:', prob.status
print 'optimal value:', prob.value
print 'optimal var:', x.value 

#LP form


#LP dual form
a = cvx.Variable(4 ) #,integer = True)
b = cvx.Variable(4 )#, integer = True)
phi = cvx.Variable(3)
obj = cvx.Minimize(cvx.sum(-a*left + b*right))


cons = [ a >= 0, b >= 0, phi >= 0, 
		b[0] - a[0] + phi[0] == 0,
		b[3] -a[3] - phi[2] == 0,
		b[1:3] - a[1:3] + phi[1:3] - phi[0:2] == 0,
		1 - cvx.sum(phi) <= 0
		]

prob = cvx.Problem(obj, cons)

prob.solve(solver = cvx.ECOS_BB)
print 'using solver ECOS_BB...'
print 'status:', prob.status
print 'optimal value:', prob.value
print 'optimal var1:', a.value 
print 'optimal var1:', b.value 






