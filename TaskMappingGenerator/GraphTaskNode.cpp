/***************************************************************************************************
* File Name:             GraphTaskNode.h
*
* Application Developer: University of California Irvine
*
* The copyright information of this software can be found in the file COPYRIGHT.
*
**************************************************************************************************/

#include "GraphTaskNode.h"

GraphTaskNode::GraphTaskNode(int taskId = -1) 
{
	id = taskId;
	taskExecTime = -1;
	incomingEdgeList.clear();
	outgoingEdgeList.clear();
	mapping = -1;
	sched = -1;
}

GraphTaskNode::~GraphTaskNode() 
{
}

int GraphTaskNode::GetTaskId() 
{
	return id;
}

int GraphTaskNode::SetTaskId(int tid) 
{
	assert(tid >= 0);
	id = tid;
	return 0;
}

int GraphTaskNode::GetTaskExectTime() {

	return taskExecTime;
}

int GraphTaskNode::SetTaskExectTime(int execTime) {

	assert(execTime > 0);
	taskExecTime = execTime;
	return 0;
}

int GraphTaskNode::AddIncomingEdge(GraphEdge *edge) 
{
	incomingEdgeList.push_back(edge);
	return 0;
}

int GraphTaskNode::AddOutgoingEdge(GraphEdge *edge) 
{
	outgoingEdgeList.push_back(edge);
	return 0;
}

int GraphTaskNode::print() {

	cout << "    (" << GetTaskId() << ", " << GetTaskExectTime() << ")" << endl;
	return 0;
}

int GraphTaskNode::PrintAllEdges()
{

	cout << "*** Printing all the edges of task " << GetTaskId() << "..." << endl;

	cout << "  there are " << incomingEdgeList.size() << " incoming edges:" << endl;
	for (unsigned int i = 0; i < incomingEdgeList.size(); i++) {
		GraphEdge &e = *(incomingEdgeList.at(i));
		assert(id == e.GetDstTaskId());
		e.print();
	}

	cout << "  there are " << outgoingEdgeList.size() << " outgoing edges:" << endl;
	for (unsigned int i = 0; i < outgoingEdgeList.size(); i++) {
		GraphEdge &e = *(outgoingEdgeList.at(i));
		assert(id == e.GetSrcTaskId());
		e.print();
	}

	cout << "*** End of printing all the edges" << endl;
	return 0;
}

int GraphTaskNode::get_mapping() {

	return mapping;
}

int GraphTaskNode::set_mapping(int pid) {

	assert(pid >= 0);
	mapping = pid;
	return 0;
}

int GraphTaskNode::get_sched() {

	return sched;
}

int GraphTaskNode::set_sched(int s) {

	assert(s >= 0);
	sched = s;
	return 0;
}

int GraphTaskNode::GetIncomingEdgeNum() 
{
	return incomingEdgeList.size();
}

int GraphTaskNode::GetOutgoingEdgeNum() 
{
	return outgoingEdgeList.size();
}

GraphEdge* GraphTaskNode::GetIncomingEdge(int index) 
{
	return incomingEdgeList[index];
}

GraphEdge* GraphTaskNode::GetOutgoingEdge(int index) 
{
	return outgoingEdgeList[index];
}

int GraphTaskNode::GetIncomingEdgeId(int index) 
{
	return incomingEdgeList.at(index)->GetId();
}

int GraphTaskNode::GetOutgoingEdgeId(int index) 
{
	return outgoingEdgeList.at(index)->GetId();
}
