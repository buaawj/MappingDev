/***************************************************************************************************
 * File Name:             arc_model.cpp 
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

#include "arc_model.h"

extern string end_str;
extern char temp_str[];

ArcModel::ArcModel(){

	arc_name[0] = '\0';
	topo = -1;
	num_proc = -1;
	num_row = -1;
	num_col = -1;
	num_layer = -1;
	proc_acc_factor = -1;
	flit_size = -1;
        cluster_count = -1;
        cluster_size = -1;
}

ArcModel::~ArcModel() {
}

int ArcModel::load(const char file[]){

	int  val;
	char str[20], topo_str[20];

	FILE *fp = fopen(file, "r");
	if (fp == NULL) {
		cout << "  ERROR: cannot find the architecture model file \"" << file << "\"" << endl;
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
	
	// read the architecture name
	fscanf(fp, "%s", str);
	assert ( !strcmp(str, "ArcName") );
	fscanf(fp, "%s", str);
	set_arc_name(str);
	
	while ( !feof(fp) ) {
		fscanf(fp, "%s", str);
		
		if ( !strcmp(str, "Topology") ) {
			
			fscanf(fp, "%s", topo_str);
			if ( !strcmp(topo_str, "Mesh") || !strcmp(topo_str, "Crossbar") ) {
				topo = 0;
			}
			else if ( !strcmp(topo_str, "Torus") ) {
				topo = 1;
			}
			else if ( !strcmp(topo_str, "Fattree") ) {
				topo = 2;
			}
			else if ( !strcmp(topo_str, "3D_Mesh") ) {
				topo = 10;
			}
                        else if ( !strcmp(topo_str, "Ring") ) {
                            topo = 3;
                        }
			else {
				cout << "  ERROR: unknown topology \"" << topo_str << "\"" << endl;
				assert (0);
			}
		}
		else {
			
			fscanf(fp, "%d", &val);
			if ( !strcmp(str, "TotalProcNum") ) {
				num_proc = val;
			}
			else if ( !strcmp(str, "RowNum") ) {
				num_row = val;
			}
			else if ( !strcmp(str, "ColumnNum") ) {
				num_col = val;
			}
			else if ( !strcmp(str, "LayerNum") ) {
				num_layer = val;
			}
			else if ( !strcmp(str, "ProcAccFactor") ) {
				proc_acc_factor = val;
			}
			else if ( !strcmp(str, "FlitSize") ) {
				flit_size = val;
			}
                        else if ( !strcmp(str, "ClusterSize") ) {
                            cluster_size = val;
                        }
                        else if ( !strcmp(str, "ClusterCount") ) {
                            cluster_count = val;
                        }
			else {
				cout << "  WARNING: unknown parameter \"" << str << "\"" << " in the architecture model file \"" << file << "\"" << endl;
			}
		}
	}
	assert (topo >= 0 && num_proc >= 0 && num_row >= 0 && num_col >= 0 && num_layer >= 0 && proc_acc_factor >= 0 && flit_size >= 0);
	
	// check for consistency
	if (topo == 0 || topo == 1 || topo == 10) {
		// mesh and torus
		assert (num_proc == num_row*num_col*num_layer);
	}
	else if (topo == 2) {
		// fattree
		assert (num_row == 0 && num_col == 0);
	}
        else if (topo == 3) {
            //ring with cluster
            assert (cluster_count * cluster_size == num_proc); //assuming only one chip, so this relationship must be met
        }
	
	fclose(fp);
	
	if (GLOBAL_DEBUG_OUT){
		cout << "*** The following parameters are loaded for the architecture model \"" << arc_name << "\":" << endl;
		cout << "    Topology = " << topo_str << endl;
		cout << "    TotalProcNum = " << num_proc << endl; 
		cout << "    RowNum = " << num_row << endl; 
		cout << "    ColumnNum = " << num_col << endl; 
		cout << "    LayerNum = " << num_layer << endl;
		cout << "    ProcAccFactor = " << proc_acc_factor << endl;
		cout << "    FlitSize = " << flit_size << endl; 
		cout << endl;
	}

	return 0;
}

char* ArcModel::get_arc_name() {

	return arc_name;
}

int ArcModel::set_arc_name(char s[]) {

	assert (s[0] != '\0');
	strcpy(arc_name, s);
	return 0;
}

int ArcModel::get_topo() {

	return topo;
}

int	ArcModel::get_num_proc() {

	return num_proc;
}

int ArcModel::get_num_row() {

	return num_row;
}

int	ArcModel::get_num_col() {

	return num_col;
}

int	ArcModel::get_num_layer() {

	return num_layer;
}

int ArcModel::get_proc_acc_factor() {

	return proc_acc_factor;
}

int	ArcModel::get_flit_size() {

	return flit_size;
}

