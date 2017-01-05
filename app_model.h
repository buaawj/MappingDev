/***************************************************************************************************
 * File Name:             app_model.h 
 *                        
 * Application Developer: Rafael K. V. Maeda (HKUST), Zhe Wang (HKUST), 
 *                        Jiang Xu (HKUST) 
 *                        
 * Software:              COSMIC Heterogeneous Multiprocessor Benchmark 
 * Version:               2.0 
 * Author:                Rafael K. V. Maeda (HKUST), Wang Zhe (HKUST), Jiang Xu (HKUST), 
 *                        Zhehui Wang (HKUST), Peng Yang (HKUST), Luan H.K. Duong (HKUST), 
 *                        Zhifei Wang (HKUST), Hoaran Li (HKUST), Xuanqi Chen (HKUST), 
 *                        Zhongyuan Tian (HKUST) 
 *                        
 * Past Members:          Xiaowen Wu, Weichen Liu, Xuan Wang, Mahdi Nikdast 
 *                        
 * Website:               http://www.ece.ust.hk/~eexu 
 *                        
 * The copyright information of this software can be found in the file COPYRIGHT. 
 *
 **************************************************************************************************/

#ifndef COSMIC_APP_MODEL
#define COSMIC_APP_MODEL

#include "global_cfg.h"
#include "app_edge.h"
#include "app_task.h"

class AppModel {

public:
	AppModel();
	~AppModel();
	
	// critical functions
	int					print_buffer_alloc_result();	// print the buffer size allocation result, which is stored on each edge of the model
	int					print_mapping_sched_result();	// print the mapping and scheduling result, which is stored on each task of the model
	int					load_buffer_alloc_result(char *file);		// load the buffer size allocation result from file
	
	AppTask&			get_task(int tid);				// get task by id
	GraphEdge&			get_edge(int eid);				// get edge by id
	
	// assistant functions
	int					load(const char file[], int acc_factor);	// load the application model from file
	int					verify_consistency();			// verify each task/edge id is equal to its index in the list
	int					print();

	// basic functions
	char*				get_app_name();
	int					set_app_name(char s[]);
	int					get_num_task();
	int					get_num_edge();
		
private:
	char				app_name[20];					// the name of the application
	vector<AppTask>		task_list;						// the list of tasks
	vector<GraphEdge>		edge_list;						// the list of edges
	
    vector<int>         starting_task_list;     		// the list of starting tasks
    vector<int>         finishing_task_list;    		// the list of finishing tasks
	
};

#endif

