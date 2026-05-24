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
    if (!in) {
        cerr << "Cannot open rankings50k.txt" << endl;
        return 1;
    }

    int n;
    in >> n;

    int* my = new int[n];
    for (int i = 0; i < n; i++) in >> my[i];

    int* pos = new int[n + 1];
    for (int i = 0; i < n; i++) pos[my[i]] = i;

    int* fr = new int[n];
    int* map = new int[n];
    int bestId = -1;
    double bestSim = -1.0;
    int fid = 1;

    long long total_time = 0;

    while (in >> fr[0]) {
        for (int i = 1; i < n; i++) in >> fr[i];
        // 映射到你的顺序（转换为 1..n 的排列）
        for (int i = 0; i < n; i++) map[i] = pos[fr[i]] + 1;

        auto start = high_resolution_clock::now();
        long long inv = countInv(map, n);
        auto end = high_resolution_clock::now();
        total_time += duration_cast<milliseconds>(end - start).count();

        long long maxInv = (long long)n * (n - 1) / 2;
        double sim = (1.0 - (double)inv / maxInv) * 100.0;

        if (sim > bestSim) {
            bestSim = sim;
            bestId = fid;
        }
        fid++;
    }

    if (bestId != -1) {
        cout << "Most similar friend: Friend " << bestId
             << ", similarity = " << fixed << setprecision(2) << bestSim << "%" << endl;
    } else {
        cout << "No friend data found." << endl;
    }
    cout << "Total algorithm time (brute force): " << total_time << " ms" << endl;

    delete[] my; delete[] pos; delete[] fr; delete[] map;
    return 0;
}
