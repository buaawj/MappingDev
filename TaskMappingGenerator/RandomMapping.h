/***************************************************************************************************
* File Name:             RandomMappingGenerator.h
*
* Application Developer: University of California Irvine
*
* The copyright information of this software can be found in the file COPYRIGHT.
*
**************************************************************************************************/

#ifndef __RANDOM_MAPPING_GENERATOR__
#define __RANDOM_MAPPING_GENERATOR__

#include <vector>
#include <unordered_set>
#include <iostream>
#include <algorithm>

using namespace std;


/*************************************************
 * Random Mapping Generator Class *
 ************************************************/
class RandomMappingGenerator
{
  private:
    unordered_set<string> mappingTable;   // Hash set to avoid duplicates
    int numOfPes;
    int numOfTasks;
    int numOfMappings;
    const char* fname;

    bool   WriteMappingToFile(string mapping);
    string RandomShuffle(int numTask, int numPe );

  public:
    RandomMappingGenerator(int nocPes, int numTasks, int numberOfMapping, char *fname)
    {
        mappingTable.clear();
        this->numOfPes = nocPes;
        this->numOfTasks = numTasks;
        this->numOfMappings = numberOfMapping;
        this->fname = fname; 
    }

    ~RandomMappingGenerator()
    {
    }

    /* Member functions */
    void GeneratRandomMapping(void);
};

#endif // __RANDOM_MAPPING_GENERATOR__