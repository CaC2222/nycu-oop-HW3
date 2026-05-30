//********************************************
// Student Name			: ³¯¿o¸©
// Student ID			: 114550194
// Student Email Address: cys.cs14@nycu.edu.tw
//********************************************
//
//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// National Yang Ming Chiao Tung University, Taiwan
// Computer Science
// Date: 2026/02
//
//
#include <iostream>
#include "mySystem_GraphSystem.h"
#include <time.h>

using namespace std;

namespace SYS_CONSTANTS {
     int max_int = INT_MAX;
     double max_double = DBL_MAX;
};

int Param::GRAPH_MAX_NUM_NODES = 10000;
int Param::GRAPH_MAX_NUM_EDGES = 10000;

GRAPH_SYSTEM::GRAPH_SYSTEM( )
{
    mFlgAutoNodeDeletion = false;

    mFlgShowNodeDepth = false;

    //mOperation = GRAPH_OPERATION_ADD_EDGE;
    //mFlgShowNodeDepth = false;

    //mOperation = GRAPH_OPERATION::GRAPH_OPERATION_NODE_DEPTH;
    //mFlgShowNodeDepth = true;

    mOperation = GRAPH_OPERATION::GRAPH_OPERATION_SHORTEST_PATH;
    mOperationName = "SHORTEST_PATH";

    mMaxNodeDepth = 0.0;

    mStartNode = nullptr;
    mDestinationNode = nullptr;

    initMemoryPool();
    createDefaultGraph();
    //
    // modify and add your code heres
    //
    mSelectedNode = nullptr;
    mPassiveSelectedNode = nullptr;
}

void GRAPH_SYSTEM::initMemoryPool( )
{
    mMaxNumNodes = Param::GRAPH_MAX_NUM_NODES;
    mMaxNumEdges = Param::GRAPH_MAX_NUM_EDGES;

    mNodeArr_Pool = new GRAPH_NODE[mMaxNumNodes];
    mEdgeArr_Pool = new GRAPH_EDGE[mMaxNumEdges];

    mCurNumOfActiveNodes = 0;
    mCurNumOfActiveEdges = 0;
    mActiveNodeArr = new int[mMaxNumNodes];
    mActiveEdgeArr = new int[mMaxNumEdges];

    mFreeNodeArr = new int[mMaxNumNodes];
    mFreeEdgeArr = new int[mMaxNumEdges];
    //
    for ( int i = 0; i < mMaxNumNodes; ++i ) {
        mNodeArr_Pool[ i ].id = i;
    }
    for ( int i = 0; i < mMaxNumEdges; ++i ) {
        mEdgeArr_Pool[ i ].id = i;
    }
    //
    reset( );
}

void GRAPH_SYSTEM::reset( )
{
    stopAutoNodeDeletion();

    //mNumPoints_DoubleCircles = 0;

    mCurNumOfActiveNodes = 0;
    mCurNumOfActiveEdges = 0;

    mCurNumOfFreeNodes = mMaxNumNodes;
    mCurNumOfFreeEdges = mMaxNumEdges;

    for ( int i = 0; i < mCurNumOfFreeNodes; ++i ) {
        mFreeNodeArr[ i ] = i;
    }
    for ( int i = 0; i < mCurNumOfFreeEdges; ++i ) {
        mFreeEdgeArr[ i ] = i;
    }

    mPassiveSelectedNode = 0;
    mSelectedNode = 0;
    //
    mFlgAutoNodeDeletion = false;

    mFlgShowNodeDepth = false;

    mMaxNodeDepth = 0.0;

    mStartNode = nullptr;
    mDestinationNode = nullptr;
}

GRAPH_NODE *GRAPH_SYSTEM::getFreeNode( ) 
{
    if ( mCurNumOfFreeNodes == 0 ) return 0;
    --mCurNumOfFreeNodes;
    int id = mFreeNodeArr[ mCurNumOfFreeNodes ];
    GRAPH_NODE *n = &mNodeArr_Pool[ id ];
    mActiveNodeArr[ mCurNumOfActiveNodes ] = id;
    n->dynamicID = mCurNumOfActiveNodes;
    ++mCurNumOfActiveNodes;
    return n;
}

