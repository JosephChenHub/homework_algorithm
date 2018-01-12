#!/usr/bin/env python

from graph_base import Graph
import numpy as np
from collections import defaultdict

'''
push relabel algorithm is implemented in 
'graph_base.py'
see class Graph

'''
txt = open('problem2.data')
lines = txt.readlines()
txt.close()
for i in range(len(lines)):
	lines[i] = lines[i].strip()
del lines[0:3] #remove first 3 lines
matrices = []

cnt = 0
if not len(lines) %3 == 0:
	raise Exception('please check the input file!')
line_splits = [lines[i:i+3] for i in range(0,len(lines),3)]
for line in line_splits:

	#first MxN
	temp = line[0].split(' ')
	M = int(temp[0])
	N = int(temp[1])
	g = Graph()

	Rows = []
	Cols = []
	Rows_sum = []
	Cols_sum = []

	temp = line[1].split(' ') 
	for item in temp:
		Rows_sum.append(int(item))
	temp = line[2].split(' ')
	for item in temp:
		Cols_sum.append(int(item))
	
	ele_sums = sum(Rows_sum)
	matrix = np.int_(np.zeros([M,N]))
	#add edges from s to Ri, Ri to Ci
	for i in range(M):
		temp = 'R' + str(i)
		Rows.append(temp)
		g.addDEdgeCap('s', temp, Rows_sum[i]) 
		for k in range(N):
			c = 'C' + str(k)
			g.addDEdgeCap(temp, c, 1) 	
	#add edges from Ci to t
	for i in range(N):
		temp = 'C' + str(i)
		Cols.append(temp)
		g.addDEdgeCap(temp, 't', Cols_sum[i])
	
	print 'run max flow algorithm...'
	max_flow = g.push_relabel('s', 't')
#	max_flow = g.ford_fulkerson('s','t') #slower than push relabel!

	print 'max_flow:', max_flow
	#print 'residual graph:'
	#g.printGraph(g.graph_residual)

	if max_flow == ele_sums: #original - residual graph
		for i in range(M):
			item = g.graph_residual[Rows[i]]
			for k in range(N):
				temp = 1 - item[k][1]
				matrix[i][k] = temp
	print 'matrix:', matrix
					
