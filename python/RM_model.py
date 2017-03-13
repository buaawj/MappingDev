from gurobipy import *
import random
# ========================================================================================== 
  
# ==========================================================================================
# ----- 'MinRS': Minimize-rule-size;
# ----- 'MaxTP': Maximize-throughput.

# ==========================================================================================
Sense = 'MinRS'
# ==========================================================================================


# ================= Random-parameters settings :
# ========================== bandwidth of links, x Mb/s.
CPA_Link_UB = 150
CPA_Link_LB = 90

# ========================== Cu in paper,  the number of rules can be placed in switch.
CPA_SW_UB = 2000
CPA_SW_LB = 1500

# ========================== Number of rules subsets for each session.
CNT_RULE_SUBSET_UB = 20
CNT_RULE_SUBSET_LB = 20
# ========================== Ci in paper,  the size of rules subset i.
CNT_Ci_UB = 100
CNT_Ci_LB = 50

# ================= Random-parameters settings :~



# ========================== Given the list Traffic Demand
CNT_Tk = 5;  	    # number of Traffic Demand
## --- in each item in the list_Tk: [Src, Dst, traffic-rate]
list_Tk = [
[0,15,120],
[1,4,100],
[2,11,90],
[8,16,100],
[9,19,80]
]

# ==================== Number of candidate paths of each Traffic Tk.
Lk = [ 4, 5, 5, 5, 5 ]
# ==================== Given the candidate paths of each Traffic Tk.
List_passed_SWi_of_Pkl = [
	[
		[0,4,10,13,17,15],
		[0,8,9,11,14,15],
		[0,7,8,12,14,16,15],
		[0,8,9,11,10,13,17,15]
	],
	[
		[1,0,4],
		[1,2,3,4],
		[1,5,6,0,8,9,4],
		[1,5,6,7,8,0,4],
		[1,2,0,8,9,4]
	],
	[
		[2,0,4,11],
		[2,10,4,11],
		[2,1,5,6,7,8,9,11],
		[2,3,4,10,11],
		[2,3,4,0,8,9,12,14,13,11]
	],
	[
		[8,9,4,10,13,14,16],
		[8,12,14,16],
		[8,9,11,14,16],
		[8,9,11,10,13,17,15,16],
		[8,9,11,14,13,17,18,19,16]
	],
	[
		[9,4,10,13,17,18,19],
		[9,4,10,13,14,16,19],
		[9,11,14,16,19],
		[9,11,13,17,15,16,19],
		[9,8,12,14,16,19]
	]
]
List_candidate_P_kl = []
for k in range(CNT_Tk):
	list_Path_k = []
	list_passed_SWi_of_Pk = List_passed_SWi_of_Pkl[k]
	for j in range( len(list_passed_SWi_of_Pk) ):
		list_Path_j = []
		list_SWs = list_passed_SWi_of_Pk[j]
		cnt = 0
		while cnt < len( list_SWs )-1:
			ln = list_SWs[cnt]
			rn = list_SWs[cnt+1]
			tuple = ln, rn
			list_Path_j.append(tuple)
			cnt = cnt+1
		list_Path_k.append( list_Path_j )
	List_candidate_P_kl.append(list_Path_k)	
print 'List_candidate_P_kl = ', List_candidate_P_kl



# ========================== Given the constants to calculate the Rules Space:
# ==== the original rules space size ||E_k|| of session k.
#      i.e., Origin_EndPolicy_size_Ek = [ RDM_SIZE_RULE for k in range(CNT_Tk) ]
list_CNT_RuleSubsets_Tk = [ 0 for k in range(CNT_Tk) ]
for k in range(CNT_Tk):
	list_CNT_RuleSubsets_Tk[k] = random.randint(CNT_RULE_SUBSET_LB, CNT_RULE_SUBSET_UB)
# ====
Ik = [ [] for k in range(CNT_Tk) ]
ruleID = 0
for k in range(CNT_Tk):
	for i in range(list_CNT_RuleSubsets_Tk[k]):
		Ik[k].append( ruleID )
		ruleID = ruleID + 1
print 'Ik:\n', Ik

