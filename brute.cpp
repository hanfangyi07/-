#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip> 
using namespace std;
using namespace std::chrono;

long long countInv(int* a, int n) {
    long long cnt = 0;
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            if (a[i] > a[j]) ++cnt;
    return cnt;
}

int main() {
    ifstream in("rankings50k.txt");
    int n;
    in >> n;

    int* my = new int[n];
    for (int i = 0; i < n; ++i) in >> my[i];
    int* pos = new int[n + 1];
    for (int i = 0; i < n; ++i) pos[my[i]] = i;

    int* fr = new int[n];
    int* map = new int[n];
    int bestId = -1;
    double bestSim = -1.0;
    int fid = 1;

    long long total_algo_time = 0;   // 累加核心算法耗时（毫秒）

    while (in >> fr[0]) {
        for (int i = 1; i < n; ++i) in >> fr[i];
        for (int i = 0; i < n; ++i) map[i] = pos[fr[i]];

        auto start = high_resolution_clock::now();
        long long inv = countInv(map, n);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        total_algo_time += duration.count();

        double sim = (1.0 - (double)inv / (n * (n - 1) / 2.0)) * 100.0;

        if (sim > bestSim) {
            bestSim = sim;
            bestId = fid;
        }
        ++fid;
    }

    if (bestId != -1) {
        cout << "Best friend: friend " << bestId
            << ", similarity = " << fixed << setprecision(2) << bestSim << "%" << endl;
    }
    else {
        cout << "No friend data found." << endl;
    }
    cout << "Total algorithm time (brute force): " << total_algo_time << " ms" << endl;

    delete[] my; delete[] pos; delete[] fr; delete[] map;
    return 0;
}