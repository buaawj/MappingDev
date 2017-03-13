/***************************************************************************************************
* File Name:             PageRank.h
*
* Application Developer: University of California Irvine
*
* The copyright information of this software can be found in the file COPYRIGHT.
*
**************************************************************************************************/

#ifndef __NOC_PAGERANK__
#define __NOC_PAGERANK__

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include "AppModel.h"
#include "NoCModel.h"

using namespace std;


/*************************************************
 * NoC Task Mapping Class *
 ************************************************/
class PageRank
{
private:

	AppModel* appModel;
	double alpha; // the pagerank damping factor
	double convergence;
	unsigned long max_iterations;
	vector<double> pageRank;

	vector< vector<double> > transitionMatrix; 


public:
	PageRank(AppModel* app, unsigned long numberofIteration = 200, double alpha = 0.85)
	{
		this->alpha = alpha;
		this->max_iterations = numberofIteration;
		this->appModel = app;

		int tasks = app->GetTaskNum();
		transitionMatrix.resize(tasks, vector<double>(tasks, 0.0));
		pageRank.resize(tasks);
	}

	~PageRank()
	{

	}

	/* Member functions */
	void BuildTransitionMatrix(void);
	vector<double>& PageRank::PageRankAlgo(void);

};

#endif // __NOC_PAGERANK__