# ========================== Given the hardware  ( ITALYNET(21 nodes, 35 links) )
N = 20;  		# number of Switches
# ==== Generate the rules-size capacity of switches (fixed value or random value).
Cu = [0 for u in range(N)]
# Cu = [ CPA_SW, CPA_SW, CPA_SW, CPA_SW, CPA_SW, CPA_SW, CPA_SW, CPA_SW, CPA_SW, CPA_SW,
# 		 CPA_SW, CPA_SW, CPA_SW, CPA_SW, CPA_SW, CPA_SW, CPA_SW, CPA_SW, CPA_SW, CPA_SW ]
for u in range(N):
	Cu[u] = random.randint(CPA_SW_LB, CPA_SW_UB)
# ==== The number of rules in subset i.
Ci = {}
for k in range(CNT_Tk):
	for i in Ik[k]:
		Ci[k,i] = random.randint(CNT_Ci_LB, CNT_Ci_UB)
	
	
# ==== Given constant matrix of M_E_UV ( It represents the topology of ITALYNET(20 nodes, 30 links) ).
M_E_UV = [
[0, 1,0,0,1,0, 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0],
[1, 0,1,0,0,1, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0],
[0, 1,0,1,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0],
[0, 0,1,0,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0],
[1, 0,0,1,0,0, 0,0,0,1,1, 0,0,0,0,0, 0,0,0,0],
[0, 1,0,0,0,0, 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0],
[1, 0,0,0,0,1, 0,1,0,0,0, 0,0,0,0,0, 0,0,0,0],
[0, 0,0,0,0,0, 1,0,1,0,0, 0,0,0,0,0, 0,0,0,0],
[0, 0,0,0,0,0, 0,1,0,1,0, 0,1,0,0,0, 0,0,0,0],
[0, 0,0,0,1,0, 0,0,1,0,0, 1,0,0,0,0, 0,0,0,0],
[0, 0,0,0,1,0, 0,0,0,0,0, 1,0,1,0,0, 0,0,0,0],
[0, 0,0,0,0,0, 0,0,0,1,1, 0,0,0,1,0, 0,0,0,0],
[0, 0,0,0,0,0, 0,0,1,0,0, 0,0,0,1,0, 0,0,0,0],
[0, 0,0,0,0,0, 0,0,0,0,1, 0,0,0,1,0, 0,1,0,0],
[0, 0,0,0,0,0, 0,0,0,0,0, 1,1,1,0,0, 1,0,0,0],
[0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,1,0,0],
[0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,1,1, 0,0,0,1],
[0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,1, 0,0,1,0],
[0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,1,0,1],
[0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,0,1,0]
]

# ==== Given constants matrix of B_uv, the bandwidth capacity of each link.
B_uv = [[0 for u in range(N)] for l in range(N)]
for u in range(N):
	for v in range(N):
		if M_E_UV[u][v] > 0:
			B_uv[u][v] = random.randint(CPA_Link_LB, CPA_Link_UB)

# ############################################### Prepare work #########################################################

# ==================== Create a dict to record all valid links_arcs.
dValid_links = {}
for u in range(N):
	for v in range(N):
		if M_E_UV[u][v] > 0:
			dValid_links[u,v] = M_E_UV[u][v]

# ==================== Create a dict to record the capacity of all links.
dB_uv = {}
for u,v in dValid_links:
	dB_uv[u,v] = B_uv[u][v]
# ---- get the max-cap of all links
Sorted_cap_links = sorted( dB_uv.values(), reverse = True )
Max_cap_links = Sorted_cap_links[0]
print '\n ==== Max_cap_links = %d\n\n'%Max_cap_links

# ========== Get the min-cap link of each candidate path p^kl.
dMinCap_of_path_Pkl = {}
for k in range(CNT_Tk):
	for j in range( Lk[k] ):
		listCaps_Pkl = []
		for u,v in dValid_links:
			if (u,v) in List_candidate_P_kl[k][j]:
				listCaps_Pkl.append( B_uv[u][v] )
		minCap_Pkl = min(listCaps_Pkl)
		dMinCap_of_path_Pkl[k,j] = minCap_Pkl
		
print 'dMinCap_of_path_Pkl:',dMinCap_of_path_Pkl

# ========================================================================
# ========================  1 Create model.
model = Model('xxx')
model.setParam( 'OutputFlag', True )

