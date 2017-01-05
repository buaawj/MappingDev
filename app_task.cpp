/***************************************************************************************************
 * File Name:             app_task.cpp 
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

#include "app_task.h"

AppTask::AppTask() {

	id = -1;
	exec_time = -1;
	incoming_edge_list.clear();
	outgoing_edge_list.clear();
	mapping = -1;
	sched = -1;
}

AppTask::~AppTask() {
}

int	AppTask::get_id() {

	return id;
}

int	AppTask::set_id(int tid) {

	assert (tid >= 0);
	id = tid;
	return 0;
}

int	AppTask::get_exec_time() {

	return exec_time;
}

int	AppTask::set_exec_time(int t) {

	assert (t > 0);
	exec_time = t;
	return 0;
}

int AppTask::add_incoming_edge(GraphEdge *e) {

	incoming_edge_list.push_back(e);
	return 0;
}

int AppTask::add_outgoing_edge(GraphEdge *e) {

	outgoing_edge_list.push_back(e);
	return 0;
}

int AppTask::print() {

	cout << "    (" << get_id() << ", " << get_exec_time() << ")" << endl;
	return 0;
}

int AppTask::print_all_edges() {

	cout << "*** Printing all the edges of task " << get_id() << "..." << endl;
	
	cout << "  there are " << incoming_edge_list.size() << " incoming edges:" << endl;
	for (unsigned int i=0; i<incoming_edge_list.size(); i++) {
		GraphEdge &e = *(incoming_edge_list.at(i));
		assert (id == e.get_dst_task_id());
		e.print();
	}
	
	cout << "  there are " << outgoing_edge_list.size() << " outgoing edges:" << endl;
	for (unsigned int i=0; i<outgoing_edge_list.size(); i++) {
		GraphEdge &e = *(outgoing_edge_list.at(i));
		assert (id == e.get_src_task_id());
		e.print();
	}
	
	cout << "*** End of printing all the edges" << endl;
	return 0;
}

int	AppTask::get_mapping() {
	
	return mapping;
}

int	AppTask::set_mapping(int pid) {
	
	assert (pid >= 0);
	mapping = pid;
	return 0;
}

int	AppTask::get_sched() {
	
	return sched;
}

int	AppTask::set_sched(int s) {
	
	assert (s >= 0);
	sched = s;
	return 0;
}

int AppTask::get_num_incoming_edge() {

	return incoming_edge_list.size();
}

int AppTask::get_num_outgoing_edge() {
	
	return outgoing_edge_list.size();
}

GraphEdge* AppTask::get_incoming_edge(int index) {

	return incoming_edge_list.at(index);
}

GraphEdge* AppTask::get_outgoing_edge(int index) {

	return outgoing_edge_list.at(index);
}

int AppTask::get_incoming_edge_id(int index) {

	return incoming_edge_list.at(index)->get_id();
}

int AppTask::get_outgoing_edge_id(int index) {

	return outgoing_edge_list.at(index)->get_id();
}


