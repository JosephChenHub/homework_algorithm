#!/usr/bin/env python

from collections import defaultdict 
import copy

from ipdb import set_trace

#a flag to test Graph
GRAPH_TEST = False

class Graph:

	def __init__(self):
		self.graph = defaultdict(list) #adjacent lists
		self.graph_backup = defaultdict(list)
		self.graph_residual = defaultdict(list)
		self.edges = 0 	#number of edges
		self.vertices = [] #vertices
		self.path = []	#store the s-t path

	#print the ajacency lists
	def printLists(self):
		print 'Graph adjacent lists:'
		for item in self.graph:
			print 'vertex:', item
			print self.graph[item]
	def printGraph(self, g):
		print 'Graph adjacent lists:'
		for item in g:
			print 'vertex:', item
			print g[item]
		
	#add a directed edge
	def addDEdge(self, u, v):
		if not u in self.vertices:
			self.graph[u] = [] #create a vertex
			self.vertices.append(u)
		if not v in self.vertices:
			self.graph[v] = []
			self.vertices.append(v)
		if not v in self.graph[u]:
			self.graph[u].append(v)
			self.edges += 1
	#add a directed edge with capacity
	def addDEdgeCap(self, u, v, capacity):
		if not u in self.vertices:
			self.graph[u] = []
			self.vertices.append(u)
		if not v in self.vertices:
			self.graph[v] = []
			self.vertices.append(v)
		for item in self.graph[u]:
			if item[0] == v: #exist
				item[1] = capacity
				return
		self.graph[u].append([v,capacity])
		self.edges += 1

	#add an isolated vertex
	def addVertex(self, u):
		if not u in self.vertices:
			self.graph[u] = []
			self.vertices.append(u)
	#change the edge's capacity
	def setDEdgeCap(self, u, v, capacity):
		flag = False
		for item in self.graph[u]:
			if item[0] == v:
				item[1] = capacity
				flag = True
		if flag == False:
			print 'error: the edge  u --> v does not exist!please use \'addDEdgeCap\'!'

	#Breadth-first search
	def BFS(self, s, t,path):
		if not s in self.vertices or not t in self.vertices:
			raise Exception('check the input vertices!')
		source = s
		visited = [False] *(len(self.graph))
		pred = [-1]*(len(self.graph)) #pred[w]=v means that w was visited after v
		queue = []
		queue.append(s)
		visited[self.vertices.index(s)] = True

		while queue:
			s = queue.pop(0) #dequeue a vertex from the queue
			
			for item in self.graph[s]:
				if len(item) == 2: 
					ind = item[0]
					val = item[1]
				else:
					ind = item
					val = 1
				if visited[self.vertices.index(ind)] == False and val > 0: #not visited and capacity > 0
					queue.append(ind)
					temp = self.vertices.index(ind)
					visited[temp] = True
					pred[self.vertices.index(ind)] = s
			
		if visited[self.vertices.index(t)] == False: #vertex t is not reachable
			return False
		else: #reachable
			self.path = [] #store the shortest s-t path
			w = self.vertices.index(t)
			while(w != self.vertices.index(source)):
				self.path.insert(0, pred[w])			
				w = self.vertices.index(pred[w])
			self.path.append(t)

			return True

	def ford_fulkerson(self,s, t):	
		if not s in self.vertices or not t in self.vertices:
			raise Exception('check the input vertices!')
		self.graph_backup = copy.deepcopy(self.graph)
		max_flow = 0  #max_flow 
		while self.BFS(s,t, self.path) != False: #find a shortest s-t path in Gf using BFS tree
			if GRAPH_TEST == True:
				print 'a s-t path:', self.path	
			#calculate the flow of the path
			flow = float('Inf')
			for i in range(len(self.path)-1):
				items = self.graph[self.path[i]] #ajacent list of vertex path[i]
				for item in items:
					if self.path[i+1] == item[0]:
						flow = min(flow, item[1])

			max_flow += flow  #augment
			if GRAPH_TEST == True:
				print 'flow',flow				
			#construct the residual graph Gf
			for i in range(len(self.path)-1):
				items = self.graph[self.path[i]] 
				for item in items:
					if self.path[i+1] == item[0]:
						item[1] -= flow 
						#add a reverse edge with capacity flow
						self.addDEdgeCap(self.path[i+1], self.path[i],flow)

		if GRAPH_TEST == True:
			print 'max_flow:', max_flow		

		self.graph_residual = copy.deepcopy(self.graph)
		self.graph = copy.deepcopy(self.graph_backup) #recover original graph
		return max_flow

	def push_relabel(self, s, t):
		if not s in self.vertices or not t in self.vertices:
			raise Exception('check the input vertices!')
		self.graph_backup = copy.deepcopy(self.graph)

		num = len(self.vertices)
		height = dict()
		for v in self.vertices:
			height[v] = 0
		height[s] = num

		excess = dict()
		for item in self.graph[s]:
			self.addDEdgeCap(item[0], s, item[1]) 
			excess[item[0]] = item[1]
			item[1] = 0

		max_flow = 0
		while excess:
			for vertex in excess.keys():
				if excess[vertex] == 0:
					excess.pop(vertex)
					continue
				push_flag = False
				for neighbour in self.graph[vertex]:
					if height[vertex] > height[neighbour[0]] and neighbour[1] > 0:
						#push excess from vertex to neighbour
						flow = min(excess[vertex], neighbour[1])    			
						excess[vertex] -= flow
						neighbour[1] -= flow
						self.addDEdgeCap(neighbour[0], vertex, flow)
						if neighbour[0] != s and neighbour[0] != t: #except s, t
							if neighbour[0] in excess:
								excess[neighbour[0]] += flow
							else:
								excess[neighbour[0]] = flow

						if neighbour[0] == t:
							max_flow += flow
						if GRAPH_TEST == True:
							print 'push flow:' + str(flow) + ' from ' + str(vertex) + ' --> ' + str(neighbour[0]) \
							+ ' height: ' + str(height) + ' excess:' + str(excess)
						push_flag = True
				if push_flag == False: #raise the height of vertex
					height[vertex] += 1 
				if excess[vertex] == 0:
					excess.pop(vertex)


		self.graph_residual = copy.deepcopy(self.graph)
		self.graph = copy.deepcopy(self.graph_backup)
		return max_flow




