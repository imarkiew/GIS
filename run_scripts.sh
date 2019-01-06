#!/usr/bin/env bash

make flow_network_generation
make edmonds_karp_algorithm
javac EdmondsKarpAlgorithm.java

# liczba węzłów
for i in 100 250 500 750 1000 1500
do
#    liczba powtórzen
    number_of_nodes=${i}
    sink_id=$((${number_of_nodes} - 1))
    for j in {1..1000}
    do
        echo "i = ${i} | j = ${j}"

        ./flow_network_generation ${number_of_nodes} ./networks
        ./edmonds_karp_algorithm ${number_of_nodes} ./networks/network_${number_of_nodes} network_${number_of_nodes}_cpp ./results false 0 ${sink_id}

		java EdmondsKarpAlgorithm ./edmonds_karp_algorithm ${number_of_nodes} ./networks/network_${number_of_nodes} network_${number_of_nodes}_java ./results false 0 ${sink_id}
    done
done

make clean
rm EdmondsKarpAlgorithm.class