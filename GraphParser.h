/***************************************************************************************************
* File Name:             GraphParser.cpp
*
* Application Developer: University of California Irvine
*
* The copyright information of this software can be found in the file COPYRIGHT.
*
**************************************************************************************************/

#ifndef __GRAPH_PARSER__
#define __GRAPH_PARSER__

#include <vector>
#include <map>
#include <iostream>
using namespace std;

enum FSM_PARSER_DATA_TABLE_STATE
{
	FSM_PARSER_DATA_TABLE_IDLE,
	FSM_PARSER_DATA_TABLE_HEADER,
	FSM_PARSER_DATA_TABLE_DATA
};

/*************************************************
* TGFF Graph Parser Class *
************************************************/
class GraphParser
{
private:
	map<int, int> taskTable;           // KEY: task id, task type id
	map<pair<int, int>, int> arcTable; // KEY: <srcId, dstId>, type id
	map<int, int> execTimeTable;       // KEY: type id, execute time
	map<int, int> commVolumTable;      // KEY: arc id,  communication volum

	FSM_PARSER_DATA_TABLE_STATE state;
	string GetFirstString(string s);
	void ParseTaskBlock(int appId, string line);
	void ParseArcBlock(int appId, string line);
	void ParseDataBlock(string line);
	int ExtractValue(string s);
	void BuildTaskGraphMapping(int appId, int start, int end);
	void GenerateRandomTaskMapping(int appId);

public:
	GraphParser(void)
	{
		taskTable.clear();
		arcTable.clear();
		execTimeTable.clear();
		commVolumTable.clear();
	}

	~GraphParser()
	{
	}

	/* Member functions to parser graph */
	bool Parse(const char *fname);
	void PrintParsedTables(void);
};

#endif // __GRAPH_PARSER__