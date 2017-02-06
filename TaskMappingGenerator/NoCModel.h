/***************************************************************************************************
 * File Name:             NoCModel.h
 *
 * Application Developer: University of California Irvine
 *
 * The copyright information of this software can be found in the file COPYRIGHT.
 *
 **************************************************************************************************/

#ifndef NOC_MODEL
#define NOC_MODEL

class NoCModel 
{

public:
NoCModel(int numPes = 0, int numrow = 0, int numcol = 0)
{
	this->numOfPes = numPes;
	this->numRow = numrow;
	this->numCol = numcol;
}

~NoCModel() 
{

}
	
	int					GetNumPes();
	int					GetNumrow();
	int					GetNumCol();
	int					GetFlitSize();

private:
	int					numOfPes;			// total number of PEs
	int					numRow;				// number of NOC rows 
	int					numCol;				// number of NOC columns
	int					flitSize;			// number of bits in a flit
};

#endif // NOC_MODEL
