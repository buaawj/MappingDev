/***************************************************************************************************
 * File Name:             main.cpp 
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

#include "global_cfg.h"
#include "app_model.h"
#include "arc_model.h"
#include "algo_buffer_alloc.h"
#include "algo_mapping_sched.h"
#include "result_writer.h"

const char software[] = "COSMIC benchmark suite";
const char version[] = " v2.0";
int num_header_lines = 16;
string end_str = "*/";
char temp_str[500];
bool run_buff_alloc_algo;
bool run_mapping_sched_algo;

#if GLOBAL_TIME_OUT
clock_t start_time;
clock_t current_time;
double time_out = TIME_OUT_LEN;
bool is_time_out = false;
#endif



int main(int argc, char *argv[]) {

    char *outMapFileName = NULL;
    char *outBufFileName = NULL;
    char *tcgFileName = NULL;
    char *arcFileName = NULL;
    char *configFileName = NULL;


    cout << endl << "New*********************************************" << endl;
    cout << "*        " << software << version << "        *" << endl;
    cout << "*********************************************" << endl << endl;

    ////////////////////////////////////////////////////////////////////
    // 1. load the global setting, and application and architecture models

    if (argc < 9) {
	cout << "  ERROR: missing parameters" << endl;
	cout << "  Usage: COSMIC_tool -i tcgFile  -a arcFile -m outMapFile -b outBufFile -t timeOutSeconds -c configFile" << endl << endl;
	return -1;
    }
    // all arguments asre in pair.
    if( (argc-1)%2 != 0) {
	cout << "  ERROR: missing parameters" << endl;
	cout << "  Usage: COSMIC_tool -i tcgFile  -a arcFile -m outMapFile -b outBufFile -t timeOutSeconds -c configFile" << endl << endl;
	return -1;

    }
    run_buff_alloc_algo = false;
    run_mapping_sched_algo = false;
    std::cout << "COSMIC-v2.0: ";
    for(int i=1;i<argc;i++) {
	std::cout << " " << argv[i];
	if ( !strcmp(argv[i], "-i") ) {
	    tcgFileName = argv[i+1];
	}
	if ( !strcmp(argv[i], "-a") ) {		
	    arcFileName = argv[i+1];
	}
	if ( !strcmp(argv[i], "-m") ) {	
	    outMapFileName = argv[i+1];
	    run_mapping_sched_algo = true;	
	}
	if ( !strcmp(argv[i], "-b") ) {	
	    outBufFileName = argv[i+1];	
	    run_buff_alloc_algo = true;
	}
	if ( !strcmp(argv[i], "-c") ) {		
	    configFileName = argv[i+1];
	}
	if ( !strcmp(argv[i], "-t") ) {
	    time_out = (unsigned)atoi(argv[i+1]);
	}
    }
    std::cout << std::endl;
	
    // load global config
    GlobalCfg glb_cfg;	
    if (configFileName == NULL) {
	// load default global config file
	if (GLOBAL_PRINT_OUT) {
	    cout << ">>  Loading default global configuration from \"./cfg/default.cfg\" ..." << endl;
	}
	glb_cfg.loadDefault();
    }
    else {
	// load given global config file
	if (GLOBAL_PRINT_OUT) {
	    cout << ">>  Loading global configuration from \"" << argv[3] << "\"..." << endl;
	}
	glb_cfg.load(configFileName);
    }
	
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






