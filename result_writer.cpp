/***************************************************************************************************
 * File Name:             result_writer.cpp 
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

#include "result_writer.h"
#include <math.h>

ResultWriter::ResultWriter() {

	app_model = NULL;
	arc_model = NULL;
	glb_cfg = NULL;
}

ResultWriter::~ResultWriter() {
}

int ResultWriter::run(AppModel &a, ArcModel &b, GlobalCfg &c, bool buff_alloc, bool mapping_sched, char *bufFileName, char *mapFileName) {

	// initialize the pointers to the objects
	app_model = &a;
	arc_model = &b;
	glb_cfg = &c;

	// write buffer allocation result to file
	if (buff_alloc) {
		write_buffer_alloc_result_to_file(bufFileName);
	}
	else {
		cout << endl << "No buffer allocation result file generated." << endl << endl;
	}

	// write mapping and scheduling result to file
	if (mapping_sched) {
		write_mapping_sched_result_to_file(mapFileName);
	}
	else {
		cout << endl << "No mapping and scheduling result file generated." << endl << endl;
	}

	return 0;
}

int ResultWriter::write_buffer_alloc_result_to_file(char *bufFileName) {

	// construct the output file name for buffer allocation


	if (GLOBAL_PRINT_OUT) {
		cout << "The buffer allocation result is recorded in \"" << bufFileName << "\"" << endl;
	}
 
	// create the output file with path info
	FILE *fp = fopen(bufFileName, "w");
	assert (fp != NULL);
	
	// write copyright information
	fprintf(fp, "/*********************************************************************************\n");
	fprintf(fp, " *\n");
	fprintf(fp, " * File name:                      %s\n", bufFileName);
	fprintf(fp, " * Version:                        %d.%d\n", glb_cfg->get_ver(), glb_cfg->get_sub_ver());
	fprintf(fp, " *\n");
	fprintf(fp, " * Software:                       COSMIC benchmark suite\n");
	fprintf(fp, " * Author:                         Zhe Wang (HKUST), Weichen Liu (HKUST), Jiang Xu (HKUST),\n");
	fprintf(fp, " *                                 Xuan Wang (HKUST), Xiaowen Wu (HKUST), Zhehui Wang (HKUST), \n");
	fprintf(fp, " *                                 Peng Yang (HKUST), Mahdi Nikdast (HKUST), Luan H.K. Duong (HKUST),\n");	
	fprintf(fp, " *                                 Wei Zhang (HKUST), Bin Li (Intel), Ravi Lyer (Intel), Ramesh Illikkal (Intel)\n");
	fprintf(fp, " *                                 \n");
	fprintf(fp, " * Website:                        http://www.ece.ust.hk/~eexu\n");
	fprintf(fp, " *\n");
	fprintf(fp, " * The copyright information of this program can be found in the file COPYRIGHT.\n");
	fprintf(fp, " *\n");
	fprintf(fp, " *********************************************************************************/\n");
		
	// write the first line:
	// number of edges
	fprintf(fp, "%d\n", app_model->GetEdgeNum());

	// write the next number of edges lines:
	// edge id, buffer size
	for (int i=0; i<app_model->GetEdgeNum(); i++) {
		
		// obtain edge info
		GraphEdge &e = app_model->get_edge(i);
		int edge_id = e.get_id();
	//	int buff_size = e.get_buff_size();
		int buff_size = e.get_buff_size() * (int)ceil(e.get_msg_size());
		
		
		// write to file
		fprintf(fp, "%d\t%d\n", edge_id, buff_size);
	}

	// finish writing by closing the file
	fclose(fp);
	return 0;
}

