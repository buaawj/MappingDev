/***************************************************************************************************
 * File Name:             global_cfg.cpp 
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

extern string end_str;
extern char temp_str[];

GlobalCfg::GlobalCfg(){
	ver = -1;
	sub_ver = -1;
	mapping_algo = -1;
	num_iter = -1;
	num_pop = -1;
	num_gen = -1;
}

GlobalCfg::~GlobalCfg() {
}


int GlobalCfg::loadDefault(void){
	int  val;
	char str[20];
	ver = 2;
	sub_ver = 0;
	mapping_algo = 0;
	num_iter = 10;
	num_pop = 10;
	num_gen = 10;
	if (GLOBAL_DEBUG_OUT){
	    cout << "*** The following parameters are loaded from the default config \"" << std::endl;
		cout << "    Version = " << ver << endl; 
		cout << "    Subversion = " << sub_ver << endl;
		cout << "    MappingAlgorithm = " << mapping_algo << endl;
		cout << "    Iteration = " << num_iter << endl; 
		cout << "    Population = " << num_pop << endl;
		cout << "    Generation = " << num_gen << endl;
		cout << endl;
	}
	return 0;
}

int GlobalCfg::load(const char file[]){

	int  val;
	char str[20];

	FILE *fp = fopen(file, "r");
	if (fp == NULL) {
		cout << "  ERROR: cannot find the global config file \"" << file << "\"" << endl;
		assert (0);
	}

	// ignore the copyright information
	
	while(1){
		fgets(temp_str, 500, fp);
		string tstr(temp_str); 
		if(tstr.find(end_str)!=string::npos){
			break;
			}
		}
	
	while ( !feof(fp) ) {
		fscanf(fp, "%s", str);
		fscanf(fp, "%d", &val);
		
		if ( !strcmp(str, "Version") ) {
			ver = val;
		}
		else if ( !strcmp(str, "Subversion") ) {
			sub_ver = val;
		}
		else if ( !strcmp(str, "MappingAlgorithm") ) {
			mapping_algo = val;
		}
		else if ( !strcmp(str, "Iteration") ) {
			num_iter = val;
		}
		else if ( !strcmp(str, "Population") ) {
			num_pop = val;
		}
		else if ( !strcmp(str, "Generation") ) {
			num_gen = val;
		}
		else {
			cout << "  WARNING: unknown parameter \"" << str << "\"" << " in the global config file \"" << file << "\"" << endl;
		}
	}
	assert (ver >= 0 && sub_ver >= 0 && mapping_algo >= 0);
	fclose(fp);
	
	if (GLOBAL_DEBUG_OUT){
		cout << "*** The following parameters are loaded from the global config file \"" << file << "\":" << endl;
		cout << "    Version = " << ver << endl; 
		cout << "    Subversion = " << sub_ver << endl;
		cout << "    MappingAlgorithm = " << mapping_algo << endl;
		cout << "    Iteration = " << num_iter << endl; 
		cout << "    Population = " << num_pop << endl;
		cout << "    Generation = " << num_gen << endl;
		cout << endl;
	}

	return 0;
}

int GlobalCfg::get_ver() {

	return ver;
}

int	GlobalCfg::get_sub_ver() {
	
	return sub_ver;
}

int	GlobalCfg::get_mapping_algo() {

	return mapping_algo;
}

int GlobalCfg::get_num_iter() {

	return num_iter;
}

int	GlobalCfg::get_num_pop() {
	
	return num_pop;
}

int	GlobalCfg::get_num_gen() {

	return num_gen;
}



