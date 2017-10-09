#!/usr/bin/env python

import numpy as np

def query(X,k):
	temp = np.sort(X)
	return temp[k-1]

def median(A,i,B,j,k):
	if k == 1:
		return min(query(A,i+1),query(B,j+1))
	if i ==0:
		i = k/2
		j = k -i	

#	print 'debug: k:'+ str(k) + ' i:' + str(i) + ' j:' + str(j)
	if query(A,i) < query(B,j):
		k = k-k/2   #discard k/2
		i += k/2
		j -= k/2    #
		if k==1:
			j -= 1	
		return median(A,i,B,j,k)
	elif query(A,i) > query(B,j):
		k = k-k/2   #discard j'=k-k/2,so k=k-j'=k/2
		i -= k/2
		j += k/2
		if k == 1:
			i -= 1
		return median(A,i,B,j,k)
	else:
		return query(A,i)			


#--check correctness --
cnt = 0
print 'test:',len(range(10,1000))
for i in range(10,1000):
	a = np.arange(1,4096,2)
	b = a + i
	c = np.sort(np.append(a,b))

	k = i	
	x1 = median(a,0,b,0,k)
	x2 = c[k-1]
	if x1 != x2:
		print 'error'
	else:
		cnt += 1
print 'ok:',cnt