# ========================================================================
# ======================== 2 Add vars.
# --- A. lamda^kl_(u,v)
dVar_lamda_kl_uv = {}
for k in range(CNT_Tk):
	for l in range( Lk[k] ):
		for u,v in dValid_links:
			dVar_lamda_kl_uv[k,l,u,v] = model.addVar( vtype=GRB.BINARY, name='dVar_lamda_kl_uv[%s %s %s %s]'%(k,l,u,v) )
model.update()

# --- B.1 Rate^kl --- The rate on the l^th path belonging to Tk.
dVar_R_kl = {}
for k in range(CNT_Tk):
	for l in range( Lk[k] ):
		dVar_R_kl[k,l] = model.addVar( vtype=GRB.CONTINUOUS, lb=0.0, name='dVar_R_kl[%s %s]'%(k,l) )
model.update()
# --- B.2 Rate^kl_uv --- The rate on the link (u,v) which belongs to j^th path coressponding to Tk.
dVar_R_kl_uv = {}
for k in range(CNT_Tk):
	for l in range( Lk[k] ):
		for u,v in dValid_links:
			dVar_R_kl_uv[k,l,u,v] = model.addVar( vtype=GRB.CONTINUOUS, lb=0.0, name='dVar_R_kl_uv[%s %s %s %s]'%(k,l,u,v) )
model.update()

# --- C.1 x^kl_iu: if the rule set i is placed on node u, corresponding to path Path^kl.
dVar_x_kl_iu = {}
for k in range(CNT_Tk):
	for l in range( Lk[k] ):
		for i in Ik[k]:
			for u in range(N):
				dVar_x_kl_iu[k,l,i,u] = model.addVar( vtype=GRB.BINARY, lb=0, ub=1, name='dVar_x_kl_iu[%s %s %s %s]'%(k,l,i,u) )
model.update()
# --- C.2 x_kiu: if the rule set i from Ik[k] is placed on node u.
dVar_x_kiu = {}
for k in range(CNT_Tk):
	for i in Ik[k]:
		for u in range(N):
			dVar_x_kiu[k,i,u] = model.addVar( vtype=GRB.BINARY, lb=0, ub=1, name='dVar_x_kiu[%s %s %s]'%(k,i,u) )
model.update()


# --- D. y^kl: the binary state indicator variable which denotes the state of selecting of path P^kl for T_k.
dVar_y_kl = {}
for k in range(CNT_Tk):
	for j in range( Lk[k] ):
		dVar_y_kl[k,j] = model.addVar( vtype=GRB.BINARY, name='dVar_y_k[%s]_j[%s]'%(k,j) )
model.update()

# =============================================== 3 Add constraints: ================================================


# ======================== # ======================== A. Constraints on paths # ======================== #
# --- Constraint (2) : if a path l is selected, each rule set i should be deployed on at least one node along this path.
for k in range(CNT_Tk):
	for l in range( Lk[k] ):
		for i in Ik[k]:
			sumRule_amt_in_path_kl = quicksum( dVar_x_kl_iu[k,l,i,u0] for u0 in List_passed_SWi_of_Pkl[k][l] )
			model.addConstr( sumRule_amt_in_path_kl >= dVar_y_kl[k,l] )
model.update()

# ---- Constraint (3) : Relationship between   dVar_x_kiu   and   dVar_x_kl_iu.
for k in range(CNT_Tk):
	for l in range( Lk[k] ):
		for i in Ik[k]:
			for u in List_passed_SWi_of_Pkl[k][l]:
				model.addConstr( dVar_x_kiu[k,i,u] >= dVar_x_kl_iu[k,l,i,u] )
model.update()


# o --- Constraint(4): to each switch, it has its rules capacity  Cu.
for u in range(N):
	sumRule_amt_in_SW_u = quicksum( dVar_x_kiu[k0,i0,u0]*Ci[k0,i0] for k0,i0,u0 in dVar_x_kiu if u0==u )
	model.addQConstr( sumRule_amt_in_SW_u <= Cu[u] )
model.update()

# ======================== # ======================== B. Constraints on Rate # ======================== #

# o --- Constraint _1:(5) The traffic rate shall be no-less than the demand of T_k.
for k in range(CNT_Tk):
	Dk =  list_Tk[k][2]
	total_rate_Tk = quicksum( dVar_R_kl[kx,j] for kx,j in dVar_R_kl if kx==k )
	model.addConstr( total_rate_Tk >= Dk )
model.update()

