/***************************************************************************************************
 * File Name:             algo_mapping_sched.cpp 
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

#include "algo_mapping_sched.h"
#include <math.h>

#define MAX_VAL		0x3fffffff
#define MIN(a,b)	((a)<(b) ? (a) : (b))

AlgoMappingSched::AlgoMappingSched() {

    app_model = NULL;
    arc_model = NULL;
    glb_cfg = NULL;

    mapping.clear();
    for (unsigned int i=0; i<sched.size(); i++) {
	sched.at(i).clear();
    }
    sched.clear();

    ready_queue.clear();
    proc_available_time.clear();
    task_start_time.clear();
    task_finish_time.clear();
    edge_enabled.clear();
	
    global_time = 0;
}

AlgoMappingSched::~AlgoMappingSched() {
}

int AlgoMappingSched::print_vector(vector<int> &v) {

    for (unsigned int i=0; i<v.size(); i++)
	cout << " " << v.at(i);
    cout << endl;
    return 0;
}

int AlgoMappingSched::get_distance(int p1, int p2) {

    int dist = -1;
    int topo = arc_model->get_topo();	
    int num_proc = arc_model->get_num_proc();
    int num_row = arc_model->get_num_row();
    int num_col = arc_model->get_num_col();
    int num_layer = arc_model->get_num_layer();
        int cluster_size = arc_model->get_cluster_size();
        int cluster_count = arc_model->get_cluster_count();
    assert (p1 >= 0 && p1 < num_proc && p2 >= 0 && p2 < num_proc && p1 != p2);
	
    if (topo == 0) {
	// 2d mesh
	assert (num_proc == num_row * num_col * num_layer);
	int c1x = p1 / num_col;
	int c1y = p1 % num_col;
	int c2x = p2 / num_col;
	int c2y = p2 % num_col;
	dist = abs(c1x-c2x) + abs(c1y-c2y);
    }
    else if (topo == 1) {
	// 2d torus
	assert (num_proc == num_row * num_col * num_layer);
	int c1x = p1 / num_col;
	int c1y = p1 % num_col;
	int c2x = p2 / num_col;
	int c2y = p2 % num_col;
	int row_dist = abs(c1x-c2x);
	int col_dist = abs(c1y-c2y);		
	dist = MIN(row_dist, num_row-row_dist) + MIN(col_dist, num_col-col_dist);
    }
    else if (topo == 2) {
	// 2d fattree
	assert (num_row == 0 && num_col == 0);
		
	// the initial block size is half of the total processors
	int block_size = num_proc/2;
	while (p1/block_size == p2/block_size) {
	    // whenever the two processors are within the same block, go to smaller blocks until they are in different ones
	    block_size = block_size/2;
	    assert (block_size >= 0);
	}
	assert (block_size >= 1);
	dist = 2 * (int)( log((double)block_size)/log(2.0) ) + 2;		
    }
        else if (topo == 3) {
            //ring
            
            int inside_cluster_distance = 1;
            
            assert(cluster_size * cluster_count == num_proc);
            
            if(p1 == p2)
                return 0;
            
            int p1c = p1/cluster_size;
            int p2c = p2/cluster_size;
            
            if(p1c == p2c)
                return inside_cluster_distance;
            
            
            int cluster_distance = min(abs(p1c-p2c), cluster_count-abs(p1c-p2c))*3;
            
            return cluster_distance+2*inside_cluster_distance;
        }
    else if (topo == 10) {
	// 3d mesh
	assert (num_proc == num_row * num_col * num_layer);
	int layer1, layer2;
		
	int proc_in_layer = num_row*num_col;
	layer1 = p1/proc_in_layer;
	p1 = p1%proc_in_layer;
	layer2 = p2/proc_in_layer;
	p2 = p2%proc_in_layer;

	int c1x = p1 / num_col;
	int c1y = p1 % num_col;
	int c2x = p2 / num_col;
	int c2y = p2 % num_col;
	dist = abs(c1x-c2x) + abs(c1y-c2y) + abs(layer1-layer2);		
    }
    else if (topo == 11) {
	// 3d torus
	assert (num_proc == num_row * num_col * num_layer);
	int layer1, layer2;
		
	int proc_in_layer = num_row*num_col;
	layer1 = p1/proc_in_layer;
	p1 = p1%proc_in_layer;
	layer2 = p2/proc_in_layer;
	p2 = p2%proc_in_layer;
		
	int c1x = p1 / num_col;
	int c1y = p1 % num_col;
	int c2x = p2 / num_col;
	int c2y = p2 % num_col;
	int row_dist = abs(c1x-c2x);
	int col_dist = abs(c1y-c2y);
	dist = MIN(row_dist, num_row-row_dist) + MIN(col_dist, num_col-col_dist) + abs(layer1-layer2);
    }
    else if (topo == 12) {
	// 3d fattree
	assert (num_row == 0 && num_col == 0);
	int layer1, layer2;
		
	int proc_in_layer = num_proc / num_layer;
	layer1 = p1/proc_in_layer;
	p1 = p1%proc_in_layer;
	layer2 = p2/proc_in_layer;
	p2 = p2%proc_in_layer;
				
	// the initial block size is half of the total processors
	int block_size = proc_in_layer/2;
		
	while (p1/block_size == p2/block_size) {
	    // whenever the two processors are within the same block, go to smaller blocks until they are in different ones
	    block_size = block_size/2;
	    assert (block_size >= 0);
	}
	assert (block_size >= 1);
		
	int dist = 2 * (int)( log((double)block_size)/log(2.0) ) + 2 + abs(layer1-layer2);
		
    }
    else {
	cout << "*** ERROR: unknown topology no. " << topo << endl;
	assert (0);
    }
	
    assert (dist >= 0);
    return dist;
}

int AlgoMappingSched::get_allocated_proc(int task_id) {

    // load balancing
    assert (glb_cfg->get_mapping_algo() == 0);
    assert (mapping.at(task_id) == -1);
	
    int allocated_proc_id = -1;
    int min_weight = MAX_VAL;
    AppTask &t = app_model->get_task(task_id);

    int num_proc = arc_model->get_num_proc();
    int flit_size = arc_model->get_flit_size();
	
    // compute weight for each processor
    for (int i=0; i<num_proc; i++) {

	// factor_t is the time from now to the finish time of the task if it is mapped to this processor
	// factor_c is the total number of distant packets on all outgoing edges
	int weight, factor_t, factor_c = 0;
	int distance = -1, token_size = -1;
		
	assert (proc_available_time.at(i) >= global_time);
	factor_t = proc_available_time.at(i) - global_time + t.get_exec_time();
		
	// calculate factor_c part 1
	for (int j=0; j<t.get_num_outgoing_edge(); j++) {
		
	    AppEdge &e = *t.get_outgoing_edge(j);
	    int dst_task = e.get_dst_task_id();
	    int dst_proc = mapping.at(dst_task);
			
	    int distance = 0;
	    if (dst_proc >= 0 && dst_proc != i) {
		distance = get_distance(i, dst_proc);
	    }
	    factor_c += (int)(distance * (e.get_msg_size() * 32) / (double)flit_size);
	}

	// calculate factor_c part 2
	for (int j=0; j<t.get_num_incoming_edge(); j++) {
		
	    AppEdge &e = *t.get_incoming_edge(j);
	    int src_task = e.get_src_task_id();
	    int src_proc = mapping.at(src_task);
			
	    int distance = 0;
	    if (src_proc >= 0 && src_proc != i) {
		distance = get_distance(i, src_proc);
	    }
	    factor_c += (int)(distance * (e.get_msg_size() * 32) / (double)flit_size);
	}		
	
	// c1 and c2 are both set to 1
	weight = factor_t + 10*factor_c;
		
	if (weight < min_weight) {
	    min_weight = weight;
	    allocated_proc_id = i;
	}
		
	//	cout << "Allocating task " << task_id << ", proc " << i;
	//	cout << " has f_t=" << factor_t << " and f_c=" << factor_c;
	//	cout << ", dist=" << distance << ", token_size=" << token_size;
	//	cout << ", choose " << allocated_proc_id << endl;
    }
	
    return allocated_proc_id;
}

int AlgoMappingSched::finish_task_execution() {

    int num_finished_tasks = 0;
    int num_edge = app_model->get_num_edge();
	
    for (unsigned int i=0; i<task_start_time.size(); i++) {
		
	AppTask &t = app_model->get_task(i);
	int exec_time = t.get_exec_time();
	int start_time = task_start_time.at(i);
	int &finish_time = task_finish_time.at(i);
		
	// task is finished when it is started and can be finished before global_time, 
	// while its finish time is not recorded (not processed as finished)
	if (start_time >= 0 && finish_time == -1 && start_time + exec_time <= global_time) {
			
	    // record its finish time
	    finish_time = start_time + exec_time;
			
	    // disable all incoming edges
	    for (int j=0; j<t.get_num_incoming_edge(); j++) {
		int eid = t.get_incoming_edge_id(j);
		assert (eid >= 0 && eid < num_edge);
		assert (edge_enabled.at(eid) == true);
		edge_enabled.at(eid) = false;
	    }

	    // enable all outgoing edges
	    for (int j=0; j<t.get_num_outgoing_edge(); j++) {
		int eid = t.get_outgoing_edge_id(j);
		assert (eid >= 0 && eid < num_edge);
		assert (edge_enabled.at(eid) == false);
		edge_enabled.at(eid) = true;
	    }
			
	    // count finished tasks
	    num_finished_tasks ++;
	}
    }
	
    return num_finished_tasks;
}

int AlgoMappingSched::update_ready_queue() {

    ready_queue.clear();
	
    for (unsigned int i=0; i<task_start_time.size(); i++) {
		
	// tasks not ever triggered
	if (task_start_time.at(i) == -1) {
		
	    // check if all incoming edges are enabled
	    AppTask &t = app_model->get_task(i);
	    bool t_ready = true;
	    for (int j=0; j<t.get_num_incoming_edge(); j++) {
		int eid = t.get_incoming_edge_id(j);
				
		if (edge_enabled.at(eid) == false) {
		    t_ready = false;
		    break;
		}
	    }
			
	    if (t_ready) {
		ready_queue.push_back(i);
	    }
	}
    }
	
    return 0;
}

int AlgoMappingSched::save_result_to_app_model() {
	
    // record mapping result
    for (unsigned int i=0; i<mapping.size(); i++) {
	AppTask &t = app_model->get_task(i);
	t.set_mapping( mapping.at(i) );
    }
	
    // record scheduling result
    for (unsigned int i=0; i<sched.size(); i++) {
	for (unsigned int j=0; j<sched.at(i).size(); j++) {
	    AppTask &t = app_model->get_task( sched.at(i).at(j) );
	    t.set_sched(j);
	}
    }
	
    return 0;
}

int AlgoMappingSched::run(AppModel &a, ArcModel &b, GlobalCfg &c) {

    // definitions
    int	num_finished_tasks = 0;			// number of finished tasks
    // initializations
    app_model = &a;
    arc_model = &b;
    glb_cfg = &c;
	
    int num_task = app_model->get_num_task();
    int num_edge = app_model->get_num_edge();
    int num_proc = arc_model->get_num_proc();
	
    ready_queue.clear();
    proc_available_time.resize(num_proc, 0);
    task_start_time.resize(num_task, -1);
    task_finish_time.resize(num_task, -1);
    edge_enabled.resize(num_edge, false);
    mapping.resize(num_task, -1);
    sched.resize(num_proc);
	
    global_time = 0;
	
    // initialize the ready queue
    update_ready_queue();
	
    if (GLOBAL_DEBUG_OUT) {
	cout << "*** The initial ready queue is:";
	print_vector(ready_queue);
    }
	
    // keep executing tasks until all tasks are finished
    while (num_finished_tasks < num_task) {
			
	// 1. allocate all the tasks in the ready queue
	for (unsigned int i=0; i<ready_queue.size(); i++) {

	    // get the first task in the ready queue
	    int task_id = ready_queue.at(i);
	    assert (task_id >= 0 && task_id < num_task);
	    AppTask &t = app_model->get_task(task_id);
			
	    // map and schedule the task
	    int proc_id = get_allocated_proc(task_id);
	    assert (proc_id >= 0 && proc_id < num_proc);

	    // save the result
	    mapping.at(task_id) = proc_id;
	    sched.at(proc_id).push_back(task_id);

	    // proc_available_time should be advanced to global_time first if it is less than global_time
	    int &proc_time = proc_available_time.at(proc_id);
	    assert (proc_time >= global_time);
			
	    // advance proc_available_time, and update task start time
	    assert (task_start_time.at(task_id) == -1 && task_finish_time.at(task_id) == -1);	
	    task_start_time.at(task_id) = proc_time;
	    proc_time = proc_time + t.get_exec_time();
	}

	// 2. time advance: global_time is advanced to the minimum one over all proc_available_time greater than global_time
	int next_time_event = MAX_VAL;
	for (unsigned int i=0; i<proc_available_time.size(); i++) {
	    int time = proc_available_time.at(i);
	    if (time > global_time && time < next_time_event)
		next_time_event = time;
	}
	assert (next_time_event < MAX_VAL);
	global_time = next_time_event;
		
	// check finished tasks, update edge_enabled to trigger new available tasks
	num_finished_tasks += finish_task_execution();
		
	// update the proc_available_time vector
	for (unsigned int i=0; i<proc_available_time.size(); i++) {
	    int &proc_time = proc_available_time.at(i);
	    if (proc_time < global_time)
		proc_time = global_time;
	}
		
	// 3. update the ready queue
	update_ready_queue();

	if (GLOBAL_DEBUG_OUT) {
	    cout << "Global time is advanced to: " << global_time << endl;
	    cout << "The proc available time vector is:";
	    print_vector(proc_available_time);
	    cout << "The ready queue is:";
	    print_vector(ready_queue);
	    cout << endl;
	}
    }

    if (GLOBAL_DEBUG_OUT) {
	cout << "*** All tasks have finished, and the schedule length is: " << global_time << endl;
    }
	
    // verify the result
    for (unsigned int i=0; i<mapping.size(); i++) {
	assert (mapping.at(i) >= 0 && mapping.at(i) < num_proc);
    }
    int accum_num_task = 0;
    for (unsigned int i=0; i<sched.size(); i++) {
	for (unsigned int j=0; j<sched.at(i).size(); j++) {
	    int tid = sched.at(i).at(j);
	    assert (tid >= 0 && tid < num_task);
	}
	accum_num_task += sched.at(i).size();
    }
    assert (accum_num_task == num_task);
	
    if (GLOBAL_DEBUG_OUT) {
	cout << "*** The mapping result is:\n";
	for (unsigned int i=0; i<mapping.size(); i++)
	    cout << "    Task " << i << " -> Processor " << mapping.at(i) << endl;
	cout << "*** The schedule on each processor is:\n";
	for (unsigned int i=0; i<sched.size(); i++) {
	    cout << "    Processor " << i << ":";
	    for (unsigned int j=0; j<sched.at(i).size(); j++) {
		cout << " " << sched.at(i).at(j);
	    }
	    cout << endl;
	}
	cout << endl;
    }
	
    // don't save any mapping and scheduling result to app_model in the middle of the algorithm, do it together at the end
    save_result_to_app_model();
	
    return 0;
}


