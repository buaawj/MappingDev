/***************************************************************************************************
 * File Name:             arc_model.h 
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

#ifndef COSMIC_ARC_MODEL
#define COSMIC_ARC_MODEL

#include "global_cfg.h"

class ArcModel {

public:
	ArcModel();
	~ArcModel();
	
	int					load(const char file[]);
	char*				get_arc_name();
	int					set_arc_name(char s[]);
	int					get_topo();
	int					get_num_proc();
	int					get_num_row();
	int					get_num_col();
	int					get_num_layer();
	int					get_proc_acc_factor();
	int					get_flit_size();
        inline int get_cluster_size(){return cluster_size;}
        inline int get_cluster_count(){return cluster_count;}

private:
	char				arc_name[20];			// name of the architecture
	int                 topo;					// topology: 0 for mesh, 1 for torus, 2 for fattree, 10 for 3d mesh
	int					num_proc;				// total number of processors
	int					num_row;				// number of rows, only available for mesh and torus
	int					num_col;				// number of columns, only available for mesh and torus
	int					num_layer;				// number of layers in 3D view
	int					proc_acc_factor;		// processor acceleration factor
	int					flit_size;				// number of bits in a flit
        int                                     cluster_count;      //number of clusters in a chip  
        int                                     cluster_size;       //number of PUs inside one cluster
};

#endif

