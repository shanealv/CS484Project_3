# CS484Project_3

######## Graph Generator ########

## Building the project
The Graph Generator project can be found in the "Graph_Generator" folder.
Included in the project folder is a makefile (to build the project), and the source code for the project.
To build the project, use the following command:
	
	make

## Running the Project
To run the project, use the command

	./generator
or
	make run

The 'make run' command will also compile the code if it has not already been compiled. 
When the program finishes, there will be a file called "graph.g" that defines a graph in the format specified by the assignment.

## Implementation Details
To create a random, complete graph, the generator assigns each node a random number of edges between 1 and [number of nodes] - 1. 
It will then assign each node random neighbors using those edges with no repeats. 
The generator then uses Depth First Search to check if the graph is a complete graph. 
If it is not a complete graph, it will continue to create new graphs until a complete graph is made.


######## Network Simulator ########

## Building the project
The Network Simulator project can be found in the "Network" folder.  
Included in the project folder is a testGraph (for initial testing purposes), a makefile (to build the project), and src folder which contains the source code for the project.
To build the project, use the following command:
	
	make

## Running the Project
To run the project on the test data, you can now use:

	./NetSim testGraph.txt SEED
	or
	make test

To run the project on a generated graph file, use the command

	./NetSim FILE SEED
	for example,
	./NetSim graph.g 1234

## Notes on Project Design
The project was design to abstract the nodes and the link that bind them into two seperate entities to manage packet transmission.  
The NetworkNode object represents a router and focusing on routing packets only.  
The NetworkLink object represents and entire bridge of input and output queues between routers.  
A Dispatcher object manages requests for moving packets through the system.  
Nodes and links make requests to the Dispatcher to queue up events to execute with a given delay.
The current time is managed by the dispatcher and is used to assign an absolute time for the queued event to occur.  

Events are stored in a priority queue where the priority is the absolute time for an event to occur.
Packets are generated at each source node forwarded using a routing table that is calculated using floyd warshall algorithm. 
The first time step is dedicated for each source node to generate their first packet to send to their corresponding destination. 
Packets are then queued for processing.  When those events are later resolved, they are pushed to the input queue on the appropriate link.
Every time set, all queues for all links in the system process the next available packet (assuming they are not propagating and transmitting).
For input queues, they use the Dispatcher to queue the transmission to the complementing output queue.  
When the output queues process packets, they forward them to the appriate node for routing.  
This cycle is repeated until the packet reaches its destination.

Throughout this process, a static Packet object maintains the total number of packets that were created and the number that arrived at their destination.
Each node also keeps track of how many packets were dropped due to input/output queues reaching their maximum size.
A static Statistic object is used to keep track of each packet that is created and recieved in each flow.
A static RandomGen object is used as a utility for all distribution generation calculations.

networknode_getlink(NetworkNode* node);