GRAPH_EDGE *GRAPH_SYSTEM::getFreeEdge( )
{
    if ( mCurNumOfFreeEdges == 0 ) return 0;
    --mCurNumOfFreeEdges;
    int id = mFreeEdgeArr[ mCurNumOfFreeEdges ];
    GRAPH_EDGE *e = &mEdgeArr_Pool[ id ];
    mActiveEdgeArr[ mCurNumOfActiveEdges ] = id;
    e->dynamicID = mCurNumOfActiveEdges;

    ++mCurNumOfActiveEdges;
    return e;
}

void GRAPH_SYSTEM::createDefaultGraph( )
{
    cout << "here"<< endl;
    reset( );

    float offset_x = 90.;
    float offset_z = 15.;

    int n_0 = addNode(offset_x + 0.0, 0.0, offset_z + 0.0 );
    //cout << "n_0:"<< n_0 << endl;

    //
    // modify and add your code heres
    //
    int n_1 = addNode(offset_x + 5.0, 0.0, offset_z + 0.0);
    int n_2 = addNode(offset_x + 0.0, 0.0, offset_z + 5.0);
    
    cout << "n_0:" << n_0 << " n_1:" << n_1 << " n_2:" << n_2 <<  endl;

    addEdge(n_0, n_1);
    addEdge(n_1, n_2);
    
}

void GRAPH_SYSTEM::createRandomGraph_DoubleCircles(int n)
{
    reset( );

    //n = 36;
    float dx = 5.0;
    float dz = 5.0;
    float r = 15; // radius
    float d = 10; // layer distance
    float offset_x = 90.;
    float offset_z = 15.;
    //
    // modify and add your code heres
    //
    float R = r + d;
    
    const float PI = 3.14159265358979323846f;

    int* innerIDs = new int[n];
    int* outerIDs = new int[n];

    for (int i = 0; i < n; ++i) {
        float angle = i * (2.0f * PI / n);
        float x = offset_x + r * cos(angle);
        float z = offset_z + r * sin(angle);
        innerIDs[i] = addNode(x, 0.0f, z);
    }
    float outer_r = r + d;
    for (int i = 0; i < n; ++i) {
        
        float angle = i * (2.0f * PI / n) + (PI / n);
        float x = offset_x + outer_r * cos(angle);
        float z = offset_z + outer_r * sin(angle);
        outerIDs[i] = addNode(x, 0.0f, z);
    }

    float max_delta_angle = acos(r / R);
    for (int i = 0; i < n; ++i) {
        float inner_angle = i * (2.0f * PI / n);

        std::vector<int> validOuterCandidates;

        for (int j = 0; j < n; ++j) {
            float outer_angle = j * (2.0f * PI / n);

            float delta_angle = fabs(outer_angle - inner_angle);
            if (delta_angle > PI) {
                delta_angle = (2.0f * PI) - delta_angle;
            }
            if (delta_angle > max_delta_angle) {
                continue;
            }
            validOuterCandidates.push_back(outerIDs[j]);
        }

        if (!validOuterCandidates.empty() && innerIDs[i] != -1) {
            int randomIndex = rand() % validOuterCandidates.size();
            int chosenOuterID = validOuterCandidates[randomIndex];

            if (chosenOuterID != -1) {
                addEdge(innerIDs[i], chosenOuterID);
            }
        }
    }

    delete[] innerIDs;
    delete[] outerIDs;
}

