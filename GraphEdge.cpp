/***************************************************************************************************
* File Name:             GraphEdge.cpp
*
* Application Developer: University of California Irvine
*
* The copyright information of this software can be found in the file COPYRIGHT.
*
**************************************************************************************************/

#include "GraphEdge.h"

GraphEdge::GraphEdge() 
{
	id = -1;
	srcTaskId = -1;
	DstTaskId = -1;
	msgSize = -1;
}

GraphEdge::~GraphEdge() 
{
}

inline int	GraphEdge::GetId() 
{
	return id;
}

inline int	GraphEdge::SetId(int eid) 
{
	assert (eid >= 0);
	id = eid;
	return 0;
}

inline int	GraphEdge::GetSrcTaskId() 
{
	return srcTaskId;
}

int	GraphEdge::SetSrcTaskId(int tid) 
{
	assert (tid >= 0);
	srcTaskId = tid;
	return 0;
}

int	GraphEdge::get_dst_task_id() {
	
	return dst_task_id;
}

int	GraphEdge::set_dst_task_id(int tid) {
	
	assert (tid >= 0);
	dst_task_id = tid;
	return 0;
}

double GraphEdge::get_msg_size() {
		
	return msg_size;
}

int	GraphEdge::set_msg_size(double s) {

	assert (s > 0);
	msg_size = s;
	return 0;
}

int GraphEdge::print() {

	cout << "    (" << get_id() << ", " << get_src_task_id() << ", " << get_dst_task_id() << ", " << get_msg_size() << ")" << endl;
	return 0;
}

int	GraphEdge::get_buff_size() {
		
	return buff_size;
}

int	GraphEdge::set_buff_size(int s) {

	assert (s > 0);
	buff_size = s;
	return 0;
}


