/***************************************************************************************************
 * File Name:             algo_buffer_alloc.cpp 
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

#include "algo_buffer_alloc.h"

#define GENETIC_PRINT_OUT 0
#define MAX_VAL 0xffffff
#define MAX_DOUBLE 1.7e300

#if GLOBAL_TIME_OUT
	extern clock_t start_time;
	extern clock_t current_time;
	extern double time_out;
	extern bool is_time_out;
#endif

AlgoBufferAlloc::AlgoBufferAlloc() {

	app_model = NULL;
	glb_cfg = NULL;

	// 10000 is greater than the maximum possible total buffer size of the existing applications
	cost_root = 10000;
}

AlgoBufferAlloc::~AlgoBufferAlloc() {
}

int AlgoBufferAlloc::run(AppModel &a, GlobalCfg &c) {

	// initializations
	app_model = &a;
	glb_cfg = &c;

	char output_file_name[100] = "./res/buffersize_";
	strcat (output_file_name, app_model->get_app_name());
	strcat (output_file_name, ".log");

	if (GENETIC_PRINT_OUT) {
		cout << "*** Output trace file name: " << output_file_name << endl;
		output_file_pointer = fopen(output_file_name, "w");
	}

	// cost_root is defined as 10^n which is greater than the max possible total buffer size.
	// The aim is to shift throughput to higher bits in the evaluation result of the individual
	// to make it the first optimization target and the buffer size the second
	get_cost_root();

	if (GENETIC_PRINT_OUT) {
		cout << "Num tasks: " << app_model->GetTaskNum() << endl;
		cout << "Num edges: " << app_model->GetEdgeNum() << endl << endl;
		fprintf(output_file_pointer, "Num tasks: %d\n", app_model->GetTaskNum());
		fprintf(output_file_pointer, "Num edges: %d\n\n", app_model->GetEdgeNum());
	}

	// create a random seed
	srand((unsigned)time(0));

	// use genetic algorithm to calculate the minimum buffer size
	assign_buffer_size_by_genetic_algorithm();	
	
	int total_buffer_size = 0;
	for (unsigned int i=0; i<best_ind.elements.size(); i++) {
		app_model->get_edge(i).set_buff_size( best_ind.elements.at(i) );
		total_buffer_size += best_ind.elements.at(i);
	}
	
	if (GENETIC_PRINT_OUT) {
		cout << endl << "The best buffer size distribution is:" << endl;
		print_vector(best_ind.elements);
		cout << "The fitness value is: " << best_ind.fitness << endl;
		cout << "The total buffer size is: " << total_buffer_size << endl << endl;

		fprintf(output_file_pointer, "\nThe best buffer size distribution is:\n");
		for (unsigned int i=0; i<best_ind.elements.size(); i++) {
			fprintf(output_file_pointer, " %d", best_ind.elements.at(i));
		}
		fprintf(output_file_pointer, "\nThe fitness value is: %d\n", best_ind.fitness);
		fprintf(output_file_pointer, "The total buffer size is: %d\n\n", total_buffer_size);
	
		fclose(output_file_pointer);
	}
	return 0;
}

int AlgoBufferAlloc::assign_buffer_size_by_genetic_algorithm() {

	if (GLOBAL_DEBUG_OUT) {
		cout << "=============The genetic algorithm starts....============" << endl << endl;
	}

	init_population();
	
	int i;
	for (i=0; i<glb_cfg->get_num_gen(); i++) {
		
		copy_parents_to_offsprings();
		genetic_crossover();
		genetic_mutation();
		genetic_selection();
		update_parents_by_offsprings();
		update_best_individual();

		// the minimum requirement (lower bound) is that each edge only has buffer size of one
		if ( best_ind.fitness <= app_model->GetEdgeNum() )	{
			assert (best_ind.fitness == app_model->GetEdgeNum());
			break;
		}
		
		int total_buffer_size = 0;
		for (unsigned int j=0; j<best_ind.elements.size(); j++) {
			total_buffer_size += best_ind.elements.at(j);
		}
		
		if (GENETIC_PRINT_OUT) {
			cout << "Generation[" << i+1 << "] = ("<< best_ind.fitness << ", " << total_buffer_size << "):";
			print_vector(best_ind.elements);
			cout << endl;
			
			fprintf(output_file_pointer, "Generation[%d] = (%d, %d):\n", i+1, best_ind.fitness, total_buffer_size);
			for (unsigned int j=0; j<best_ind.elements.size(); j++) {
				fprintf(output_file_pointer, " %d", best_ind.elements.at(j));
			}
			fprintf(output_file_pointer, "\n\n");
		}
	}

	if (GENETIC_PRINT_OUT) {
		if (i < glb_cfg->get_num_gen()) {
			cout << endl << "*** Algorithm terminated after " << i+1 << " iterations since the buffer size lower bound is reached ***" << endl;
			fprintf(output_file_pointer, "\n*** Algorithm terminated after %d iterations since the buffer size lower bound is reached ***\n", i+1);
		}
		else {
			cout << endl << "*** Algorithm terminated since the maximum number of generations is reached ***" << endl;
			fprintf(output_file_pointer, "\n*** Algorithm terminated since the maximum number of generations is reached ***\n");
		}
	}
    return 0;
}

int AlgoBufferAlloc::get_cost_root() {

	while (cost_root < app_model->GetEdgeNum() * 10) {

		cost_root = cost_root * 10;
		assert (cost_root < MAX_VAL);
	}
	return 0;
}

int AlgoBufferAlloc::print_vector(const vector<int> &v) {

	for (unsigned int i=0; i<v.size(); i++)
		cout << " " << v.at(i);
	cout << endl;
	return 0;
}

////////////////////////////////////////////////////
// The genetic algorithm
//

inline int AlgoBufferAlloc::random(const int n) {

	return rand() % n;
}

// get a random element: a buffer size in [1, NUM_ITERATION]
int AlgoBufferAlloc::get_random_element() {

	return 1 + random(glb_cfg->get_num_iter());		
}

// get a random individual: a buffer size distribution
int AlgoBufferAlloc::get_random_individual(individual &d) {

	d.elements.clear();
	d.fitness = -1;
	d.elements.resize(app_model->GetEdgeNum());
	for (unsigned int i=0; i<d.elements.size(); i++) {
		d.elements.at(i) = get_random_element();
	}
	return 0;
}

// get a random permutation on n elements and store it to vector v
int AlgoBufferAlloc::get_random_permutation(const int n, vector<int> &v) {

	v.clear();
	for (int i=0; i<n; i++)
		v.push_back(i);

	random_shuffle(v.begin(), v.end());

	if (GLOBAL_DEBUG_OUT) {
		cout << "The random crossover sequence is:";
		print_vector(v);
		cout << endl;
	}
	return 0;
}

// initialize offsprings by copying all the individuals in parents to it
int AlgoBufferAlloc::copy_parents_to_offsprings() {

	pop_offsprings.clear();
	for (unsigned int i=0; i<pop_parents.size(); i++) {
		pop_offsprings.push_back( pop_parents.at(i) );
	}
	assert ((int)pop_offsprings.size() == glb_cfg->get_num_pop());
	return 0;
}

// update parents by copying the best NUM_POPULATION offsprings to it
int AlgoBufferAlloc::update_parents_by_offsprings() {

	assert ((int)pop_offsprings.size() == 4 * glb_cfg->get_num_pop());
	pop_parents.clear();
	for (int i=0; i<glb_cfg->get_num_pop(); i++) {
		pop_parents.push_back( pop_offsprings.at(i) );
	}
	assert ((int)pop_parents.size() == glb_cfg->get_num_pop());
	return 0;
}

// update the best solution, which is the first individual in the parents
int AlgoBufferAlloc::update_best_individual() {

	best_ind.elements.clear();
	best_ind.elements = pop_parents.at(0).elements;
	best_ind.fitness  = pop_parents.at(0).fitness;
	assert ((int)best_ind.elements.size() == app_model->GetEdgeNum());
	assert (best_ind.fitness > 0);
	return 0;
}

// initialize the parents population by randomization
int AlgoBufferAlloc::init_population() {

	if (GLOBAL_DEBUG_OUT) {
		cout << "--- In Initialization step ---" << endl << endl;
	}

	individual ind;
	pop_parents.clear();
	
	// add a special individual which is a possible good solution
	ind.elements.clear();
	ind.fitness = -1;
	ind.elements.resize(app_model->GetEdgeNum());
	for (unsigned long i=0; i<ind.elements.size(); i++) {
		ind.elements.at(i) = 2;
	}
	pop_parents.push_back(ind);
	
	for (long i = 0; i<glb_cfg->get_num_pop()-1; i++) {
		get_random_individual(ind);
		pop_parents.push_back(ind);
	}

	if (GLOBAL_DEBUG_OUT) {
		cout << "The initial population is:" << endl;
		for (unsigned long i=0; i<pop_parents.size(); i++) {
			cout << "ind["<<i<<"]:";
			print_vector(pop_parents.at(i).elements);
		}
		cout << endl;
	}
	return 0;
}

int AlgoBufferAlloc::get_random_bit_mask(vector<int> &v) {

	for (unsigned int i=0; i<v.size(); i++) {
		v.at(i) = random(2);	// get a random 0-1 value 
	}
	if (GLOBAL_DEBUG_OUT) {
		cout << "The bit mask is:";
		print_vector(v);
	}
	return 0;
}

// crossover two individuals with entries of e1 and e2 to generate two new individuals and add them to the offsprings
int AlgoBufferAlloc::uniform_order_based_crossover(const int e1, const int e2) {
	
	individual &parent_ind1 = pop_offsprings.at(e1);
	individual &parent_ind2 = pop_offsprings.at(e2);
	assert ((int)parent_ind1.elements.size() == app_model->GetEdgeNum());
	assert ((int)parent_ind2.elements.size() == app_model->GetEdgeNum());

	individual offspring_ind1, offspring_ind2;
	offspring_ind1.elements.resize(app_model->GetEdgeNum());
	offspring_ind1.fitness = -1;
	offspring_ind2.elements.resize(app_model->GetEdgeNum());
	offspring_ind2.fitness = -1;

	// generate a random bit_mask
	vector<int> bit_mask;
	bit_mask.resize(app_model->GetEdgeNum());
	get_random_bit_mask(bit_mask);

	for (int i=0; i<app_model->GetEdgeNum(); i++) {
		// switch the elements if the bit mask is 1
		if ( 1 == bit_mask.at(i) ) {
			offspring_ind1.elements.at(i) = parent_ind2.elements.at(i);
			offspring_ind2.elements.at(i) = parent_ind1.elements.at(i);
		}
		// do not switch the elements if the bit mask is 0
		else {
			offspring_ind1.elements.at(i) = parent_ind1.elements.at(i);
			offspring_ind2.elements.at(i) = parent_ind2.elements.at(i);
		}
	}

	pop_offsprings.push_back(offspring_ind1);
	pop_offsprings.push_back(offspring_ind2);

	if (GLOBAL_DEBUG_OUT) {
		cout << "The two new children by crossover" << endl;
		print_vector(pop_offsprings.at(e1).elements);
		print_vector(pop_offsprings.at(e2).elements);
		cout << "are:" << endl;
		print_vector(offspring_ind1.elements);
		print_vector(offspring_ind2.elements);
		cout << endl;
	}
	return 0;
}

int AlgoBufferAlloc::genetic_crossover() {

	if (GLOBAL_DEBUG_OUT) {
		cout << "--- In Crossover step ---" << endl << endl;
	}

	// get a random sequence on the populations
	vector<int> seq;
	int pop_size = pop_offsprings.size();
	get_random_permutation(pop_size, seq);
	
	// do crossover on each pair of parents grouped by the sequence
	for (int i=0; i<pop_size/2; i++) {
		uniform_order_based_crossover( seq.at(2*i), seq.at(2*i+1) );
	}
	assert ((int)pop_offsprings.size() == 2*pop_size);
	return 0;
}

// mutate an individual with entry of e to generate a new individual and add it to the offsprings
int AlgoBufferAlloc::ssl_mutation(const int e) {

	individual &parent_ind = pop_offsprings.at(e);
	assert ((int)parent_ind.elements.size() == app_model->GetEdgeNum());

	individual offspring_ind;
	offspring_ind.elements.resize(app_model->GetEdgeNum());
	offspring_ind.fitness = -1;
	
	int n1 = random(app_model->GetEdgeNum());
	int n2 = random(app_model->GetEdgeNum());
	int begin = (n1<=n2)?n1:n2;
	int end   = (n1<=n2)?n2:n1;
	for (int i=0; i<app_model->GetEdgeNum(); i++) {

		// change the element to a random value if it is within the range [begin, end]
		if ( i>=begin && i<=end ) {
			offspring_ind.elements.at(i) = get_random_element();
		}
		// keep the element with its original value if it is out of the range [begin, end]
		else {
			offspring_ind.elements.at(i) = parent_ind.elements.at(i);
		}
	}

	pop_offsprings.push_back(offspring_ind);

	if (GLOBAL_DEBUG_OUT) {
		cout << "The new child by mutating" << endl;
		print_vector(pop_offsprings.at(e).elements);
		cout << "in [" << begin << "," << end << "] is:" << endl;
		print_vector(offspring_ind.elements);
		cout << endl;
	}
	return 0;
}

int AlgoBufferAlloc::genetic_mutation() {

	if (GLOBAL_DEBUG_OUT) {
		cout << "--- In Mutation step ---" << endl << endl;
	}

	int pop_size = pop_offsprings.size();
	for (int i=0; i<pop_size; i++) {
		ssl_mutation(i);
	}
	assert ((int)pop_offsprings.size() == 2*pop_size);
	return 0;
}

// compute the fitness value of the individual with entry of e in offsprings and update its fitness field
long AlgoBufferAlloc::get_fitness_value(const int e) {

	individual &ind = pop_offsprings.at(e);
	assert ((int)ind.elements.size() == app_model->GetEdgeNum());
	if (ind.fitness == -1) {
	
	//	cout << "start hsdf" << endl;
		double thr_inverse = get_hsdf_thru(ind.elements);
	//	cout << "end hsdf" << endl;

		if (GLOBAL_DEBUG_OUT) {
			cout << "inverse of throughput: " << thr_inverse << endl;
		}
		
		int tbs = 0;
		for (unsigned int i=0; i<ind.elements.size(); i++) {
			tbs += ind.elements.at(i);
		}
		assert (tbs < cost_root);
		ind.fitness = (long)thr_inverse*cost_root + tbs;
	//	cout << "thr_inverse = " << thr_inverse << ", cost_root = " << cost_root << ", fitness = " << ind.fitness << endl; 
	}
	assert (ind.fitness > 0);
	return 0;
}

bool compare_individual(const individual d1, const individual d2) {

	return d1.fitness < d2.fitness;
}

int AlgoBufferAlloc::genetic_selection() {

	if (GLOBAL_DEBUG_OUT) {
		cout << "--- In Selection step ---" << endl << endl;
	}

	// calculate fitness values for all the individuals in offsprings
	assert ((int)pop_offsprings.size() == 4*glb_cfg->get_num_pop());
	for (int i=0; i<4*glb_cfg->get_num_pop(); i++) {
		get_fitness_value(i);
	}

	// all children's performance
	if (GLOBAL_DEBUG_OUT) {
		cout << "All the children's information (fitness:elements):" << endl;
		for (int i=0; i<4*glb_cfg->get_num_pop(); i++) {
			cout << pop_offsprings.at(i).fitness << ":\t";
			print_vector(pop_offsprings.at(i).elements);
		}
		cout << endl;
	}

	// select the NUM_POPULATION individuals (1/4) with the smallest fitness values from the offsprings 
	sort (pop_offsprings.begin(), pop_offsprings.end(), compare_individual);

	if (GLOBAL_DEBUG_OUT) {
		cout << "The selected children's information (fitness:elements):" << endl;
		for (int i=0; i<glb_cfg->get_num_pop(); i++) {
			cout << pop_offsprings.at(i).fitness << ":\t";
			print_vector(pop_offsprings.at(i).elements);
		}
		cout << endl;
	}
	return 0;
}

// function to calculate throughput of a hsdf graph
double AlgoBufferAlloc::get_hsdf_thru(vector<int> &sz) {

	const int ACTOR = app_model->GetTaskNum();
	const int BUFF = app_model->GetEdgeNum();
	assert (sz.size() == BUFF);
	double progressTime = 0;
	int thru, fire;
	int time, step, rep_st;
	vector<int> end, as;
	vector<int> ch;
	vector<State> st;

	// initialization
	time=0, step=0, rep_st=-1;
	end.resize(ACTOR, 0);
	as.resize(ACTOR, 0);
	ch.resize(BUFF, 0);
	std::cout << "=>";
	fflush(stdout);
	    
	while (true) {

#if GLOBAL_TIME_OUT
		//ZHE: added to avoid too long running time, the time_out length can be set in main function
		current_time = clock();
		if((double)(current_time - start_time)/CLOCKS_PER_SEC >= time_out) {
			is_time_out = true;
			std::cout << "=>";
			fflush(stdout);
			return MAX_DOUBLE;
		}
		if( (current_time - start_time)/CLOCKS_PER_SEC >= progressTime) {
		    progressTime = 1.01*(current_time - start_time)/progressTime;
		    fflush(stdout);
		    std::cout << "=>";
		}
#endif

		// state refresh: 3 -> 0
		for (int i=0; i<ACTOR; i++) {
			if (as[i] == 3) {
				as[i] = 0;
			}
		}

		// check if free actors can fire: 0 -> 1
		for (int i=0; i<ACTOR; i++) {
			bool can_fire = true;
			GraphTaskNode &t = app_model->get_task(i);
			for (int j=0; j<t.get_num_incoming_edge(); j++) {
				int in_edge_id = t.get_incoming_edge_id(j);
				if (ch[in_edge_id] == 0) {
					can_fire = false;
					break;
				}
				else {
					assert (ch[in_edge_id] > 0);
				}
			}
			for (int j=0; j<t.get_num_outgoing_edge(); j++) {
				int out_edge_id = t.get_outgoing_edge_id(j);
				if (sz[out_edge_id] == ch[out_edge_id]) {
					can_fire = false;
					break;
				}
				else {
					assert (sz[out_edge_id] > ch[out_edge_id]);
				}
			}
			if (can_fire && as[i] == 0) { 
				as[i] = 1; 
				end[i] = time + t.get_exec_time(); 
			}
		}

		// start running the actors by consuming tokens: 1 -> 2
		for (int i=0; i<ACTOR; i++) {
			if (as[i] == 1) {
				as[i] = 2;
			}
		}

		// obtain step: sort in step decreasing order
		step = MAX_VAL;
		for (int i=0; i<ACTOR; i++) {
			if (as[i] == 2 && end[i]-time<step) {
				step = end[i]-time;
			}
		}

		// time advance
		time = time + step;
	//	cout << "time = " << time << endl;
		
		// end running the actors by producing tokens: 2 -> 3
		for (int i=0; i<ACTOR; i++) {
			if (as[i]==2 && end[i]==time) {
				as[i] = 3;
				GraphTaskNode &t = app_model->get_task(i);
				for (int j=0; j<t.get_num_incoming_edge(); j++) {
					int in_edge_id = t.get_incoming_edge_id(j);
					ch[in_edge_id] --;
					assert (ch[in_edge_id] >= 0);
				}
				for (int j=0; j<t.get_num_outgoing_edge(); j++) {
					int out_edge_id = t.get_outgoing_edge_id(j);
					ch[out_edge_id] ++;
					assert (ch[out_edge_id] <= sz[out_edge_id]);
				}
			}
		}

		// check if the state of ending the specific actor: the last one
		if (as[ACTOR-1]==3) {
			unsigned int i;
			for (i=0; i<st.size(); i++) {
				bool rep_st_found = true;
				for (int j=0; j<BUFF; j++) {
					if (ch[j] != st[i].tk[j]) {
						rep_st_found = false;
						break;
					}
				}
				if ( !rep_st_found )
					continue;
				for (int j=0; j<ACTOR-1; j++) {
					if ( !(( end[j]-time>0 && end[j]-time==st[i].rt[j] ) || ( end[j]-time<=0 && st[i].rt[j]==0 )) ) {
						rep_st_found = false;
						break;
					}
				}
				if ( rep_st_found ) 
					break;
			}
			if (i < st.size()) {
				rep_st = i; 
				thru = time - st[i].time; 
				fire = st.size()-i;
				break;
			}
			else {
				State new_st;
				new_st.tk.resize(BUFF, 0);
				new_st.rt.resize(ACTOR-1, 0);
				for (int i=0; i<BUFF; i++) {
					new_st.tk[i] = ch[i];
				}
				for (int i=0; i<ACTOR-1; i++) {
					new_st.rt[i] = (end[i]-time>0) ? end[i]-time : 0;
				}
				new_st.time = time;
				st.push_back(new_st);
			}
		}
	}

	return thru/(double)fire;
}


