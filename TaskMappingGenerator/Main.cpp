/***************************************************************************************************
* File Name:             Main.cpp
*
* Application Developer: University of California Irvine
*
* The copyright information of this software can be found in the file COPYRIGHT.
*
**************************************************************************************************/

#include "AppModel.h"
#include "NoCModel.h"
#include "NoCTaskMapping.h"
#include "PageRank.h"
#include <iostream>
#include <time.h>
using namespace std;
#include <map>

const char software[] = "Task Mapping Suite";
const char version[] = " v1.0";
const int  NUM_OF_PES = 9;
const int  NUM_OF_ROW = 3;
const int  NUM_OF_COL = 3;
const int  NUM_OF_TASKS = 6;
int main(int argc, char *argv[]) 
{
    char *outMapFileName = "output.txt";

    map<pair<int, int>, int> edges;
 //   edges[make_pair(1, 2)] = 25;
 //   edges[make_pair(1, 3)] = 10;
 //   edges[make_pair(1, 4)] = 15;
 //   edges[make_pair(2, 5)] = 15;
	//edges[make_pair(3, 5)] = 10;
	//edges[make_pair(4, 5)] = 15;

	//edges[make_pair(1, 2)] = 1;
	//edges[make_pair(1, 3)] = 2;

	edges[make_pair(1, 2)] = 1;
	edges[make_pair(1, 3)] = 1;
	edges[make_pair(3, 1)] = 1;
	edges[make_pair(3, 2)] = 1;
	edges[make_pair(3, 5)] = 1;
	edges[make_pair(4, 5)] = 1;
	edges[make_pair(5, 4)] = 1;
	edges[make_pair(4, 6)] = 1;
	edges[make_pair(6, 4)] = 1;
	edges[make_pair(5, 6)] = 1;

	// Use a different seed value so that we don't get
	// same result each time we run this program
	// Should only do it once at the begining of program.
	srand(time(NULL));

    cout << endl << "*********************************************" << endl;
    cout << "*        " << software << version << "        *" << endl;
    cout << "*********************************************" << endl << endl;

    ////////////////////////////////////////////////////////////////////
    // 1. Load application and architecture models
    NoCModel noc(NUM_OF_PES, NUM_OF_ROW, NUM_OF_COL);
    
    AppModel app;
	app.set_app_name("NOC Task Mapping");
	app.BuildAppModel(edges, NUM_OF_TASKS);
    	
    ////////////////////////////////////////////////////////////////////
    // 2. Generate mapping and write it to file
	//NoCTaskMapping taskMapping(100000, outMapFileName, &app, &noc);

	//taskMapping.GeneratRandomMapping();
	//int optimal = INT_MAX;
	//int count = 0;
	//vector<bool> used(9, false);
	//vector<int> path;
	//// optimal count is 128
	//// Total is 15120
	//taskMapping.BruteForceMapping(5, 9, path, used, optimal, count);
	//cout << "Communication cost " << optimal;

	////////////////////////////////////////////////////////////////////
	// 3. Page Ranking
	PageRank pageRanker(&app);
	pageRanker.BuildTransitionMatrix();

	vector<double> pr = pageRanker.PageRankAlgo();
	for (int i=0; i<pr.size(); ++i)
	{
		cout << pr[i] << endl;
	}
    return 0;
}									   






