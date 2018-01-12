#!/usr/bin/env python

from collections import defaultdict 
from graph_base import Graph

#====== load balance ==============#
def load_balance(g, task_num, s, t, out_vertices ):
	left = 0
	right = task_num
	max_flow = g.ford_fulkerson(s,t)
	if max_flow != task_num:
		print 'error:there left jobs to distribute!'
		print 'please check the graph!'

	pre_load = task_num
	max_load = pre_load
	while left <= right:
		pre_load = max_load
		max_load = left + (right-left)/2
		for v in out_vertices:
			g.setDEdgeCap(v, t, max_load)
		#run ford fulkerson
		max_flow = g.ford_fulkerson('s', 't')
		if max_flow == task_num: #reduce the capacities of vertices to 't'
			right = max_load-1
		else:
			left = max_load+1		

		if left >= right and max_flow != task_num:
			max_load = pre_load

	return max_load
	

#read data
txt = open('problem1.data','r')
lines = txt.readlines()
txt.close()
del lines[0:3]
cnt = 0
graphs = defaultdict(list) #split the txt
for line in lines:
	if line != '\n':
		temp = line.split('\n')
		graphs[cnt].append(temp[0])
	else:
		cnt += 1
#construct graphs
for it in range(len(graphs)):
	item = graphs[it]
	g = Graph()
	temp = item[0].split(' ')
	task_num = int(temp[0])
	x = range(task_num)
	y = []
	ind = 0
	for com in item[1:]:
		temp = com.split(' ')
		y.append(temp[0])
		y.append(temp[1])
		g.addDEdgeCap(x[ind], temp[0], 1)
		g.addDEdgeCap(x[ind], temp[1], 1)
		ind += 1

	g.addVertex('t')			
	for i in x:
		g.addDEdgeCap('s', i, 1)
	for i in y:
		g.addDEdgeCap(i, 't', task_num)

#	print 'graph:'
#	g.printLists()
	max_load = load_balance(g, task_num, 's', 't', y)
	print 'the number of jobs:', task_num
	print 'minimum max_load:', max_load
