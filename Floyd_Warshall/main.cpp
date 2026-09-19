#include<bits/stdc++.h>
#include<random>
#include<omp.h>
using namespace std;

#define INF 1000000000

vector<vector<int>> getRandomMatrix(int V){
    vector<vector<int>> dist(V, vector<int>(V));

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> weight(1, 100);

    for(int i = 0; i < V; i++){
        for(int j = 0; j < V; j++){
            if(i == j){
                dist[i][j] = 0;
            }else if(rand() % 4 == 0){
                dist[i][j] = INF;
            }else {
                dist[i][j] = weight(gen);
            }
        }
    }
    return dist;
}

void normalFloydWarshall(vector<vector<int>>& dist){
    int V = dist.size();

    for(int k = 0; k < V; k++){
        for(int i = 0; i < V; i++){
            for(int j = 0; j < V; j++){
                if(dist[i][k] != INF && dist[k][j] != INF){
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
}

void parallelFloydWarshall(vector<vector<int>>& dist){
    int V = dist.size();

    for(int k = 0; k < V; k++){

        #pragma omp parallel for
        for(int i = 0; i < V; i++){
            for(int j = 0; j < V; j++){
                if(dist[i][j] != INF && dist[k][j] != INF){
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
}

int main(){
    int V;
    cout << "Please enter the number of Nodes:";
    cin >> V;

    int threads;
    cout << "Number of threads between: "<< endl;
    cin >> threads;

    int maxThread = omp_get_max_threads();

    if(threads <= 0){
        threads = 1;
    }else if(threads > maxThread){
        threads = maxThread;
    }

    vector<vector<int>> dist1 = getRandomMatrix(V);
    vector<vector<int>> dist2 = dist1;

    auto start = omp_get_wtime();
    normalFloydWarshall(dist1);
    auto end = omp_get_wtime();
    cout << "Serial: "<< end - start <<" sec\n";

    omp_set_num_threads(threads);

    auto start1 = omp_get_wtime();
    parallelFloydWarshall(dist2);
    auto end1 = omp_get_wtime();
    cout << "Parallel: "<< end1 - start1 << " sec\n";


    return 0;
}