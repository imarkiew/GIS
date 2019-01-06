CXX=g++
CXXFLAGS=-std=c++11

flow_network_generation.o: flow_network_generation.cpp
	$(CXX) $(CXXFLAGS) -c flow_network_generation.cpp

flow_network_generation: flow_network_generation.o
	$(CXX) $(CXXFLAGS) flow_network_generation.o -o flow_network_generation

edmonds_karp_algorithm.o: edmonds_karp_algorithm.cpp
	$(CXX) -O3 $(CXXFLAGS) -c edmonds_karp_algorithm.cpp

edmonds_karp_algorithm: edmonds_karp_algorithm.o
	$(CXX) -O3 $(CXXFLAGS) edmonds_karp_algorithm.o -o edmonds_karp_algorithm

clean:
	rm *.o flow_network_generation edmonds_karp_algorithm