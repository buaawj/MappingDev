from gurobipy import *

# Vertices and associated supply
vertices = {0: 4, 1: 3, 2: 2, 3: 0, 4: -6, 5: -3}

# Dict key: edge, value: (capacity, cost per flow, fixed cost)
edges = {(0,4): (4,1,1),
         (0,3): (2,1,1),
         (1,3): (3,1,1),
         (2,5): (2,1,1),
         (3,4): (2,1,1),
         (3,5): (1,1,1)}

m = Model()

x = {} # Flow on each edge
y = {} # Binary variable for each edge

# key: vertex, value: list of edges entering/leaving the vertex
edgeIn   = { v:[] for v in vertices }
edgeOut  = { v:[] for v in vertices }

print "*****************"
print edgeIn
print "*****************"

# Add variables
for edge in edges:
  u = edge[0]
  v = edge[1]
  y[edge] = m.addVar(vtype=GRB.BINARY, name="y" + str(edge))
  x[edge] = m.addVar(lb=0, vtype=GRB.CONTINUOUS, name="x" + str(edge) )
  edgeIn[v] = edgeIn[v] + [x[edge]]
  edgeOut[u] = edgeOut[u] + [x[edge]]



m.update()

print "*****************"
print edgeIn
print "*****************"

# Add constraints
for v in vertices:
  m.addConstr(quicksum(edgeOut[v]) - quicksum(edgeIn[v]) == vertices[v], name="v%d" % v)

for edge in edges:
  m.addConstr(x[edge] <= edges[edge][0]*y[edge], name=str(edge))

# Set objective
m.setObjective(quicksum((edges[edge][1]*x[edge] + edges[edge][2]*y[edge]) for edge in edges))

m.optimize()