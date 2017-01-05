/***************************************************************************************************
 * File Name:             GraphEdge.h 
 *                        
 * Application Developer: University of California Irvine
 *                                          
 * The copyright information of this software can be found in the file COPYRIGHT. 
 *
 **************************************************************************************************/

#ifndef GRAPH_EDGE
#define COSMIC_APP_EDGE

#include "global_cfg.h"

class GraphEdge {

public:
	GraphEdge();
	~GraphEdge();

	int					GetId();
	int					SetId(int eid);
	int					GetSrcTaskId();
	int					SetSrcTaskId(int tid);
	int					GetDstTaskId();
	int					SetDstTaskId(int tid);
	double				GetMsgSize();
	int					SetMsgSize(int vol);
	int					print();
	
private:

	// basic parameters
	int                 id;                         // the id of the edge
	int                 srcTaskId;                  // the id of the source task
	int                 DstTaskId;                  // the id of the destination task
	double				msgSize;					// the worst-case message size
	
};


#endif // GRAPH_EDGE

