/***************************************************************************************************
* File Name:             NoCTaskMapping.cpp
*
* Application Developer: University of California Irvine
*
* The copyright information of this software can be found in the file COPYRIGHT.
*
**************************************************************************************************/
#include <string>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include "NocTaskMapping.h"
#include <cassert>
#include <unordered_map>
#include <algorithm>
using namespace std;

/***************************************************************************************
 * Random mapping generator Member Functions
 ***************************************************************************************/
void NoCTaskMapping::GeneratRandomMapping(void)
{
	cout << endl;
	cout << "************************" << endl;
	cout << "Generating" << this->numOfMappings <<"Random Mappings" <<  endl;
	cout << "************************" << endl;
	cout << endl;

	assert(appModel != NULL);
	assert(nocModel != NULL);
	int numOfTasks = appModel->GetTaskNum();
	int numOfPes = nocModel->GetNumPes();
	mappingTable.clear();

	// Hash table to remove duplicated data
	unordered_map<string, int> powerData;
	for (int i = 0; i < numOfMappings; ++i)
	{
		string mappingStr;

		mappingStr = RandomShuffle(numOfTasks, numOfPes);
		
		if (!mappingStr.empty() && mappingTable.count(mappingStr) == 0)
		{
			mappingTable.insert(mappingStr);
			powerData[mappingStr] = ComputeCommunicationCost(mappingStr);

		}

	}

	//Write all the data to output file at once to save I/O time
	if (false == WriteMappingToFile(powerData))
	{
		cout << "Error writing output files";
		exit(0);
	}

	return;
}

// Brute Force mapping
void NoCTaskMapping::BruteForceMapping(int k, int n, vector<int> &path, vector<bool> &used, int& optimal, int& count)
{
	if (path.size() == k)
	{
		int cost = ComputeCommunicationCost(path);
			count++;
		optimal = min(optimal, cost);
		return;
	}

	for (int i = 0; i<n; ++i)
	{
		if (used[i] == true) continue;

		used[i] = true;
		path.push_back(i);
		BruteForceMapping(k, n, path, used, optimal, count);
		used[i] = false;
		path.pop_back();
	}
}


/*
bool NoCTaskMapping::WriteMappingToFile(string mapping)
{
	// create the output file with path info
	assert(fname != NULL);
	//FILE *fp = fopen_s(fname, "w");
	//assert(fp != NULL);

	// fopen_s is not sharedable, thus getting error13(access denied)
	FILE *fp = _fsopen(fname, "a", _SH_DENYWR);
	if ( fp == NULL)
	{
		cout<<"File was not opened\n";
		exit(0);
	}

	assert(appModel != NULL);
	int numTasks = appModel->GetTaskNum();

	fprintf(fp, "%s\t%s\n", "map: ", mapping.c_str());

	// task id, pe id
	for (int i = 0; i < numTasks; i++)
	{
		// write to file
		fprintf(fp, "%d\t%d\n", i + 1, mapping[i]-'0');
	}

	// Compute communication cost
	fprintf(fp, "%d\n", ComputeCommunicationCost(mapping));

	if (fp != NULL)
	{
		fclose(fp);
	}
}*/

bool NoCTaskMapping::WriteMappingToFile(string mapping)
{
	// create the output file with path info
	assert(fname != NULL);
	//FILE *fp = fopen_s(fname, "w");
	//assert(fp != NULL);

	ofstream fout;

	fout.open(fname, ios::app);    // open file for appending
	if (fout.fail())
	{
		return false;
	}

	assert(appModel != NULL);
	int numTasks = appModel->GetTaskNum();

	fout << "map: " << mapping;

	// Compute communication cost
	fout << " " << ComputeCommunicationCost(mapping) << endl; 

	fout.close();
	return true;
}

