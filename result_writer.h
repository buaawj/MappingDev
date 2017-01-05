/***************************************************************************************************
 * File Name:             result_writer.h 
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

#ifndef COSMIC_RESULT_WRITER
#define COSMIC_RESULT_WRITER

#include "global_cfg.h"
#include "app_model.h"
#include "app_task.h"
#include "app_edge.h"
#include "arc_model.h"

class ResultWriter {

public:
	ResultWriter();
	~ResultWriter();

    int					run(AppModel &a, ArcModel &b, GlobalCfg &c, bool buff_alloc, bool mapping_sched, char *bufFileName, char *mapFileName);
	int					write_buffer_alloc_result_to_file(char *bufFileName);
	int					write_mapping_sched_result_to_file(char *mapFileName);
	int 				construct_folder_name(char *folder_name, int type);		// format the output file names, type=0 for buffer allocation result, type=1 for mapping and scheduling result	
	int 				construct_file_name(char *file_name, int type);			// format the output file names, type=0 for buffer allocation result, type=1 for mapping and scheduling result
	
private:
	AppModel*			app_model;
	ArcModel*			arc_model;
	GlobalCfg*			glb_cfg;
};

#endif

