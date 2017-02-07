/***************************************************************************************************
* File Name:             GraphEdge.cpp
*
* Application Developer: University of California Irvine
*
* The copyright information of this software can be found in the file COPYRIGHT.
*
**************************************************************************************************/

#include "GraphEdge.h"
#include <cassert>
#include <iostream>
using namespace std;

GraphEdge::GraphEdge()
{
	id = -1;
	srcTaskId = -1;
	dstTaskId = -1;
	msgSize = -1;
}

GraphEdge::~GraphEdge()
{
}

int	GraphEdge::GetId()
{
	return id;
}

int	GraphEdge::SetId(int eid)
{
	assert (eid >= 0);
	id = eid;
	return 0;
}

int	GraphEdge::GetSrcTaskId()
{
	return srcTaskId;
}

int	GraphEdge::SetSrcTaskId(int tid)
{
	assert (tid >= 0);
	srcTaskId = tid;
	return 0;
}

int	GraphEdge::GetDstTaskId()
{
	return dstTaskId;
}

int	GraphEdge::SetDstTaskId(int tid)
{
	assert (tid >= 0);
	dstTaskId = tid;
	return 0;
}

int GraphEdge::GetMsgSize()
{
	return msgSize;
}

int	GraphEdge::SetMsgSize(int msg)
{
	assert (msg > 0);
	msgSize = msg;
	return 0;
}

int GraphEdge::print()
{
	cout << "    (" << GetId() << ", " << GetSrcTaskId() << ", " << GetDstTaskId() << ", " << GetMsgSize() << ")" << endl;
	return 0;
}
