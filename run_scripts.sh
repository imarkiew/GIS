#!/usr/bin/env bash

# liczba węzłów
for i in {100..101}
do
#    liczba powtórzen
    number_of_nodes=${i}
    sink_id=$((${number_of_nodes} - 1))
    for j in {1..10}
    do
        make flow_network_generation
        ./flow_network_generation ${number_of_nodes} ./networks
        make clean_network_generation

        make edmonds_karp_algorithm
        ./edmonds_karp_algorithm ${number_of_nodes} ./networks/network_${number_of_nodes} network_${number_of_nodes}_cpp ./results true 0 ${sink_id}
        make clean_edmonds_karp_algorithm

	java -jar GIS_Edmonds_Karp_alg.jar ./edmonds_karp_algorithm ${number_of_nodes} ./networks/network_${number_of_nodes} network_${number_of_nodes} ./results false 0 ${sink_id}
    done
done
