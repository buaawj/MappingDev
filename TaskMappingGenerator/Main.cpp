/***************************************************************************************************
* File Name:             Main.cpp
*
* Application Developer: University of California Irvine
*
* The copyright information of this software can be found in the file COPYRIGHT.
*
**************************************************************************************************/

#include "AppModel.h"
#include "NoCModel.h"
#include <map>

using namespace std;

const char software[] = "Task Mapping Suite";
const char version[] = " v1.0";
const int  NUM_OF_PES = 9;
const int  NUM_OF_ROW = 3;
const int  NUM_OF_COL = 3;

int main(int argc, char *argv[]) 
{
    char *outMapFileName = NULL;
    char *outBufFileName = NULL;
    char *tcgFileName = NULL;
    char *arcFileName = NULL;
    char *configFileName = NULL;

    map<pair<int, int>, int> edges;
    edges[make_pair(1, 2)] = 25;
    edges[make_pair(1, 3)] = 10;
    edges[make_pair(1, 4)] = 15;
    edges[make_pair(2, 5)] = 25;
    edges[make_pair(1, 2)] = 25;


    cout << endl << "New*********************************************" << endl;
    cout << "*        " << software << version << "        *" << endl;
    cout << "*********************************************" << endl << endl;

    ////////////////////////////////////////////////////////////////////
    // 1. Load application and architecture models
    NoCModel noc(NUM_OF_PES, NUM_OF_ROW, NUM_OF_COL);
    
    AppModel appModel;
    
	
    // load architecture model
    ArcModel arc_model;
    if (GLOBAL_PRINT_OUT) {
	cout << ">>  Loading architecture model from \"" << argv[2] << "\"..." << endl;
    }
    arc_model.load(arcFileName);

    // load application model
    AppModel app_model;
    if (GLOBAL_PRINT_OUT) {
	cout << ">>  Loading application model from \"" << argv[1] << "\"..." << endl << endl;
    }
    app_model.load(tcgFileName, arc_model.get_proc_acc_factor());
	
	
    ////////////////////////////////////////////////////////////////////
    // 2. compute buffer allocation by genetic algorithm
	
    if (run_buff_alloc_algo) {
	
	AlgoBufferAlloc algo_buffer_alloc;
#if GLOBAL_TIME_OUT
	start_time = clock();
#endif
	// run() will run the buffer allocation algorithm and save the result in app_model
	if (GLOBAL_PRINT_OUT) {
	    cout << endl << ">>  Running buffer allocation algorithm..." << endl << endl;
	}
	algo_buffer_alloc.run(app_model, glb_cfg);
		
	// print the buffer allocation result
	if (GLOBAL_PRINT_OUT) {
	    app_model.print_buffer_alloc_result();
	}
    }
    else {
	
	// load from buffer alloc result file
	char buff_file[40];
	strcpy(buff_file, "./res/");
	strcat(buff_file, app_model.get_app_name());
	strcat(buff_file, ".buf");
		

	if (GLOBAL_PRINT_OUT) {
	    cout << endl << ">>  Loading buffer allocation result from file..." << endl << endl;
	}

	app_model.load_buffer_alloc_result(outBufFileName);
    }

	
    ////////////////////////////////////////////////////////////////////
    // 3. compute mapping and scheduling by load balancing
	
    if (run_mapping_sched_algo) {
	AlgoMappingSched algo_mapping_sched;
		
	// run() will run the load balancing algorithm and save the result in app_model
	if (GLOBAL_PRINT_OUT) {
	    cout << endl << "\n>>  Running mapping and scheduling algorithm..." << endl << endl;
	}
	algo_mapping_sched.run(app_model, arc_model, glb_cfg);
		
	// print the mapping and scheduling result
	if (GLOBAL_PRINT_OUT) {
	    app_model.print_mapping_sched_result();
	}
    }
	
	
    ////////////////////////////////////////////////////////////////////
    // 4. write the results to the output file
	
    ResultWriter res_writer;
	
    // run() will write the result to the output file
    // the function will output destination file location, name and related information
    if (GLOBAL_PRINT_OUT) {
	cout << endl << ">>  Generating results to files..." << endl << endl;
    }	
    res_writer.run(app_model, arc_model, glb_cfg, run_buff_alloc_algo, run_mapping_sched_algo, outBufFileName, outMapFileName);

#if GLOBAL_TIME_OUT
    if(is_time_out) {
	cout<< "\nRunning time too long, stop exploring memory allocation and return current optimal result.  " <<endl;
    }
#endif

    cout << ">>  Results generated successfully." << endl << endl;
	
    return 0;
}






