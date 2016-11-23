#include "generator.h"
using namespace std;


int main(int argc, char *argv[])
{
	//Set up utility resources
	srand(time(NULL));
	struct Graph* g = createGraph();
	
	
	while(!DFS(g, 0))
	{
		cout << "incomplete graph!" << endl;
		g = createGraph();
	}
	
	printGraph(g);

	return 0;
}

//An iterative implementation of Depth-First Search
//returns 1 if graph is a connected graph. Else 0
int DFS(struct Graph* g, int v)
{
	//Utility variables
	struct Node* n;
	int visitedNodes = 0;
	stack<int> s;
	
	s.push(v);
	while(!s.empty())
	{
		v = s.top();
		s.pop();
		n = getNode(g, v);
		if(n->visited == 0)
		{
			n->visited = 1;
			visitedNodes++;
			pushAllNeighbors(g,v,&s);
		}
	}

	if(visitedNodes == NUM_NODES)
		return 1;
	return 0;
}

struct Graph* createGraph()
{
	//Utility variables
	int i, j, numNeighbors, tempNeighbor;
	int isUnique;
	struct Graph *g;
	struct Node *n;
	list<int>::iterator it;
	
	g = (struct Graph*)malloc(sizeof(struct Graph));
	g->nodes = new list<Node*>[NUM_NODES]();
	
	//Populate graph
	for(i = 0; i < NUM_NODES; i++)
	{
		//initialize general data
		n = (struct Node*)malloc(sizeof(struct Node));
		n->visited = 0;
		n->id = i;
		numNeighbors = rand() % (NUM_NODES) + 1;	//between 1 and NUM_NODES - 1 neighbors
		n->neighbors = new list<int>[numNeighbors]();
		
		//Assign edges at random
		for(j = 0; j < numNeighbors; j++)
		{
			//Generate random numbers until a unique neighbor is created
			isUnique = 0;
			do
			{
				tempNeighbor = rand() % NUM_NODES;	//between 0 and NUM_NODES, corresponds to id
				//node cannot have self as neighbor
				if(tempNeighbor == n->id)
					continue;
				//make sure there are no duplicates
				if(n->neighbors->size() > 0)
				for(it = n->neighbors->begin(); it != n->neighbors->end(); it++)
					if(*it == tempNeighbor)
						continue;
				isUnique = 1;
			}while(!isUnique);
			n->neighbors->push_back(tempNeighbor);
		}
		n->neighbors->sort();
		
		//Insert node into graph
		g->nodes->push_back(n);
	}

	return g;
}

struct Node* getNode(struct Graph* g, int v)
{
	if(v > NUM_NODES - 1 || v < 0)
	{
		cout << "Node " << v << " does not exist" << endl;
		exit(1);
	}

	//Utility variables
	list<Node*>::iterator nit;
	
	nit = g->nodes->begin();
	while((*nit)->id != v)
		nit++;
	
	return *nit;
}

void pushAllNeighbors(struct Graph* g, int n, stack<int>* s)
{
	//Utility variables
	list<Node*>::iterator nit;
	list<int>::iterator iit;
	
	for(nit = g->nodes->begin(); nit != g->nodes->end(); nit++)
	{
		if((*nit)->id == n)	//if this node is the node we are expanding, add all neighbors
		{
			for(iit = (*nit)->neighbors->begin(); iit != (*nit)->neighbors->end(); iit++)
				s->push(*iit);
		}
		else				//Otherwise, only add node if it has an edge to teh expanding node
		{
			for(iit = (*nit)->neighbors->begin(); iit != (*nit)->neighbors->end(); iit++)
			{
				if(*iit == n)
				{
					s->push((*nit)->id);
					break;
				}
			}
		}
	}
	
}

void printGraph(struct Graph* g)
{
	//Utility variables
	list<Node*>::iterator nit;
	list<int>::iterator iit;
	list<Edge*>::iterator eit;
	list<Edge*> edges;
	Edge* tempEdge;
	int edgeExists;
	ofstream output;
	
	//Get edges
	for(nit = g->nodes->begin(); nit != g->nodes->end(); nit++)
		for(iit = (*nit)->neighbors->begin(); iit != (*nit)->neighbors->end(); iit++)
		{
			edgeExists = 0;
			
			for(eit = edges.begin(); eit != edges.end(); eit++)
				if( ((*eit)->source == (*nit)->id && (*eit)->dest == *iit)
					|| ((*eit)->dest == (*nit)->id && (*eit)->source == *iit))
				{
					edgeExists = 1;
					break;
				}
			
			if(!edgeExists)
			{
				tempEdge = (struct Edge*)malloc(sizeof(struct Edge));
				tempEdge->source = (*nit)->id;
				tempEdge->dest = *iit;
				edges.push_back(tempEdge);
			}
		}
	
	
	output.open ("graph.g");
	
	output << NUM_NODES << " " << edges.size() << "\n";
	
	//Print all edges
	for(eit = edges.begin(); eit != edges.end(); eit++)
	{
		output << (*eit)->source << " " << (*eit)->dest << "\n";
	}
	
	output.close();
}
