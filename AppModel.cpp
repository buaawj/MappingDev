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

int AppModel::print_buffer_alloc_result()
{

}

int	AppModel::print_mapping_sched_result() {

	cout << "*** The mapping and scheduling result in (task id, mapped proc id, schedule sequence number) is:" << endl;
	for (unsigned int i = 0; i < taskList.size(); i++) {
		GraphTaskNode &t = taskList.at(i);
		cout << "    (" << t.GetTaskId() << ", " << t.get_mapping() << ", " << t.get_sched() << ")" << endl;
	}
	cout << endl;

	return 0;
}

int	AppModel::load_buffer_alloc_result(char *file) {

}

GraphTaskNode& AppModel::GetTask(int tid) {

	return taskList.at(tid);
}

GraphEdge& AppModel::GetEdge(int eid) {

	return edgeList.at(eid);
}

int AppModel::load(const char file[], int acc_factor) {

	// temp variables
	int	v[10];
	int num_task = -1;
	int num_edge = -1;
	int num_starting_task = -1;
	int num_finishing_task = -1;
	int dummy;
	float floatDummy;
	assert(acc_factor >= 0);

	FILE *fp = fopen(file, "r");
	if (fp == NULL) {
		cout << "  ERROR: cannot find the application model file \"" << file << "\"" << endl;
		assert(0);
	}

	// ignore the copyright information

	while (1) {
		fgets(temp_str, 500, fp);
		string tstr(temp_str);
		if (tstr.find(end_str) != string::npos) {
			break;
		}
	}

	// read the application name
	char str[100];
	fscanf(fp, "%s", str);
	assert(!strcmp(str, "AppName"));
	fscanf(fp, "%s", str);
	set_app_name(str);

	// read the first line: number of tasks, number of edges
	fscanf(fp, "%d", &num_task);
	fscanf(fp, "%d", &num_edge);
	assert(num_task > 0 && num_edge > 0);
	// initialize the lists
	taskList.resize(num_task);
	edgeList.resize(num_edge);

	fscanf(fp, "%d", &num_starting_task);
	assert(num_starting_task > 0);
	starting_task_list.resize(num_starting_task);

	// read the next num_starting_task lines for starting task info
	for (int i = 0; i < num_starting_task; i++) {

		// task id
		fscanf(fp, "%d", &v[0]);
		assert(v[0] >= 0 && v[0] < num_task);

		// add the task to the list
		starting_task_list.at(i) = v[0];
	}

	fscanf(fp, "%d", &num_finishing_task);
	assert(num_finishing_task > 0);
	finishing_task_list.resize(num_finishing_task);

	// read the next num_finishing_task lines for finishing task info
	for (int i = 0; i < num_finishing_task; i++) {

		// task id
		fscanf(fp, "%d", &v[0]);
		assert(v[0] >= 0 && v[0] < num_task);

		// add the task to the list
		finishing_task_list.at(i) = v[0];
	}

	// read the next num_task lines for task info
	for (int i = 0; i < num_task; i++) {
		// task id
		fscanf(fp, "%d", &v[0]);

		// -- CYCLES INFO
		// -- c_std
		fscanf(fp, "%d", &dummy);
		// -- c_mean
		fscanf(fp, "%d", &v[1]);
		// -- c_min
		fscanf(fp, "%d", &dummy);
		// -- c_max
		fscanf(fp, "%d", &dummy);

		// -- DATA CACHE INFO
		// -- requestSize_mean
		fscanf(fp, "%d", &dummy);
		// -- interArrivalTime_mean
		fscanf(fp, "%d", &dummy);
		// -- dcacheRdSize_mean
		fscanf(fp, "%d", &dummy);
		// -- dcacheRdSize_std
		fscanf(fp, "%d", &dummy);
		// -- dcahceWrSize_mean
		fscanf(fp, "%d", &dummy);
		// -- dcahceWrSize_std
		fscanf(fp, "%d", &dummy);
		// -- dcacheAlpha
		fscanf(fp, "%f", &floatDummy);
		// -- dcacheU1
		fscanf(fp, "%d", &dummy);
		// -- dcacheU2
		fscanf(fp, "%d", &dummy);
		// -- dcacheStd1
		fscanf(fp, "%d", &dummy);
		// -- dcacheStd2
		fscanf(fp, "%d", &dummy);
		// -- dcacheMtbur
		fscanf(fp, "%f", &floatDummy);
		// -- reuse mean
		fscanf(fp, "%d", &dummy);
		// -- reuse std
		fscanf(fp, "%d", &dummy);

		// -- INSTRUCTION CACHE INFO
			// -- requestSize_mean
		fscanf(fp, "%d", &dummy);
		// -- interArrivalTime_mean
		fscanf(fp, "%d", &dummy);
		// -- icacheRdSize_mean
		fscanf(fp, "%d", &dummy);
		// -- icacheRdSize_std
		fscanf(fp, "%d", &dummy);
		// -- icacheAlpha
		fscanf(fp, "%f", &floatDummy);
		// -- icacheU1
		fscanf(fp, "%d", &dummy);
		// -- icacheU2
		fscanf(fp, "%d", &dummy);
		// -- icacheStd1
		fscanf(fp, "%d", &dummy);
		// -- icacheStd2
		fscanf(fp, "%d", &dummy);
		// -- icacheMtbur
		fscanf(fp, "%f", &floatDummy);
		// -- reuse mean
		fscanf(fp, "%d", &dummy);
		// -- reuse std
		fscanf(fp, "%d", &dummy);

		assert(v[0] >= 0 && v[0] < num_task);
		assert(v[1] > 0);

		// construct the task
		GraphTaskNode t;
		t.SetTaskId(v[0]);
		t.SetTaskExectTime(v[1] * acc_factor);		// the task execution time is set to the correct value on the given architecture

		// add the task to the list
		taskList.at(v[0]) = t;
	}

	// read the next num_edge lines for communication info
	for (int i = 0; i < num_edge; i++) {

		// edge id, src task id, dst task id, worst-case message size
		fscanf(fp, "%d", &v[0]);
		fscanf(fp, "%d", &v[1]);
		fscanf(fp, "%d", &v[2]);

		float num_word;
		fscanf(fp, "%f", &num_word);
		assert(v[0] >= 0 && v[0] < num_edge);
		assert(v[1] >= 0 && v[1] < num_task);
		assert(v[2] >= 0 && v[2] < num_task);
		if (num_word <= 0) {
			printf("line got: edgeId(%d) src(%d) dst(%d) numWord(%f)\n", v[0], v[1], v[2], num_word);
			printf("num_word should be greater than 0, got:%f\n", num_word);
			exit(0);
		}
		//	v[3] = (int)(num_word * 32);

		// construct the edge
		GraphEdge e;
		e.SetId(v[0]);
		e.SetSrcTaskId(v[1]);
		e.SetDstTaskId(v[2]);
		e.SetMsgSize(num_word);

		// add the edge to the list
		edgeList.at(v[0]) = e;
		GraphTaskNode &srcTask = taskList.at(v[1]);
		GraphTaskNode &dstTask = taskList.at(v[2]);
		srcTask.AddOutgoingEdge(&edgeList.at(v[0]));
		dstTask.AddIncomingEdge(&edgeList.at(v[0]));
	}

	// verify if the task list is consistent: task/edge id equal to its entry index
	verify_consistency();

	if (GLOBAL_DEBUG_OUT) {
		print();
	}

	return 0;
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