# o --- Constraint _2:(6) The traffic rate of the path shall be no greater than the minimum one on links.
for k in range(CNT_Tk):
	for l in range( Lk[k] ):
		for u,v in dValid_links:
			if (u,v) in List_candidate_P_kl[k][l]:
				model.addConstr( dVar_R_kl[k,l] <= dVar_R_kl_uv[k,l,u,v] )
model.update()

# o --- Constraint _3:(7) When multi-paths share a common link (x,y), total rate on P^kl <= cap_link(x,y).
for u,v in dValid_links:
	sumRate_on_uv = quicksum( dVar_R_kl_uv[k,l,u0,v0] for k,l,u0,v0 in dVar_R_kl_uv if u0==u and v0==v )
	model.addQConstr( sumRate_on_uv <= dB_uv[u,v] )
model.update()




# ======================== # ======================== C. Constraints on Rules # ======================== #

# --- Constraint (8) :  dVar_R_kl_uv, dVar_y_kl, dB_uv
for k in range(CNT_Tk):
	for l in range( Lk[k] ):
		for u,v in dValid_links:
			model.addConstr( dVar_R_kl_uv[k,l,u,v] <= dVar_y_kl[k,l]*dB_uv[u,v] )
model.update()

# ======================== # ======================== End of all Constraints :~


# ======================== 5. set objective
#    MINIMIZE MAXIMIZE
# total_rules_occupation = quicksum( dVar_x_kiu[k,i,u] for k,i,u in dVar_x_kiu )### This is the original obj-function in the IWQoS14 paper. We can see that the Ci[k,i] was lost by mistake, WTF.
total_rules_occupation = quicksum( dVar_x_kiu[k,i,u]*Ci[k,i] for k,i,u in dVar_x_kiu )### The right obj-function.
model.setObjective( total_rules_occupation, GRB.MINIMIZE )


# ---- Compute optimal solution
model.optimize()

# ======================== 6 Print solution
if model.status == GRB.status.OPTIMAL:
	
	print '\n------------ 1, dVar_y_kl ---------------------\n'
	dRet__Rout = {}
	for k,l in dVar_y_kl:
		dRet__Rout[k,l] = dVar_y_kl[k,l].getAttr("x")			
	for k in range(CNT_Tk):
		for l in range( Lk[k] ):
			if dRet__Rout[k,l] > 0.0001 :
				print 'y^kl_ kl[%d %d] --- %d '%(k,l,dRet__Rout[k,l])
		# print '\n'
	print '\n'
		
			
	print '\n------------ 2, dVar_x^kiu ---------------------\n'
	for k in range(CNT_Tk):		
		for i in Ik[k]:
			for u in range(N):
				if dVar_x_kiu[k,i,u].getAttr("x") >= 0.0001 :
					print 'dVar_x_kiu[%d %d %d] --- ( %d )'%(k,i,u,dVar_x_kiu[k,i,u].getAttr("x"))
					
	# print '\n------------ 3, dVar_x^kl_iu ---------------------\n'
	# dRet__RuleAllocation = {}
	# for k,l,i,u in dVar_x_kl_iu:
		# dRet__RuleAllocation[k,l,i,u] = dVar_x_kl_iu[k,l,i,u].getAttr("x")	
	# for k in range(CNT_Tk):
		# for l in range( Lk[k] ):	
			# for i in Ik[k]:		
				# for u in range(N):
					# if dRet__RuleAllocation[k,l,i,u] >= 0.0001 :
						# print 'dVar_x_kl_iu[%d %d %d %d] --- ( %d )'%(k,l,i,u,dRet__RuleAllocation[k,l,i,u])

	print '\n------------ 4, dVar_Rate^kl ---------------------\n'
	dRet__Rk = {}
	for k,l in dVar_R_kl:
		dRet__Rk[k,l] = dVar_R_kl[k,l].getAttr("x")			
	for k,l in dRet__Rk:
		if dRet__Rk[k,l] > 0.01 :
			print 'Rate^kl__kl[%d %d] --- ( %f )'%(k,l,dRet__Rk[k,l])
			

			
	print '\n------------- _OPT_val --------------------'
	print '_OPT_Rule-Space ==',model.ObjVal
		

else:
	print '\n---------------------------------------------- hello, Failed !!\n',
	model.computeIIS()
	model.write("model_rec.ilp")
	print '\n------- Did not Optimized. ----------\n'
#======================================================================================================================	