void GRAPH_SYSTEM::createNet_Circular( int n, int num_layers )
{
    reset( );

    float dx = 5.0;
    float dz = 5.0;
    float r = 5; // radius
    float d = 5; // layer distance 
    float offset_x = 90.;
    float offset_z = 30.;

    //
    // modify and add your code heres
    //
    const float PI = 3.14159265358979323846f;
        
    int** gridIDs = new int* [num_layers+1];
    for (int i = 0; i < num_layers+1; ++i) {
        gridIDs[i] = new int[n];
    }

    for (int layer = 0; layer < num_layers+1; ++layer) {
        float current_r = r + layer * d;

        for (int i = 0; i < n; ++i) {
            float angle = i * (2.0f * PI / n);
            float x = offset_x + current_r * cos(angle);
            float z = offset_z + current_r * sin(angle);

            gridIDs[layer][i] = addNode(x, 0.0f, z);
        }
    }
    for (int layer = 0; layer < num_layers+1; ++layer) {
        for (int i = 0; i < n; ++i) {
            int curr = gridIDs[layer][i];
            if (layer < num_layers) {
                int next = gridIDs[layer][(i + 1) % n]; 
                if (curr != -1 && next != -1) {
                    addEdge(curr, next);
                }
            }

            if (layer < num_layers) {
                int nextLayerCurr = gridIDs[layer + 1][i];
                if (curr != -1 && nextLayerCurr != -1) {
                    addEdge(curr, nextLayerCurr);
                }
            }
        }
    }

    for (int i = 0; i < num_layers+1; ++i) {
        delete[] gridIDs[i];
    }
    delete[] gridIDs;

}
void GRAPH_SYSTEM::createNet_Square(int n, int num_layers)
{
    reset();

    float dx = 5.0;
    float dz = 5.0;
    float offset_x = 5.;
    float offset_z = 5.;

    int total_dim = n + 2 * num_layers - 2;

    int** gridIDs = new int* [total_dim];
    for (int i = 0; i < total_dim; ++i) {
        gridIDs[i] = new int[total_dim];
        for (int j = 0; j < total_dim; ++j) gridIDs[i][j] = -1;
    }

    for (int row = 0; row < total_dim; ++row) {
        for (int col = 0; col < total_dim; ++col) {

            if (row < num_layers || row >= total_dim - num_layers ||
                col < num_layers || col >= total_dim - num_layers){
                float x = offset_x + col * dx;
                float z = offset_z + row * dz;
                gridIDs[row][col] = addNode(x, 0.0f, z);
            }
        }
    }

    for (int row = 0; row < total_dim; ++row) {
        for (int col = 0; col < total_dim; ++col) {
            int currID = gridIDs[row][col];
            if (currID == -1) continue;

            if (col < total_dim - 1) {
                int rightID = gridIDs[row][col + 1];
                if (rightID != -1) {
                    addEdge(currID, rightID);
                }
            }

            if (row < total_dim - 1) {
                int downID = gridIDs[row + 1][col];
                if (downID != -1) {
                    addEdge(currID, downID);
                }
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        delete[] gridIDs[i];
    }
    delete[] gridIDs;
}
void GRAPH_SYSTEM::createNet_RadicalCircular( int n ) {

    reset( );

    float offset_x = 90.0;
    float offset_z = 15.0;

    float r = 15; // radius

    //
    // modify and add your code heres
    //
    int centerID = addNode(offset_x, 0.0f, offset_z);
    int* peripheralIDs = new int[n];
    const float PI = 3.14159265358979323846f;

    for (int i = 0; i < n; ++i) {
        float angle = i * (2.0f * PI / n);
        float x = offset_x + r * cos(angle);
        float z = offset_z + r * sin(angle);

        peripheralIDs[i] = addNode(x, 0.0f, z);
    }
    for (int i = 0; i < n; ++i) {
        int currPeripheral = peripheralIDs[i];
        addEdge(centerID, currPeripheral);
    }
}

//
// return the node id
// return -1: no free node
//
int GRAPH_SYSTEM::addNode( float x, float y, float z, float r )
{
    GRAPH_NODE *g;
    g = getFreeNode( );
    //
    // modify and add your code heres
    //
    if (g == nullptr) return -1;
    g->p = vector3(x, y, z);
    g->r = r;

    g->edgeID.clear();
    g->visited = false;
    g->path_parent = nullptr;
    g->depth = SYS_CONSTANTS::max_int;
    g->path_cost = SYS_CONSTANTS::max_double;

    return g->id;
}

//
// return the edge id
// return -1: no free edge
//
int GRAPH_SYSTEM::addEdge( int nodeID_0, int nodeID_1 )
{
    GRAPH_EDGE *e;
    e = getFreeEdge( );
    //
    // modify and add your code heres
    //
    if (nodeID_0 == nodeID_1) return -1;
    if (e == nullptr) return -1;

    e->nodeID[0] = nodeID_0;
    e->nodeID[1] = nodeID_1;

    mNodeArr_Pool[nodeID_0].edgeID.push_back(e->id);
    mNodeArr_Pool[nodeID_1].edgeID.push_back(e->id);

    return e->id;
}

void GRAPH_SYSTEM::askForInput( )
{
    //
    // modify and add your code heres
    //
    cout << "GRAPH_SYSTEM" << endl;
    cout << "Key usage:" << endl;
    cout << "1: create a default graph" << endl;
    cout << "2: create a graph ..." << endl;
    cout << "3: create a graph ..." << endl;
    cout << "4: create a graph ..." << endl;
    cout << "5: create a graph ..." << endl;
    cout << "Delete: delete a selected node and all the edges incident to it" << endl;
    cout << "Spacebar: Change operation" << endl;
    cout << " " << endl;
    cout << "Use the mouse to select nodes and add edges" << endl;
    cout << "Click the left button to select/unselect or create an edge" << endl;
    cout << " " << endl;
    cout << "A selected node is highlighted as red." << endl;

}


GRAPH_NODE *GRAPH_SYSTEM::findNearestNode( double x, double z, double &cur_distance2 ) const
{
    GRAPH_NODE *n = nullptr;
    cur_distance2 = -1.0;
    //
    // modify and add your code heres
    //
    for (int i = 0; i < mCurNumOfActiveNodes; i++) {
        int nodeID = mActiveNodeArr[i];
        GRAPH_NODE* current_node = &mNodeArr_Pool[nodeID];

        double dx = current_node->p.x - x;
        double dz = current_node->p.z - z;
        double dist2 = dx * dx + dz * dz;
        if (i == 0) {
            cur_distance2 = dist2;
            n = current_node;
        }
        else if (dist2 < cur_distance2) {
            cur_distance2 = dist2;
            n = current_node;
        }
    }
    return n;
}

void GRAPH_SYSTEM::moveTo(double x, double y )
{
    if (!mSelectedNode) return;
    mSelectedNode->p.x = x;
    mSelectedNode->p.z = y;
}

//
// handle the mouse click event at position (x, z).
//
void GRAPH_SYSTEM::clickAt(double x, double z)
{
    double cur_d2;
    GRAPH_NODE *nearest_node = findNearestNode( x, z, cur_d2 );
    if (nearest_node == 0 ) {
        mSelectedNode = 0;
        return;
    }
    if ( cur_d2 > nearest_node->r* nearest_node->r ) {
        mSelectedNode = 0;
        return;

    }
    if ( mSelectedNode == nearest_node) {
        mSelectedNode = 0;
        return;
    }

    performOperation(nearest_node);

}

void GRAPH_SYSTEM::performOperation(GRAPH_NODE* node) {
    switch (mOperation) {
    case GRAPH_OPERATION::GRAPH_OPERATION_NODE_DEPTH:
        mSelectedNode = node;
        mFlgShowNodeDepth = true;
        computeDepthOfAllNodesFromSelectedNode();
        break;
    case GRAPH_OPERATION::GRAPH_OPERATION_ADD_EDGE:
        if (mSelectedNode != 0) {
            addEdge(mSelectedNode->id, node->id);
            mSelectedNode = 0;
            return;
        }
        mSelectedNode = node;
        break;
    case GRAPH_OPERATION::GRAPH_OPERATION_SHORTEST_PATH:
        if (mSelectedNode != 0) {
            mStartNode = mSelectedNode;
            mDestinationNode = node;
            computeShortestPath();
            mSelectedNode = 0;
            return;
        }
        mSelectedNode = node;
        break;
    }
}

void GRAPH_SYSTEM::deleteEdge( int edgeID )
{
    GRAPH_EDGE *e = &mEdgeArr_Pool[ edgeID ];
    int dynamicID = e->dynamicID;

    //
    // modify and add your code heres
    //
    removeEdgeFromNode(e, e->nodeID[0]);
    removeEdgeFromNode(e, e->nodeID[1]);

    mCurNumOfActiveEdges--;
    int lastActiveIndex = mCurNumOfActiveEdges;
    if (dynamicID != lastActiveIndex) {
        int lastEdgeID = mActiveEdgeArr[lastActiveIndex]; 

        mActiveEdgeArr[dynamicID] = lastEdgeID;     
        mEdgeArr_Pool[lastEdgeID].dynamicID = dynamicID; 
    }
    

    mFreeEdgeArr[mCurNumOfFreeEdges] = edgeID;
    mCurNumOfFreeEdges++;
}

void GRAPH_SYSTEM::removeEdgeFromNode( const GRAPH_EDGE *e, int nodeID )
{
    GRAPH_NODE *n = &mNodeArr_Pool[ nodeID ];
    //
    // modify and add your code heres
    //
    for (int i = 0; i < n->edgeID.size(); ++i) {
        if (n->edgeID[i] == e->id) {
            n->edgeID.erase(n->edgeID.begin() + i);
            break;
        }
    }

}
void GRAPH_SYSTEM::deleteEdgesOfNode( int nodeID )
{
    GRAPH_NODE *n  = &mNodeArr_Pool[ nodeID ];
    //
    // modify and add your code heres
    //
    while (!n->edgeID.empty()) {
        int targetEdgeID = n->edgeID[0];
        deleteEdge(targetEdgeID);
    }
}

void GRAPH_SYSTEM::deleteNode( int nodeID ) {
    if ( mCurNumOfActiveNodes <= 0 ) return;
    GRAPH_NODE *n = &mNodeArr_Pool[ nodeID ];
    //
    // modify and add your code heres
    //
    int targetDynamicID = n->dynamicID;
    deleteEdgesOfNode(nodeID);
    int lastActiveIndex = mCurNumOfActiveNodes - 1;

    if (targetDynamicID != lastActiveIndex) {
        int lastNodeID = mActiveNodeArr[lastActiveIndex]; 

        mActiveNodeArr[targetDynamicID] = lastNodeID;     
        mNodeArr_Pool[lastNodeID].dynamicID = targetDynamicID; 
    }
    mCurNumOfActiveNodes--;

    mFreeNodeArr[mCurNumOfFreeNodes] = nodeID;
    mCurNumOfFreeNodes++;
}

void GRAPH_SYSTEM::deleteSelectedNode(  ) {
    if ( mSelectedNode == 0 ) return;
    //
    // modify and add your code heres
    //
    deleteNode(mSelectedNode->id);
    mSelectedNode = 0;
}

bool GRAPH_SYSTEM::isSelectedNode( ) const
{
    return mSelectedNode != 0;
}

void GRAPH_SYSTEM::getInfoOfSelectedPoint( double &r, vector3 &p ) const
{
    if ( isSelectedNode( ) == false ) return;
    r = mSelectedNode->r;
    p = mSelectedNode->p;

}

void GRAPH_SYSTEM::changeOperation()
{
    switch (mOperation) {
    case GRAPH_OPERATION::GRAPH_OPERATION_NODE_DEPTH:
        mOperation = GRAPH_OPERATION::GRAPH_OPERATION_ADD_EDGE;
        mOperationName = "ADD_EDGE";
        break;
    case GRAPH_OPERATION::GRAPH_OPERATION_ADD_EDGE:
        mOperation = GRAPH_OPERATION::GRAPH_OPERATION_SHORTEST_PATH;
        mOperationName = "SHORTEST_PATH";
        break;
    case GRAPH_OPERATION::GRAPH_OPERATION_SHORTEST_PATH:
        mOperation = GRAPH_OPERATION::GRAPH_OPERATION_NODE_DEPTH;
        mOperationName = "NODE_DEPTH";

        break;
    }
}

void GRAPH_SYSTEM::resetDepthOfAllNodes()
{
    mMaxNodeDepth = 0;
    //
    // modify and add your code heres
    // 

    int numNodes = getNumOfNodes();
    for (int i = 0; i < numNodes; ++i) {
        int nodeID = mActiveNodeArr[i];
        GRAPH_NODE* n = &mNodeArr_Pool[nodeID];
        //
        // modify and add your code heres
        // 
        // set node's depth
        // and others if necessary
        //
        n->depth = SYS_CONSTANTS::max_int;
        n->visited = false;
    }
}



/*
* k := computeDepthOfAllNodesFromSelectedNode

* void k( Node *n, int depth ) {
	if n is null, return
	set n’s depth to depth
	for each adjacent node m of n				; note m should not be n
		if (m’s depth < depth + 1) k(m, depth+1)
}

void computeDepthOfAllNodesFromSelectedNode( ) {
	for each node n, set its depth to FLT_MAX
	k(selected_node, 0)
	update mMaxNodeDepth
}
*/

void GRAPH_SYSTEM::computeDepthOfAllNodesFromSelectedNode(GRAPH_NODE* node, int depth)
{
    if (node == 0) return;
    node->depth = depth;
    node->visited = true;
    //
    // modify and add your code heres
    //
    // for all edges incident to the node: node
        // get edge ID
        // get e = &mEdgeArr_Pool[edgeID];
        // n0 = &mNodeArr_Pool[e->nodeID[0]];
        // n1 = &mNodeArr_Pool[e->nodeID[1]];
        // determine the next node, which is set as the current node
        // update the current node's depth
        // recursively update all the nodes connected to the current node
            
    //for (int i = 0; i < node->edgeID.size(); ++i) {
        //
        // modify and add your code heres
        // 
     //}   
    
    for (int i = 0; i < node->edgeID.size(); ++i) {
        int edgeID = node->edgeID[i];
        GRAPH_EDGE* e = &mEdgeArr_Pool[edgeID];
        GRAPH_NODE* n0 = &mNodeArr_Pool[e->nodeID[0]];
        GRAPH_NODE* n1 = &mNodeArr_Pool[e->nodeID[1]];

        GRAPH_NODE* neighbor = (n0 == node) ? n1 : n0;

        int next_depth = depth + 1;

        if (!neighbor->visited || next_depth < neighbor->depth) {
            computeDepthOfAllNodesFromSelectedNode(neighbor, next_depth);
        }
    }
    
}

void GRAPH_SYSTEM::computeDepthOfAllNodesFromSelectedNode()
{
    // reset the depth of all nodes
    // if mSelectedNode is nullpute, return
    // set  mMaxNodeDepth = 0;
    // set mSelectedNode->depth as zero
    // invoke computeDepthOfAllNodesFromSelectedNode
    // finally, determine the mMaxNodeDepth
    // 
    
    //
    // modify and add your code heres
    //
    resetDepthOfAllNodes();
    if (mSelectedNode == nullptr) return;

    mMaxNodeDepth = 0;
    mSelectedNode->depth = 0;
    computeDepthOfAllNodesFromSelectedNode(mSelectedNode, 0);

    // Determine the mMaxNodeDepth
    int numNodes = getNumOfNodes();
    int currentMax = 0;

    for (int i = 0; i < numNodes; ++i) {
        int nodeID = mActiveNodeArr[i];
        GRAPH_NODE* n = &mNodeArr_Pool[nodeID];

        if (n->visited && n->depth != SYS_CONSTANTS::max_int) {
            if (n->depth > currentMax) {
                currentMax = n->depth;
            }
        }
    }
    mMaxNodeDepth = currentMax;
}

// This member function is not used.
float GRAPH_SYSTEM::getNodeDepthFromSelectedNode(int nodeIndex) const
{
    float d = 0.0;
    //
    // modify and add your code heres
    //
    const GRAPH_NODE* n = &mNodeArr_Pool[nodeIndex];
    d = static_cast<float>(n->depth);

    return d;
}

void GRAPH_SYSTEM::resetPathInformationOfAllNodes()
{
    //
    // modify and add your code heres
    // 

    int numNodes = getNumOfNodes();
    for (int i = 0; i < numNodes; ++i) {
        int nodeID = mActiveNodeArr[i];
        GRAPH_NODE* n = &mNodeArr_Pool[nodeID];
        //
        // modify and add your code heres
        //
        // set path cost of node
        // set path_parent of node
        n->path_cost = SYS_CONSTANTS::max_double; 
        n->path_parent = nullptr;                 
        n->visited = false;
    }
}

void GRAPH_SYSTEM::computeShortestPath(GRAPH_NODE *node)
{
    if (node == 0) return;
    //
    // modify and add your code heres
    //    


    // for each edge incident to node, node 
    // DO
    for (int i = 0; i < node->edgeID.size(); ++i) {
        //
        // modify and add your code heres
        //
        // get an edge
        // determine the next node. Set it as the current node.
        // compute distance d: node->p.distance(next->p);
        // if new path cost is not better, check for the other edges
        // if new path cost is better, update the node's path cost and path_parent
        // Also, invokte computeShortestPath for the current node.
        int edgeID = node->edgeID[i];
        GRAPH_EDGE* e = &mEdgeArr_Pool[edgeID];

        GRAPH_NODE* n0 = &mNodeArr_Pool[e->nodeID[0]];
        GRAPH_NODE* n1 = &mNodeArr_Pool[e->nodeID[1]];
        //
        // modify and add your code heres
        //
        GRAPH_NODE* neighbor = (n0 == node) ? n1 : n0;
        double edgeDistance = node->p.distance(neighbor->p);

        double newPathCost = node->path_cost + edgeDistance;
        if (newPathCost >= neighbor->path_cost) {
            continue;
        }
        neighbor->path_cost = newPathCost;
        neighbor->path_parent = node;
        computeShortestPath(neighbor);
    }
}

void GRAPH_SYSTEM::computeShortestPath()
{
    //
    // modify and add your code heres
    //
    // reset path information of all nodes
    // if mStartNode == nullptr || mDestinationNode == nullptr, return
    // invokte computeShortestPath with mStartNode
    //
    resetPathInformationOfAllNodes();
    if (mStartNode == nullptr || mDestinationNode == nullptr) return;

    mStartNode->path_cost = 0.0;
    mStartNode->visited = true;
    computeShortestPath(mStartNode);
}

void GRAPH_SYSTEM::handleKeyPressedEvent( unsigned char key )
{
    
    switch( key ) {
    case 127: // delete
        mFlgAutoNodeDeletion = false;
        deleteSelectedNode( );
        break;
    case '1':
        mFlgAutoNodeDeletion = false;
        createDefaultGraph( );
        mSelectedNode = 0;
        break;
    case '2':
        mFlgAutoNodeDeletion = false;
        createNet_Circular(12, 6);
        mSelectedNode = 0;

        break;
    case '3':
        mFlgAutoNodeDeletion = false;
        createNet_Square(15, 4);
        mSelectedNode = 0;

        break;
    case '4':
        mFlgAutoNodeDeletion = false;
        createNet_RadicalCircular(24);
        mSelectedNode = 0;

        break;
    case '5':
        mNumPoints_DoubleCircles = 24;
        mFlgAutoNodeDeletion = false;
        createRandomGraph_DoubleCircles(mNumPoints_DoubleCircles);
        mSelectedNode = 0;
        break;
    case '<':
        mNumPoints_DoubleCircles--;
        if (mNumPoints_DoubleCircles<3) 
            mNumPoints_DoubleCircles = 3;
        mFlgAutoNodeDeletion = false;
        createRandomGraph_DoubleCircles(mNumPoints_DoubleCircles);
        mSelectedNode = 0;
        break;
    case '>':
        mNumPoints_DoubleCircles++;
        if (mNumPoints_DoubleCircles> 36) 
            mNumPoints_DoubleCircles = 36;
        mFlgAutoNodeDeletion = false;
        createRandomGraph_DoubleCircles(mNumPoints_DoubleCircles);
        mSelectedNode = 0;
        break;

    case 'r':
        mFlgAutoNodeDeletion = false;
        mSelectedNode = 0;
        break;
    case ' ':
        changeOperation();
        break;
    case 'd':
    case 'D':
        mFlgAutoNodeDeletion = !mFlgAutoNodeDeletion;
        break;
    }
}

void GRAPH_SYSTEM::handlePassiveMouseEvent( double x, double z )
{
    double cur_d2;
    GRAPH_NODE *n = findNearestNode( x, z, cur_d2 );
    if ( n == 0 ) return;
    if ( cur_d2 > n->r*n->r ) {
        mPassiveSelectedNode = 0;
        return;
    }
    mPassiveSelectedNode = n;
}

int GRAPH_SYSTEM::getNumOfNodes( ) const
{
    return mCurNumOfActiveNodes;
}

void GRAPH_SYSTEM::getNodeInfo( int nodeIndex, double &r, vector3 &p ) const
{
    int nodeID = mActiveNodeArr[ nodeIndex ];
    GRAPH_NODE *n = &mNodeArr_Pool[ nodeID ];
    r = n->r;
    p = n->p;
}

void GRAPH_SYSTEM::getNodeInfo(int nodeIndex, double& r, vector3& p, float& depth) const
{
    int nodeID = mActiveNodeArr[nodeIndex];
    GRAPH_NODE* n = &mNodeArr_Pool[nodeID];
    r = n->r;
    p = n->p;
    depth = n->depth;
}

int GRAPH_SYSTEM::getNumOfEdges( ) const
{
    return mCurNumOfActiveEdges;
}

vector3 GRAPH_SYSTEM::getNodePositionOfEdge( int edgeIndex, int nodeIndex ) const
{
    int edgeID = mActiveEdgeArr[ edgeIndex ];
    GRAPH_EDGE *e = &mEdgeArr_Pool[ edgeID ];
    return mNodeArr_Pool[ e->nodeID[ nodeIndex ] ].p;
}

void GRAPH_SYSTEM::stopAutoNodeDeletion()
{
    mFlgAutoNodeDeletion = false;
}

//
// For every frame, update( ) function is called.
//
// 
void GRAPH_SYSTEM::update( )
{
    if (!mFlgAutoNodeDeletion) {
     
        return;
    }
    if (mCurNumOfActiveNodes<=0) {
     mFlgAutoNodeDeletion = false;
        return;
    }
    Sleep(250);

    //
    // modify and add your code
    // 
    // delete the selected node?
    // delete all the edges incident to the selected node?
    //
    if (mSelectedNode != 0) {
        deleteNode(mSelectedNode->id);
    }
    else if (mCurNumOfActiveNodes > 0) {
        int targetNodeID = mActiveNodeArr[0];
        deleteNode(targetNodeID);
    }
    mSelectedNode = 0;
    mPassiveSelectedNode = 0;
}