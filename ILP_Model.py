from gurobipy import *

# Vertices and NoC size
NumOfVertices = 5
N = 3

# Dict key: edge, value: (communication volume)
#edges = {(1,3): 5,
#         (2,3): 60,
#         (2,4): 70,
#         (2,5): 200,
#         (3,6): 100,
#         (4,6): 100,
#         (4,7): 250,
#         (5,7): 250,
#         (5,8): 50}

edges = {(1,2): 25,
         (1,3): 10,
         (1,4): 15,
         (2,5): 15,
         (3,5): 10,
         (4,5): 15}


# ========================================================================
# ========================  1 Create model.
m = Model()

# ========================================================================
# ======================== 2 Add vars.
a = {} # Binary variable to indicate if Task i is mappined on Tile(x, y)
for i in range(NumOfVertices):
  for x in range(N):
    for y in range(N):
      a[i,x,y] = m.addVar( vtype=GRB.BINARY, name="a[%s %s %s]" %(i,x,y) )

X = {} # Binary variable to indicate if the distance in x dimension between task i and j is equal to d.
Y = {} # Binary variable to indicate if the distance in y dimension between task i and j is equal to d.
for i in range(NumOfVertices):
  for j in range(NumOfVertices):
    for d in range(N):
      X[i,j,d] = m.addVar( vtype=GRB.BINARY, name="x[%s %s %s]" %(i,j,d) )
      Y[i,j,d] = m.addVar( vtype=GRB.BINARY, name="y[%s %s %s]" %(i,j,d) )

m.update()

# =============================================== 3 Add constraints: ================================================
# Constraint 1:every task i must be mapped to a router with the coordinate (x,y) and only one task can be mapped to a single router
for v in range(NumOfVertices):
  m.addConstr(quicksum( a[v,i,j] for i in range(N) for j in range(N)) == 1, name="constr1" )


# Constraint 2: The number of tasks in the WCTG may be less than the number available routers.
for x in range(N):
  for y in range(N):
    m.addConstr(quicksum( a[i,x, y] for i in range(NumOfVertices) ) <= 1, name="constr2" )

# Constraint 3: The number of tasks in the WCTG may be less than the number available routers.
for d in range(N):
  for edge in edges:
    for x1 in range(N):
      for y1 in range(N):
        for x2 in range(N):
          for y2 in range(N):
            i = edge[0]-1
            j = edge[1]-1
            if abs(x1-x2)==d:
              m.addConstr( X[i,j,d] >= a[i, x1, y1] + a[j, x2, y2] - 1, name="constr3" )

# Constraint 4: The number of tasks in the WCTG may be less than the number available routers.
for d in range(N):
  for edge in edges:
    for x1 in range(N):
      for y1 in range(N):
        for x2 in range(N):
          for y2 in range(N):
            i = edge[0]-1
            j = edge[1]-1
            if abs(y1-y2)==d:
              m.addConstr( Y[i,j,d] >= a[i, x1, y1] + a[j, x2, y2] - 1, name="constr3" )

# =============================================== 4. set objective: ================================================
objX = quicksum( edges[edge]*d*X[edge[0]-1, edge[1]-1, d] for edge in edges for d in range(N)  );
objY = quicksum( edges[edge]*d*Y[edge[0]-1, edge[1]-1, d] for edge in edges for d in range(N)  );

m.setObjective( objX+objY, GRB.MINIMIZE )

m.update()
m.write("test.lp");
# ---- Compute optimal solution
m.optimize()
for v in m.getVars():
  print v.varName, v.x
