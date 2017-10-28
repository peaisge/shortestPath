all: \
	shortest_constaints opti opti_parallel shortest_constaints_p naif naif_parallel k_shortest k_shortest_p

naif: Dijkstra.cpp Graph.cpp Arc.cpp
	g++ Dijkstra.cpp Graph.cpp Arc.cpp -o dijkstra

opti: Dijkstra_opti.cpp Graph.cpp MinHeap.cpp Arc.cpp
	g++ Dijkstra_opti.cpp Graph.cpp MinHeap.cpp Arc.cpp -o dijkstra_opti

shortest_constaints: Shortest_Path_Constraints.cpp Graph.cpp MinHeap.cpp Arc.cpp
	g++ Shortest_Path_Constraints.cpp Graph.cpp MinHeap.cpp Arc.cpp -o shortest_constraints

shortest_constaints_p: Shortest_Path_Constraints.cpp Graph.cpp MinHeap.cpp Arc.cpp
	mpic++  Shortest_Path_Constraints_P.cpp Graph.cpp Arc.cpp -o shortest_constraints_p

naif_parallel: Dijkstra_p.cpp Graph.cpp Arc.cpp
	mpic++ Dijkstra_p.cpp Graph.cpp Arc.cpp -o dijkstra_p

opti_parallel: Dijkstra_opti_p.cpp Graph.cpp MinHeap.cpp Arc.cpp
	mpic++ Dijkstra_opti_p.cpp Graph.cpp MinHeap.cpp Arc.cpp -o dijkstra_opti_p

k_shortest: k_shortest_paths.cpp Dijkstra_withoutmain.cpp Graph.cpp Arc.cpp MinHeap.cpp
	g++ k_shortest_paths.cpp Dijkstra_withoutmain.cpp Graph.cpp Arc.cpp MinHeap.cpp -o k_shortest

k_shortest_p: k_shortest_paths_p.cpp Dijkstra_withoutmain.cpp Graph.cpp Arc.cpp MinHeap.cpp
	mpic++ k_shortest_paths_p.cpp Dijkstra_withoutmain.cpp Graph.cpp Arc.cpp MinHeap.cpp -o k_shortest_p

clean:
	rm shortest_constraints dijkstra_opti dijkstra_opti_p shortest_constraints_p dijkstra dijskstra_p k_shortest
