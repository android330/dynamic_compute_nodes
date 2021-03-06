# dynamic_compute_nodes
This is the software componet of the Power Adaptive Dynamic Compute Nodes Project that was done during the Fall 2021 - Spring 2022 year.
To see any reference state diagrams or UML diagrams, please refer to the Project Binder which your mentor should have, or email me at android@vt.edu.
## Technical Summary
Overarching node software is controlled by a state machine that changes state based on status of surrounding nodes or available power.  The program initializes in the Idle state; in this state it waits for a node to send it data that initializes the job and transitions to the Operate state.  If 30 seconds has passed and the program has still not received data then it will transition to the Operate state with the job set to its default parameters.  In the Operate state, the node will continue job operation; the program will also send and receive data to nodes that are turning on and turning off.  Once the program has detected that it is losing power based on the status of its GPIO pin then it will transition to the DataSend state.  In this state it sends its data to other nodes in the hopes that they will continue job operation.  After the data has been sent, the program transitions to the Stop state.  In this state the job will continue to run, but it will not send and receive data from other nodes.  At this point, the program will either transition back to the Operate state if the node has detected that it has enough power over the GPIO pin, or it will fully turn off when the power coming to it stops.

Each node directly connects to adjacent nodes via TCP.  The basis for the internodal connection is a P2P communication in which each node connects to another node.  This decentralized communication was due to the constraint that any node could fail since solar power is not a reliable energy source.  The connections established in this scenario have each node connecting to every other node due to the scaled down nature of the model.  In a scaled up model, each node could only connect to adjacent nodes that have a high likelihood of receiving power at that moment in time, since there is a cyclical nature to power availability at a given node.

The job class defines an interface that can be used to instance other job types. This interface allows the state machine to easily start, stop, and update a running predefined job. The initial job was designed to run algorithms that had many repeated tasks accumulated at the end. The specific algorithm used was the Monte Carlo algorithm for calculating pi. The job class was also designed so that further iterations of this project could implement their own algorithms using this interface. In the future it would be better to have a way to store process information of any running process.

## How to Run
Program requires Wiring Pi to be installed on Raspberry pi, workaround used is to send compiled binaries to other nodes.

### To Compile
```
make
```

### To Run
```
./node.out *NodeHostName* *TermianlIP*
```

### Example
```
./node.out azimov 127.0.0.1
```

## Notice About Terminal
Terminal was an *unimplemented* aspect of this proejct, it is supposed to be run not on a raspberry pi but a windows computer that san see what is going on in each of the nodes. *Unimplemnted* because it fully works, but requires a home trusted network to be used and disabled firewall, and even then...
