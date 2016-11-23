#ifndef GENERATOR
#define GENERATOR

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h> 
#include <list>
#include <stack>
#include <fstream>

#define NUM_NODES 150

struct Node
{
	std::list<int>* neighbors;   //Node ids that are connected to this node
	int visited;				//boolean value if node has been visited in DFS
	int id;						//An identification
};

struct Graph
{
	std::list<Node*>* nodes;
};

struct Edge
{
	int source;
	int dest;
};

int DFS(struct Graph* g, int v);
void printGraph(struct Graph* g);
struct Graph* createGraph();
struct Node* getNode(struct Graph* g, int n);
void pushAllNeighbors(struct Graph* g, int n, std::stack<int>* s);

#endif

//DFS
//1  procedure DFS-iterative(G,v):
//2      let S be a stack
//3      S.push(v)
//4      while S is not empty
//5          v = S.pop()
//6          if v is not labeled as discovered:
//7              label v as discovered
//8              for all edges from v to w in G.adjacentEdges(v) do
//9                  S.push(w)