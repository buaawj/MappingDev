/***************************************************************************************************
 * File Name:             algo_buffer_alloc.h 
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
#ifndef COSMIC_ALGO_BUFFER_ALLOC
#define COSMIC_ALGO_BUFFER_ALLOC

#include "global_cfg.h"
#include "app_model.h"
#include "app_task.h"
#include "app_edge.h"
#include <cstdlib>				// needed for randomization
#include <ctime>				// needed for random seed
#include <algorithm>

typedef struct _individual {
	vector<int>	elements;		// elements that forms the individual: a vector of the buffer size distribution
	long		fitness;		// evaluation result of the individual: thru_inverse*cost_root+total_buff_size
}individual;

typedef vector<individual> population;

typedef struct _State {
	vector<int> tk;
	vector<int> rt;
	int time;
}State;

class AlgoBufferAlloc {

public:
	AlgoBufferAlloc();
	~AlgoBufferAlloc();

	int				run(AppModel &a, GlobalCfg &c);
	int				assign_buffer_size_by_genetic_algorithm();

	int				get_cost_root();
	int				print_vector(const vector<int> &v);

	inline int		random(const int n);
	int				get_random_element();
	int				get_random_individual(individual &d);
	int				get_random_permutation(const int n, vector<int> &v);

	int				copy_parents_to_offsprings();
	int				update_parents_by_offsprings();
	int				update_best_individual();
	int				init_population();

	int				get_random_bit_mask(vector<int> &v);
	int				uniform_order_based_crossover(const int e1, const int e2);
	int				genetic_crossover();

	int				ssl_mutation(const int e);
	int				genetic_mutation();

	long			get_fitness_value(const int e);
	int				genetic_selection();

	double			get_hsdf_thru(vector<int> &sz);

private:
	AppModel*		app_model;
	GlobalCfg*		glb_cfg;

	int				cost_root;
	population		pop_parents, pop_offsprings;
	individual		best_ind;						// to store the best result
	FILE*			output_file_pointer;
};

#endif

