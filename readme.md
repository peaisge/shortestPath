A makefile is used to compile all programs, either individually or at once with the make command.

dijkstra

Naive version of Dijkstra's algorithm as a sequential code
To run the script with one of the rscp files, type "./dijkstra rcspX.txt"
    NB: If you do not provide the argument rscp.txt, the script runs on a default graph that we created in the file Graph.cpp
        This is true for all the scripts.
    
dijkstra_opti
Optimized (in terms of complexity) sequential version of Dijkstra's algorithm using heaps as a sequential code
To run the script, type "./dijkstra_opti rcspX.txt"

dijkstra_opti_p
Optimized sequential version of Dijkstra's algorithm using heaps as a parallelised code
To run the script on N cores, type "mpirun -np N ./dijkstra_opti_p rcspX.txt"

shortest_constraints
Method for finding the shortest path under constraints as a sequential code
To run the script, type "./shortest_constraints rcspX.txt"

shortest_constraints_p
Method for finding the shortest path under constraints as a parallelised code
To run the script on N cores, type "mpirun -np N ./dijkstra_opti_p rcspX.txt"
    NB: Do not chose more cores than vertices in the graph (4 vertices in the default graph) 

k_shortest
Method for finding the k shortest paths as a sequential code
To run the script and print the j shortest paths, type "./k_shortest j rcspX.txt"
    NB: Do not chose j excessively large: the scripts does not handle the case where 
    there are less than j paths between the vertices s and t

k_shortest_p
Method for finding the k shortest paths as a parallelised code
To run the script on N cores and print the j shortest paths, type "mpirun -np N ./k_shortest_p j rcspX.txt"
