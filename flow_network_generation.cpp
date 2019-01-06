#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <random>

using namespace std;

// funkcja do wyboru liczby całkowitej z podanego zakresu, w przybliżeniu z rozkładem jednostajnym
int random_int(int min, int max){
    return min + (int)((double)rand() / ((double)RAND_MAX + 1) * (max - min + 1));
}

// funkcja wybierająca n elementów bez powtórzeń z connections, zgodnie z rozkładem jednostajnym
vector<int> random_choice_without_replacement(vector<int> connections, int n){
    vector<int> result;
    int index = 0;
    int element = 0;
    for(int i = 0; i < n; i++){
        index = random_int(0, connections.size() - 1);
        element = connections.at(index);
        result.push_back(element);
        connections.erase(connections.begin() + index);
    }
    return result;
}

int main(int argc, char** argv) {

//    liczba wierzchołków
    int n = atoi(argv[1]);
//    ścieżka do katalogu do zapisów wyników
    string output_directory = argv[2];

//    inicjalizacja zmiennych
    auto C = new double * [n];
    for(int i = 0; i < n; i++){
        C[i] = new double [n];
        for(int j = 0; j < n; j++){
            C[i][j] = 0.0;
        }
    }
    string line = "";
    srand(time(NULL));
    default_random_engine generator;
    uniform_real_distribution<double> distribution(1.0, 10.0); // funkcja generująca liczby zmiennoprzecinkowe zgodnie z rozkładem jednostajnym o nośniku [1, 10]

//    tworzymy drzewo z węzłów 0 -> 1...n - 1, gdzie 0 to źródło, a n - 1 ujście (pomijamy na końcu ujście, ze względu na założenie o braku połączeń z niego do innych węzłów)
    for(int i = 0; i < n - 1; i++){
        C[i][i + 1] = distribution(generator);
    }

//    wektor zawierający wektory z aktualnymi możliwymi jeszcze sąsiadami danego wierzchołka
    vector<vector<int>> connections;
    for(int i = 0; i < n; i++){
//    dla każdego węzła oprócz ujścia
        if(i != n - 1){
            vector<int> connections_tmp;
//            pomijamy źródło, ze względu na założenie o braku połączeń z aktualnego węzła do niego
            for(int j = 1; j < n; j++){
//                pomijamy pętle własne oraz bezpośredni następnik, ponieważ już go użyliśmy w drzewie
                if(i != j && i != j - 1){
                    connections_tmp.push_back(j);
                }
            }
            connections.push_back(connections_tmp);
        }
        else{
            vector<int> empty_vector;
            connections.push_back(empty_vector);
        }
    }

//    liczba dodatkowych połączeń
    int number_of_additional_connections = 0;
//  pomijamy na końcu ujście, ze względu na założenie o braku połączeń z niego do innych węzłów
    for(int i = 0; i < n - 1; i++){
//          wylosowana liczba dodatkowych połączeń z aktualnej możliwej, min = 0, max = wszystkie
            number_of_additional_connections = random_int(0, connections.at(i).size());
            if(number_of_additional_connections > 0){
//                wybieramy losowo bez zwracania number_of_additional_connections dodatkowych połączen
                vector<int> selected_additional_neighbors = random_choice_without_replacement(connections.at(i), number_of_additional_connections);
                vector<int>::iterator index;
                for(int j = 0; j < number_of_additional_connections; j++){
//                    szukamy indeksu aktualnie dodawanego węzła
                    index = find(connections.at(i).begin(), connections.at(i).end(), selected_additional_neighbors.at(j));
//                    usuwamy aktualnie dodawany węzeł z wektora możliwych do połączenia węzłów
                    connections.at(i).erase(index);
//                    przypisujemy losową wagę połączeniu
                    C[i][selected_additional_neighbors.at(j)] = distribution(generator);
                }
            }
    }

//    zapis wygenerowanej macierzy C do pliku
    ofstream network(output_directory + string("/network_") + to_string(n));
    if (network.good()) {
        for(int i = 0; i < n; i++){
            network << C[i][0];
            for(int j = 1; j < n; j++){
                network << " " << C[i][j];
            }
            network << endl;
        }
        network.close();
    }
    else cout << "Unable to open file";

//    dealokacja zmiennych dynamicznych
      for(int i = 0; i < n; i++){
        delete [] C[i];
      }

      delete [] C;
}