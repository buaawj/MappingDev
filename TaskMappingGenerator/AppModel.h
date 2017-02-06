/***************************************************************************************************
* File Name:             AppModel.h
*
* Application Developer: University of California Irvine
*
* The copyright information of this software can be found in the file COPYRIGHT.
*
**************************************************************************************************/

#ifndef APP_MODEL
#define APP_MODEL

#include "GraphEdge.h"
#include "GraphTaskNode.h"

class AppModel
{

public:
	AppModel();
	~AppModel();

	// critical functions
	void				PrintMappingResult();	        // print the mapping and scheduling result, which is stored on each task of the model

	GraphTaskNode&		GetTask(int tid);				// get task by id
	GraphEdge&			GetEdge(int eid);				// get edge by id

	int					load(const char file[], int acc_factor);	// load the application model from file
	int					verify_consistency();			// verify each task/edge id is equal to its index in the list
	int					print();
	int 				BuildAppModel(map<pair<int, int>, int> &edges);

	// basic functions
	char*				get_app_name();
	int					set_app_name(char s[]);
	int					GetTaskNum();
	int					GetEdgeNum();
	int					GetDistance(int pe1, int pe2);


private:
	char				     appName[20];					// the name of the application
	vector<GraphTaskNode>    taskList;						// the list of tasks
	vector<GraphEdge>		 edgeList;						// the list of edges

	vector<int>              starting_task_list;     		// the list of starting tasks
	vector<int>              finishing_task_list;    		// the list of finishing tasks
};

#endif // APP_MODEL