bool NoCTaskMapping::WriteMappingToFile(unordered_map<string, int> &powerData)
{
	// create the output file with path info
	assert(fname != NULL);
	//FILE *fp = fopen_s(fname, "w");
	//assert(fp != NULL);

	ofstream fout;

	fout.open(fname, ios::app);    // open file for appending
	if (fout.fail())
	{
		return false;
	}

	for (unordered_map<string, int>::iterator it = powerData.begin(); it != powerData.end(); ++it)
	{
		fout << "map: " << it->first;

		// Compute communication cost
		fout << " " << it->second << endl;
	}

	fout.close();
	return true;
}

string NoCTaskMapping::RandomShuffle(int k, int n)
{
	// Reservoir Sampling: randomly choosing k samples from a list of n items
	vector<int> pes(n);
//	vector<int> mapping(k);
	stringstream ss;
	for (int i = 0; i < n; ++i) pes[i] = i;

	/*int i;  // index for elements in stream[]
	for (i = 0; i < k; i++)
		mapping[i] = pes[i];

	// Iterate from the (k+1)th element to nth element
	for (; i < n; i++)
	{
		// Pick a random index from 0 to i.
		int j = rand() % (i + 1);

		// If the randomly  picked index is smaller than k, then replace
		// the element present at the index with new element from stream
		if (j < k)
			mapping[j] = pes[i];
	}*/

	random_shuffle(pes.begin(), pes.end());

	// Convet mapping vector to string
	for (unsigned int j = 0; j < k; ++j)
	{
		ss << pes[j];
	}
	return ss.str();
}

int NoCTaskMapping::ComputeCommunicationCost(vector<int> &mapping)
{
	int totalCost = 0;
	assert(appModel != NULL);
	if (mapping.empty())
	{
		return 0;
	}

	vector<GraphEdge> &edges = appModel->GetEdgeList();
	int numTasks = appModel->GetTaskNum();

	for (unsigned int i = 0; i < edges.size(); ++i)
	{
		// task id is 1-based.
		int srcTaskId = edges[i].GetSrcTaskId() - 1;
		int destTaskId = edges[i].GetDstTaskId() - 1;
		int commVol = edges[i].GetMsgSize();

		assert(srcTaskId >= 0 && srcTaskId < numTasks && destTaskId >= 0 && destTaskId < numTasks);

		int pe1 = mapping[srcTaskId];
		int pe2 = mapping[destTaskId];
		int dist = GetDistance(pe1, pe2);
		totalCost += GetDistance(pe1, pe2) * commVol;
	}

	return totalCost;
}


int NoCTaskMapping::ComputeCommunicationCost(string mapping)
{
	int totalCost = 0;
	assert(appModel != NULL);
	if (mapping.empty())
	{
		return 0;
	}

	vector<GraphEdge> &edges = appModel->GetEdgeList();
	int numTasks = appModel->GetTaskNum();

	for (unsigned int i = 0; i < edges.size(); ++i)
	{
		// task id is 1-based.
		int srcTaskId = edges[i].GetSrcTaskId()-1;
		int destTaskId = edges[i].GetDstTaskId()-1;
		int commVol = edges[i].GetMsgSize();

		assert(srcTaskId >= 0 && srcTaskId < numTasks && destTaskId >= 0 && destTaskId < numTasks);

		int pe1 = mapping[srcTaskId] - '0';
		int pe2 = mapping[destTaskId] - '0';
		int dist = GetDistance(pe1, pe2);
		totalCost += GetDistance(pe1, pe2) * commVol;
	}

	return totalCost;
}

int NoCTaskMapping::GetDistance(int pe1, int pe2)
{
	assert(nocModel != NULL);

	int dist = -1;
	int numPes = nocModel->GetNumPes();
	int numRow = nocModel->GetNumrow();
	int numCol = nocModel->GetNumCol();
	assert(pe1 >= 0 && pe1 < numPes && pe2 >= 0 && pe2 < numPes && pe1 != pe2);

	// 2d mesh
	int c1x = pe1 / numCol;
	int c1y = pe1 % numCol;
	int c2x = pe2 / numCol;
	int c2y = pe2 % numCol;
	dist = abs(c1x - c2x) + abs(c1y - c2y);

	return dist;
}