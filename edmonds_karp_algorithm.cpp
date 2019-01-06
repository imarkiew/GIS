#include <iostream>
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <queue>
#include <limits>
#include <cmath>
#include <ctime>
#include <chrono>
#include <sstream>

using namespace std;

// funkcja do sprawdzania czy dwie liczby zmiennoprzecinkowe są dostatecznie blisko siebie
bool is_close(double a, double b, double rel_tol = 10e-9, double abs_tol = 0.0){
    return abs(a - b) <= max(rel_tol*max(abs(a), abs(b)), abs_tol);
}

int main(int argc, char** argv) {

//    liczba wierzchołków
    int n = atoi(argv[1]);
//    ścieżka do macierzy C
    string network_path = argv[2];
//    nazwa wyjściowa
    string output_name = argv[3];
//    ścieżka do katalogu do zapisów wyników
    string output_directory = argv[4];
//    czy zapisujemy macierz przepływów netto oraz maksymalny przepływ
    stringstream ss(argv[5]);
    bool is_flow_data_saved = false;
    ss >> boolalpha >> is_flow_data_saved;
//    numer wierzchołka odpowiadający źródłu
    int s = atoi(argv[6]);
//    numer wierzchołka odpowiadający ujściu
    int t = atoi(argv[7]);

////    pierwszy przykład testowy
//    int n = 7;
//    string network_path = "./tests/test_network_1";
//    string output_name = "test_network_1_cpp";
//    string output_directory = "./tests";
//    bool is_flow_data_saved = true;
//    int s = 2;
//    int t = 4;

////    drugi przykład testowy
//    int n = 7;
//    string network_path = "./tests/test_network_2";
//    string output_name = "test_network_2_cpp";
//    string output_directory = "./tests";
//    bool is_flow_data_saved = true;
//    int s = 0;
//    int t = 6;

    //    inicjalizacja zmiennych
    double fmax = 0.0;
    queue<int> Q;
    double cp = 0;
    int x, y = 0;
    bool esc = false;
    int y_tmp = 0;
    auto C = new double * [n];
    auto F = new double * [n];
    auto P = new int [n];
    auto CFP = new double [n];

    for(int i = 0; i < n; i++){
        C[i] = new double [n];
        F[i] = new double [n];
        CFP[i] = 0.0;
        P[i] = 0;
        for(int j = 0; j < n; j++){
            C[i][j] = F[i][j] = 0.0;
        }
    }

//    wczytywanie macierzy C z pliku
    ifstream input_file(network_path);
    string line = "";
    int i = 0;
    if (input_file.good()) {
        while (getline(input_file, line)) {
            vector<string> vec;
            boost::algorithm::split(vec, line, boost::is_any_of(" "));
            for(int j = 0; j < vec.size(); j++){
                C[i][j] = stod(vec.at(j));
            }
            i++;
        }
        input_file.close();
    }else cout << "Unable to open file";

//  pomiar czasu 1)
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();

//    algorytm Edmondsa-Karpa (wyjaśniony w sprawozdaniu)
    while(true){
        fill_n(P, n, -1);
        P[s] = -2;
        CFP[s] = numeric_limits<double>::max();
        while(!Q.empty()) Q.pop();
        Q.push(s);
        esc = false;
        while(!Q.empty()){
            x = Q.front();
            Q.pop();
            for(y = 0; y < n; y++){
                cp = C[x][y] - F[x][y];
                if(!is_close(cp, 0) && P[y] == -1){
                    P[y] = x;
                    CFP[y] = min(CFP[x], cp);
                    if(y == t){
                        fmax += CFP[t];
                        y_tmp = y;
                        while(y_tmp != s){
                            x = P[y_tmp];
                            F[x][y_tmp] += CFP[t];
                            F[y_tmp][x] -= CFP[t];
                            y_tmp = x;
                        }
                        esc = true;
                        break;
                    }
                    Q.push(y);
                }
            }
            if(esc) break;
        }
        if(!esc) break;
    }

//    pomiar czasu 2)
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
//    różnica czasów
    auto duration = end - begin;
//    rzutowanie na milisekundy
    auto ms = chrono::duration_cast<chrono::milliseconds>(duration).count();
    ofstream timer;
//    zapis zmierzonego czasu do pliku
    timer.open(output_directory + string("/networks_cpp_time"), ios_base::app);
    if (timer.good()) {
        timer << n << " " << ms << endl;
        timer.close();
    }else cout << "Unable to open file";

//    ewentualny zapis macierzy przepływów netto F i maksymalnego przepływu maxflow do plików
    if(is_flow_data_saved) {
        ofstream flows(output_directory + string("/") + output_name + string("_flows"));
        if (flows.good()) {
            for (int i = 0; i < n; i++) {
                flows << F[i][0];
                for (int j = 1; j < n; j++) {
                    flows << " " << F[i][j];
                }
                flows << endl;
            }
            flows.close();
        }
        else cout << "Unable to open file";

        ofstream max_flow(output_directory + string("/") + output_name + string("_maxflow"));
        if (flows.good()) {
            max_flow << fmax << endl;
            flows.close();
        }
        else cout << "Unable to open file";
    }

//    dealokacja zmiennych dynamicznych
      for(int i = 0; i < n; i++){
        delete [] C[i];
        delete [] F[i];
      }

      delete [] C;
      delete [] F;
      delete [] P;
      delete [] CFP;
}