/***************************************************************************************************
* File Name:             AppModel.cpp
*
* Application Developer: University of California Irvine
*
* The copyright information of this software can be found in the file COPYRIGHT.
*
**************************************************************************************************/

#include "appModel.h"

extern string end_str;
extern char temp_str[];

AppModel::AppModel()
{
	appName[0] = '\0';
	taskList.clear();
	edgeList.clear();

	starting_task_list.clear();
	finishing_task_list.clear();
}

AppModel::~AppModel()
{
}


void AppModel::PrintMappingResult() 
{
	cout << "*** The mapping and scheduling result in (task id, mapped proc id is:" << endl;
	for (unsigned int i = 0; i < taskList.size(); i++) 
	{
		GraphTaskNode &t = taskList.at(i);
		cout << "    (" << t.GetTaskId() << ", " << t.get_mapping() << ", " << ")" << endl;
	}
	cout << endl;

	return 0;
}

int	AppModel::load_buffer_alloc_result(char *file) {

}

GraphTaskNode& AppModel::GetTask(int tid) 
{
	return taskList.at(tid);
}

GraphEdge& AppModel::GetEdge(int eid) 
{
	return edgeList.at(eid);
}

int AppModel::BuildAppModel(map<pair<int, int>, int> &edges, int numTasks)
{
	// initialize the lists
	taskList.resize(numTasks+1);
	for(int i=0; i<=numTasks; ++i)
	{
		taskList[i] = GraphTaskNode(i);
	}
	edgeList.resize(edges.size());

	int edgeId = 0;
	for(map<pair<int, int>, int>::iterator it = edges.begin(); it<edge.end(); ++it)
	{
		srcTaskId = it->first.first;
		destTaskId = it->first.second;
		commVol = it->second;

		// construct the edge
		GraphEdge edge;
		edge.SetId(edgeId);
		edge.SetSrcTaskId(srcTaskId);
		edge.SetDstTaskId(destTaskId);
		edge.SetMsgSize(commVol);

		// add the edge to the list
		edgeList[edgeId] = edge;
		GraphTaskNode &srcTask = taskList[srcTaskId];
		GraphTaskNode &dstTask = taskList[destTaskId];
		srcTask.AddOutgoingEdge(&edgeList[edgeId]);
		dstTask.AddIncomingEdge(&edgeList[edgeId]);

		++edgeId;
	}

	// verify if the task list is consistent: task/edge id equal to its entry index
	verify_consistency();

	if (GLOBAL_DEBUG_OUT) 
	{
		print();
	}
}


int AppModel::GetDistance(int pe1, int pe2)
{
	
}


int AppModel::verify_consistency() {

	for (unsigned int i = 0; i < taskList.size(); i++) {
		GraphTaskNode &t = taskList.at(i);
		if (i != t.GetTaskId()) {
			std::cout << "Exppected id=" << i << " Receveid=" << t.GetTaskId() << endl;
		}
		assert(i == t.GetTaskId());
	}
	for (unsigned int i = 0; i < edgeList.size(); i++) {
		GraphEdge &e = edgeList.at(i);
		if (i != e.GetId()) {
			std::cout << "Exppected id=" << i << " Receveid=" << e.GetId() << endl;
		}
		assert(i == e.GetId());
	}
	return 0;
}

int AppModel::print() {

	cout << "*** Printing the application model \"" << appName << "\" with " << taskList.size()
		<< " tasks and " << edgeList.size() << " edges..." << endl;

	cout << "  task in (id, exec_time):" << endl;
	for (unsigned int i = 0; i < taskList.size(); i++) {
		GraphTaskNode &t = taskList.at(i);
		t.print();
	}

	cout << "  edge in (id, src, dst, msg_size):" << endl;
	for (unsigned int i = 0; i < edgeList.size(); i++) {
		GraphEdge &e = edgeList.at(i);
		e.print();
	}

	cout << "*** End of printing the application model" << endl << endl;
	return 0;
}

char* AppModel::get_app_name() {

	return appName;
}

int AppModel::set_app_name(char s[]) {

	assert(s[0] != '\0');
	strcpy(appName, s);
	return 0;
}

int	AppModel::GetTaskNum() {

	return taskList.size();
}

int	AppModel::GetEdgeNum() {

	return edgeList.size();
}


