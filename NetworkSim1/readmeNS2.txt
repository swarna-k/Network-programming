The code written for the project was an NS2 simulation that compared the throughputs of different TCP connections in a Vegas and Sack TCP environment. 

CODE ARCHITECTURE

In this simulation, we set up a network of six nodes.  Arguments are accepted from the command prompt and which then decides the Flavor of the TCP connections as well as the end to end delay of the connections. Two source nodes are created that have a TCP agent connected to them were connected to a router and two receivers which were TCP sinks were connected to a second router. We then run the simulation for four hundred seconds and record the throughputs and the ratios between the throughputs of the two connections.

CODE USAGE

1. Open Terminal.
2. Enter the following command:
   ns ns2.tcl <TCP_flavor> <case_no>
3. TCP_flavor can either be Vegas or Sack.
4. case_no can either be 1, 2 or 3. This determines the RTT delay for connections.
5. Once the simulation is run, an output file is generated with the throughputs for each of     the connections as well as a ratio of the two throughputs.