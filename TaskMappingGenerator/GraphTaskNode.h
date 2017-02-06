/***************************************************************************************************
 * File Name:             GraphTaskNode.h
 *
 * Application Developer: University of California Irvine
 *
 * The copyright information of this software can be found in the file COPYRIGHT.
 *
 **************************************************************************************************/

#ifndef GRAPH_TASK_NODE
#define GRAPH_TASK_NODE

#include "GraphEdge.h"

class GraphTaskNode
{
public:
	GraphTaskNode();
	~GraphTaskNode();

	int					GetTaskId();
	int					SetTaskId(int tid);
	int					GetTaskExectTime();
	int					SetTaskExectTime(int t);

	int					AddIncomingEdge(GraphEdge *edge);
	int					AddOutgoingEdge(GraphEdge *edge);
	int					print();
	int					PrintAllEdges();

	int					get_mapping();
	int					set_mapping(int pid);
	int					get_sched();
	int					set_sched(int s);

	int 				GetIncomingEdgeNum();
	int 				GetOutgoingEdgeNum();
	GraphEdge*			GetIncomingEdge(int index);
	GraphEdge*			GetOutgoingEdge(int index);
	int					GetIncomingEdgeId(int index);
	int					GetOutgoingEdgeId(int index);

private:

	int                 id;                         
	int					taskExecTime;			    
	vector<GraphEdge*>	incomingEdgeList;           
	vector<GraphEdge*>	outgoingEdgeList;     

	int                 mapping;                    // the id of the PU the task is assigned
	int                 sched;                   	// the sequence number the task is assigned to execute on the PU
};


#endif // GRAPH_NODE
