import java.io.*;
import java.lang.*;
import java.util.Arrays;
import java.util.PriorityQueue;
import java.util.Scanner;

import static java.lang.Math.min;

public class Main {

    public static boolean  is_close(double a, double b){
        double rel_tol = Math.pow(10,-9);
        double abs_tol = 0.0;
        return Math.abs(a-b) <= Math.max(rel_tol*Math.max(Math.abs(a),Math.abs(b)),abs_tol);
        }

    public static  double[][] readArray(String path) throws Exception{
        Scanner sc = new Scanner(new BufferedReader(new FileReader(path)));

        Scanner scCounter = new Scanner(new BufferedReader(new FileReader(path)));
        long count = scCounter.nextLine().chars().filter(ch -> ch == ' ').count();

        int rows = (int) count + 1;
        int columns = (int) count +1;

        double [][] myArray = new double[rows][columns];
        while(sc.hasNextLine()) {
            for (int i=0; i<myArray.length; i++) {
                String[] line = sc.nextLine().trim().split(" ");
                for (int j=0; j<line.length; j++) {
                    myArray[i][j] = Double.parseDouble(line[j]);
                }
            }
        }

        return myArray;
    }


    public static void main(String [ ] args) throws IOException {
        // liczba wierchołków
        Integer n = Integer.decode(args[1]);


        // scieżka do macierzy C
        String natwork_path = args[2];

        // nazwa wyjsciowa
        String output_name = args[3] + "java";

        // scieżka do katalogu do zapisu wyników
        String output_directory = args[4];

        // czy zapisujemy macierz przepływów netto oraz maksymalny przepływ
        String ss = args[5];

        boolean is_flow_data_saved = true;
        //is_flow_data_saved = Boolean.getBoolean(ss);

        //    numer wierzchołka odpowiadający źródłu
        Integer s = Integer.decode(args[6]);
        //    numer wierzchołka odpowiadający ujściu
        Integer t = Integer.decode(args[7]);

        //    pierwszy przykład testowy
        //    int n = 7;
        //    string network_path = "./tests/test_network_1";
        //    string output_name = "test_network_1_cpp";
        //    string output_directory = "./tests";
        //    bool is_flow_data_saved = true;
        //    int s = 2;
        //    int t = 4;

        //    drugi przykład testowy
        //    int n = 7;
        //    string network_path = "./tests/test_network_2";
        //    string output_name = "test_network_2_cpp";
        //    string output_directory = "./tests";
        //    bool is_flow_data_saved = true;
        //    int s = 0;
        //    int t = 6;

        //inicjalizacja zmiennych
        double fmax = 0.0;

        PriorityQueue<Integer> Q = new PriorityQueue<Integer>();
        double cp = 0;
        int x,y =0;
        boolean esc = false;
        int y_tmp = 0;

        double[][] C = new double [n][n];
        double[][] F = new double [n][n];
        int[] P = new int [n];
        double[] CFP = new double [n];

        for(int i = 0; i < n; i++){
            CFP[i] = 0.0;
            P[i] = 0;
            for(int j = 0; j < n; j++){
                C[i][j] = F[i][j] = 0.0;
            }
        }

        try {
            C = readArray(natwork_path);
        } catch (Exception e) {
            e.printStackTrace();
        }

        long start = System.currentTimeMillis();

        while(true){
            Arrays.fill(P,-1);

            P[s] = -2;
            CFP[s] = Double.MAX_VALUE;
            while(!Q.isEmpty()) Q.poll();
            Q.add(s);
            esc = false;
            while(!Q.isEmpty()){
                x = Q.peek();
                Q.poll();
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
                        Q.add(y);
                    }
                }
                if(esc) break;
            }
            if(!esc) break;
        }


        long stop = System.currentTimeMillis() ;

        long duration = stop - start;


        FileWriter fstream = new FileWriter(output_directory + "/" + "networks_java" + "_time.txt",true);
        BufferedWriter out = new BufferedWriter(fstream);
        out.append(n + " " + duration);
        out.newLine();
        out.close();

        if(is_flow_data_saved){

            try (PrintWriter out_flows = new PrintWriter(  output_directory + "/" + output_name + "_flows")) {
                for (int i = 0; i < n; i++) {
                    out_flows.print(F[i][0] + " ");
                    for (int j = 1; j < n; j++) {
                        out_flows.print(F[i][j] + " ");
                    }
                    out_flows.println();
                }
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            }



            try (PrintWriter out_maxflow = new PrintWriter(  output_directory + "/" + output_name + "_maxflow")) {
                out_maxflow.println(fmax);
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            }
        }
    }
}
