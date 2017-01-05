/***************************************************************************************************
 * File Name:             algo_mapping_sched.h 
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

#ifndef COSMIC_ALGO_MAPPING_SCHED
#define COSMIC_ALGO_MAPPING_SCHED

#include "global_cfg.h"
#include "app_model.h"
#include "app_task.h"
#include "app_edge.h"
#include "arc_model.h"

class AlgoMappingSched {

public:
    AlgoMappingSched();
    ~AlgoMappingSched();

    // basic functions
    int print_vector(vector<int> &v);
    int get_distance(int p1, int p2);
    int get_allocated_proc(int task_id);
    int finish_task_execution();
    int update_ready_queue();
    int save_result_to_app_model();
    int run(AppModel &a, ArcModel &b, GlobalCfg &c);	// map and schedule the application tasks to the processors

private:
    AppModel*				app_model;
    ArcModel*				arc_model;
    GlobalCfg*				glb_cfg;

    vector<int> 			mapping;
    vector< vector<int> > 	sched;

    vector<int> 			ready_queue;
    vector<int> 			proc_available_time;
    vector<int>				task_start_time;
    vector<int>				task_finish_time;
    vector<bool>			edge_enabled;				// true when an edge is enabled by its src task finished
	
    int 					global_time;				// the time instant to make decisions
};

#endif

