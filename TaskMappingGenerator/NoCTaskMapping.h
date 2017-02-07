/***************************************************************************************************
* File Name:             NoCTaskMapping.h
*
* Application Developer: University of California Irvine
*
* The copyright information of this software can be found in the file COPYRIGHT.
*
**************************************************************************************************/

#ifndef __NOC_TASK_MAPPING__
#define __NOC_TASK_MAPPING__

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
class NoCTaskMapping
{
private:
	unordered_set<string> mappingTable;
	int numOfMappings;
	const char* fname;

	AppModel* appModel;
	NoCModel* nocModel;

	bool   WriteMappingToFile(string mapping);
	bool   WriteMappingToFile(unordered_map<string, int> &powerData);
	string RandomShuffle(int numTask, int numPe);
	int    GetDistance(int pe1, int pe2);


public:
	NoCTaskMapping(int numberOfMapping, char *fname, AppModel* app, NoCModel* noc)
	{
		this->numOfMappings = numberOfMapping;
		this->fname = fname;
		mappingTable.clear();
		appModel = app;
		nocModel = noc;
	}

	~NoCTaskMapping()
	{

	}

	/* Member functions */
	void GeneratRandomMapping(void);
	int ComputeCommunicationCost(vector<int> &mapping);
	int ComputeCommunicationCost(string mapping);
	void BruteForceMapping(int k, int n, vector<int> &path, vector<bool> &used, int& optimal, int& count);


};

#endif // __NOC_TASK_MAPPING__