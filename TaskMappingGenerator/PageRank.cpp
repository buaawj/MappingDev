/***************************************************************************************************
* File Name:             NoCTaskMapping.cpp
*
* Application Developer: University of California Irvine
*
* The copyright information of this software can be found in the file COPYRIGHT.
*
**************************************************************************************************/
#include <string>
#include <vector>
#include "PageRank.h"
#include <cassert>
#include <algorithm>
using namespace std;

/***************************************************************************************
 * PageRank Member Functions
 ***************************************************************************************/
void PageRank::BuildTransitionMatrix(void)
{
	vector<int> danglingNodes;
	assert(appModel != NULL);

	int N = appModel->GetTaskNum();
	// Taks Id starts from 1,not 0.
	for (int i=1; i<=N; ++i)
	{
		GraphTaskNode node = appModel->GetTaskList()[i];
		int sum = 0;

		// Dangling node
		if (node.GetOutgoingEdgeNum() == 0)
		{
			for (int i = 0; i < N; ++i)
			{
				this->transitionMatrix[node.GetTaskId()-1][i] = 1.0 / (double)N;
			}
			continue;
		}

		// Not dangling node
		for (int j=0; j<node.GetOutgoingEdgeNum(); ++j)
		{
			int destNode = node.GetOutgoingEdge(j)->GetDstTaskId();
			int weight = node.GetOutgoingEdge(j)->GetMsgSize();
			this->transitionMatrix[node.GetTaskId()-1][destNode-1] = weight;
			sum += weight;
		}

		// Normalize this row
		for (int i = 0; i < N; ++i)
		{
			this->transitionMatrix[node.GetTaskId()-1][i] /= sum;
		}
	}
}

// Page ranking algorithm
vector<double>& PageRank::PageRankAlgo(void)
{
	double diff = 0.0;
	double delta = 1.0e-6;
	bool isDone = false;
	int iteration = 0;

	assert(appModel != NULL);
	int N = appModel->GetTaskNum();
	vector<double> v = vector<double>(N, 1.0/ (double)N);

	// Intialize pagerank to 1/N
	for (int i=0 ; i<pageRank.size(); ++i)
	{
		pageRank[i] = 1.0/(double)N;
	}

	// Do the power iteration
	while (!isDone && iteration < max_iterations)
	{
		// pr(k+1) = alpha*pr(k)*M + (1-alpha) * V/N (v is all one vector)
		vector<double> last(pageRank);
		for (int i = 0; i<pageRank.size(); ++i)
		{
			pageRank[i] = 0.0;
		}

		for (int i=0; i<pageRank.size(); ++i)
		{
			double sum = 0.0;

			for (int j = 0; j < last.size(); ++j)
			{
				sum += alpha*last[j] * transitionMatrix[j][i];
			}

			pageRank[i] = sum + (1.0 - alpha) / (double)N;
		}

		double sum = 0.0;
		for (int i = 0; i < pageRank.size(); ++i)
		{
			sum += fabs(pageRank[i] - last[i]);
		}

		if (fabs(diff - sum) < pageRank.size()*delta)
		{
			isDone = true;
		}
		diff = sum;
		++iteration;
	}
	
	if (iteration >= max_iterations)
	{
		cout << "Error---The problem is not feasible";
		assert(true);
	}

	return pageRank;
}
