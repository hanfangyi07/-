#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
using namespace std;
using namespace std::chrono;

long long countInversionsBIT(int* a, int n) {
    int* bit = new int[n + 2]();
    long long count = 0;
    for (int i = n - 1; i >= 0; i--) {
        int idx = a[i] - 1;
        int sum = 0;
        while (idx > 0) {
            sum += bit[idx];
            idx -= idx & -idx;
        }
        count += sum;
        idx = a[i];
        while (idx <= n) {
            bit[idx] += 1;
            idx += idx & -idx;
        }
    }
    delete[] bit;
    return count;
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
        for (int i = 0; i < n; i++) map[i] = pos[fr[i]] + 1;

        auto start = high_resolution_clock::now();
        long long inv = countInversionsBIT(map, n);
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
    cout << "Total algorithm time (BIT): " << total_time << " ms" << endl;

    delete[] my; delete[] pos; delete[] fr; delete[] map;
    return 0;
}
