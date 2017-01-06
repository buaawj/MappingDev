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

#include "global_cfg.h"
#include "GraphEdge.h"
#include "GraphTaskNode.h"

class AppModel
{

public:
	AppModel();
	~AppModel();

	// critical functions
	int					print_buffer_alloc_result();	// print the buffer size allocation result, which is stored on each edge of the model
	int					print_mapping_sched_result();	// print the mapping and scheduling result, which is stored on each task of the model
	int					load_buffer_alloc_result(char *file);		// load the buffer size allocation result from file

	GraphTaskNode&		GetTask(int tid);				// get task by id
	GraphEdge&			GetEdge(int eid);				// get edge by id

	// assistant functions
	int					load(const char file[], int acc_factor);	// load the application model from file
	int					verify_consistency();			// verify each task/edge id is equal to its index in the list
	int					print();

	// basic functions
	char*				get_app_name();
	int					set_app_name(char s[]);
	int					GetTaskNum();
	int					GetEdgeNum();

private:
	char				     appName[20];					// the name of the application
	vector<GraphTaskNode>    taskList;						// the list of tasks
	vector<GraphEdge>		 edgeList;						// the list of edges

	vector<int>              starting_task_list;     		// the list of starting tasks
	vector<int>              finishing_task_list;    		// the list of finishing tasks

};

#endif // APP_MODEL

