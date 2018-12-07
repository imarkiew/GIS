CXX=g++
CXXFLAGS=-std=c++11

flow_network_generation.o: flow_network_generation.cpp
	$(CXX) $(CXXFLAGS) -c flow_network_generation.cpp

flow_network_generation: flow_network_generation.o
	$(CXX) $(CXXFLAGS) flow_network_generation.o -o flow_network_generation

clean_network_generation:
	rm *.o flow_network_generation

edmonds_karp_algorithm.o: edmonds_karp_algorithm.cpp
	$(CXX) $(CXXFLAGS) -c edmonds_karp_algorithm.cpp

edmonds_karp_algorithm: edmonds_karp_algorithm.o
	$(CXX) $(CXXFLAGS) edmonds_karp_algorithm.o -o edmonds_karp_algorithm

clean_edmonds_karp_algorithm:
	rm *.o edmonds_karp_algorithm