/***************************************************************************************************
* File Name:             RandomMappingGenerator.cpp
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
#include "RandomMapping.h"

using namespace std;

/***************************************************************************************
 * Random mapping generator Member Functions 
 ***************************************************************************************/
void RandomMappingGenerator::GeneratRandomMapping(void)
{
    ifstream infile(fname);

    cout << endl;
    cout << "************************" << endl;
    cout << "Generating %s Random Mappings" << this->numOfMappings << endl;
    cout << "************************" << endl;
    cout << endl;

    for(int i=0; i<numOfMappings; ++i)
    {
        stringstream ss;
        string mappingStr;

        do
        {
            mappingStr = RandomShuffle(numOfTasks,numOfPes);
        }while(mappingTable.count(mappingStr) > 0)

        if(mappingStr.empty()==false)
        {
            mappingTable.insert(mappingStr);
        }

       if( false == WriteMappingToFile(mappingStr))
       {
           cout<< "Error Writing Mapping to output file";
           exit(0);
       }    
    }
    
    return;
}

bool RandomMappingGenerator::WriteMappingToFile(string mapping)
{
    // create the output file with path info
    assert(fname != NULL);
	FILE *fp = fopen(fname, "w");
	assert (fp != NULL);
		
	fprintf(fp, "%d\n", "map");

	// task id, pe id
	for (int i=0; i<numOfTasks; i++) 
    {
	    // write to file
		fprintf(fp, "%d\t%d\n", i+1, mapping[i]);
	}
}

string RandomMappingGenerator:: RandomShuffle(int numTask, int numPe  )
{
    srand(time(NULL));
    vector<int> mapping(numTask);
    stringstream ss;

    for(int i=0 ;i<numTask; ++i)
    {
        int peid = rand() % (numPe);
        mapping.push_back(peid);
    }

    // Convet mapping vector to string
    for(int j=0; j<mapping.size(); ++j)
    {
        ss<< mapping[j];
    }

    return ss.str();
}