#a simple test for the class Graph
#you can use integers or characters to stand for vertices
#both will be OK
if __name__ == '__main__':
	g = Graph()
	g.addDEdgeCap('s','s1',10)
	g.addDEdgeCap('s','s3',10)
	g.addDEdgeCap('s1','s3',2)
	g.addDEdgeCap('s1', 's4',8)
	g.addDEdgeCap('s2','t',10)
	g.addDEdgeCap('s4','t',10)
	g.addDEdgeCap('s4','s2',6)
	g.addDEdgeCap('s1','s2', 4)
	g.addDEdgeCap('s3','s4',9)
	#print
	g.printLists()
	#ford-fulkerson
	print 'Run a test for Ford Fulkerson algorithm...'
	g.ford_fulkerson('s','t')
	
	#a new Graph
	print '\nHere is a same graph...'
	g = Graph()
	g.addDEdgeCap(0,1,10)
	g.addDEdgeCap(4,2,6)
	g.addDEdgeCap(0,3,10)
	g.addDEdgeCap(2,5,10)
	g.addDEdgeCap(1,3,2)
	g.addDEdgeCap(4,5,10)
	g.addDEdgeCap(1,2,4)
	g.addDEdgeCap(3,4,9)
	g.addDEdgeCap(1,4,8)
	g.printLists()
	g.ford_fulkerson(0,5)
	#run test for push relabel
	print '\nRunning push relabel algorithm...'
	print 'original graph:'
	g.printLists()
	max_flow = g.push_relabel(0, 5)
	print 'max_flow:', max_flow