int ResultWriter::write_mapping_sched_result_to_file(char *mapFileName) {

	// construct the output file name for mapping and scheduling
	// create the output file
	FILE *fp = fopen(mapFileName, "w");
	assert (fp != NULL);

	// write copyright information
	fprintf(fp, "/*********************************************************************************\n");
	fprintf(fp, " *\n");
	fprintf(fp, " * File name:                      %s\n", mapFileName);
	fprintf(fp, " * Version:                        %d.%d\n", glb_cfg->get_ver(), glb_cfg->get_sub_ver());
	fprintf(fp, " *\n");
	fprintf(fp, " * Software:                       COSMIC benchmark suite\n");
	fprintf(fp, " * Author:                         Zhe Wang (HKUST), Weichen Liu (HKUST), Jiang Xu (HKUST),\n");
	fprintf(fp, " *                                 Xuan Wang (HKUST), Xiaowen Wu (HKUST), Zhehui Wang (HKUST), \n");
	fprintf(fp, " *                                 Peng Yang (HKUST), Mahdi Nikdast (HKUST), Luan H.K. Duong (HKUST),\n");	
	fprintf(fp, " *                                 Wei Zhang (HKUST), Bin Li (Intel), Ravi Lyer (Intel), Ramesh Illikkal (Intel)\n");
	fprintf(fp, " *                                 \n");
	fprintf(fp, " * Website:                        http://www.ece.ust.hk/~eexu\n");
	fprintf(fp, " *\n");
	fprintf(fp, " * The copyright information of this program can be found in the file COPYRIGHT.\n");
	fprintf(fp, " *\n");
	fprintf(fp, " *********************************************************************************/\n");

	// write the first line:
	// number of PUs, number of tasks, number of edges
	fprintf(fp, "%d\n%d\t%d\n", arc_model->get_num_proc(), app_model->GetTaskNum(), app_model->GetEdgeNum());

	// write the next number of tasks lines:
	// task id, mapped proc id, schedule sequence number
	for (int i=0; i<app_model->GetTaskNum(); i++) {
	
		// obtain task info
		GraphTaskNode &t = app_model->get_task(i);
		int task_id = i;
		int mapping = t.get_mapping();
		int sched = t.get_sched();
		
		// write to file
		if( (arc_model->get_topo() == 0 || arc_model->get_topo() == 1) && arc_model->get_num_proc() == 1 ) {
		    fprintf(fp, "%d\t%d\t%d\t%d\n", task_id, 0, 0, i);
		}
		else if (arc_model->get_topo() == 0 || arc_model->get_topo() == 1) {
			// mesh or torus in 2d
		    int num_col = arc_model->get_num_col();
		    int x = mapping / num_col;
		    int y = mapping % num_col;
		    fprintf(fp, "%d\t%d\t%d\t%d\n", task_id, x, y, sched);
		}
		else if (arc_model->get_topo() == 2 || arc_model->get_topo() == 3) {
		    // fattree in 2d
		    fprintf(fp, "%d\t%d\t%d\n", task_id, mapping, sched);
		}
		else {
		    assert (0);
		}
		
	}
	
	// finish writing by closing the file
	fclose(fp);	
	return 0;
}

int ResultWriter::construct_folder_name(char *folder_name, int type) {

	strcpy (folder_name, "./res/");
	if (type == 0) {
		// buffer allocation
	//	strcat (folder_name, "buf/");
	}
	else if (type == 1) {
		// mapping and scheduling
	//	strcat (folder_name, arc_model->get_arc_name());
	//	strcat (folder_name, "/");	
	}
	else {
		assert (0);
	}
	
	return 0;
}

int ResultWriter::construct_file_name(char *file_name, int type) {

	if (type == 0) {
		// buffer allocation result
		strcpy (file_name, app_model->get_app_name());
	//	strcat (file_name, "_");
	//	strcat (file_name, arc_model->get_arc_name());
		strcat (file_name, ".buf");
	}
	else if (type == 1) {
		// mapping and scheduling result
		strcpy (file_name, app_model->get_app_name());
		strcat (file_name, "_");
		strcat (file_name, arc_model->get_arc_name());
		strcat (file_name, ".map");
	}
	else {
		assert (0);
	}

	return 0;
}

