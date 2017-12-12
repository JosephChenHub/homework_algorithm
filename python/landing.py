#!/usr/bin/env python

import cvxpy as cvx
import numpy as np
import ecos

x = cvx.Variable(4,integer=False)
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
