/***************************************************************************************************
 * File Name:             global_cfg.h 
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

#ifndef COSMIC_GLOBAL_CFG
#define COSMIC_GLOBAL_CFG

#include <assert.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

// TODO: move the macro definitions to parameters in the global config file
//		 so that changing their values does not involve re-compiling the program
#define GLOBAL_DEBUG_OUT 	0
#define GLOBAL_PRINT_OUT 	0
#define GLOBAL_TIME_OUT		1 //ZHE: whether a timeout-mechanism is required to avoid too long running

#define TIME_OUT_LEN 7200 //ZHE: in seconds

class GlobalCfg {

public:
	GlobalCfg();
	~GlobalCfg();
	
	int 			load(const char file[]);
	int 			loadDefault(void);
	int				get_ver();
	int				get_sub_ver();
	int				get_mapping_algo();
	int				get_num_iter();
	int				get_num_pop();
	int				get_num_gen();

private:	
	int 			ver;
	int				sub_ver;
	int				mapping_algo;
	int				num_iter;
	int				num_pop;
	int				num_gen;
};

#endif


