/***************************************************************************************************
 * File Name:             app_task.h 
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

#ifndef COSMIC_APP_TASK
#define COSMIC_APP_TASK

#include "global_cfg.h"
#include "app_edge.h"

class AppTask {

public:
	AppTask();
	~AppTask();

	// basic functions
	int					get_id();
	int					set_id(int tid);
	int					get_exec_time();
	int					set_exec_time(int t);

	// assistant functions
	int					add_incoming_edge(GraphEdge *e);
	int					add_outgoing_edge(GraphEdge *e);
	int					print();
	int					print_all_edges();

	// architecture-aware functions
	int					get_mapping();
	int					set_mapping(int pid);
	int					get_sched();
	int					set_sched(int s);
	
	// edge visiting functions
	int 				get_num_incoming_edge();
	int 				get_num_outgoing_edge();
	GraphEdge*			get_incoming_edge(int index);
	GraphEdge*			get_outgoing_edge(int index);
	int					get_incoming_edge_id(int index);
	int					get_outgoing_edge_id(int index);
	
private:

	// basic parameters
	int                 id;                         // the id of the task
	int					exec_time;					// the worst-case execution time, the architectural acceleration factor is included when loaded from file	
	vector<GraphEdge*>	incoming_edge_list;         // each entry is an incoming edge
	vector<GraphEdge*>	outgoing_edge_list;         // each entry is an outgoing edge

	// architecture-aware info
	int                 mapping;                    // the id of the PU the task is assigned
	int                 sched;                   	// the sequence number the task is assigned to execute on the PU
};


#endif

