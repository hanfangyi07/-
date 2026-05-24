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
    int n;
    in >> n;
    int* myRank = new int[n];
    for (int i = 0; i < n; i++) in >> myRank[i];
    int* pos = new int[n + 1];
    for (int i = 0; i < n; i++) pos[myRank[i]] = i;
    int* friendRank = new int[n];
    int* mapped = new int[n];
    int bestFriend = -1;
    double bestSim = -1.0;
    int friendId = 1;
    long long total_algo_time = 0;   // 耗时
    while (in >> friendRank[0]) {
        for (int i = 1; i < n; i++) in >> friendRank[i];
        for (int i = 0; i < n; i++) mapped[i] = pos[friendRank[i]] + 1;
        auto start = high_resolution_clock::now();
        long long inv = countInversionsBIT(mapped, n);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        total_algo_time += duration.count();
        long long maxInv = (long long)n * (n - 1) / 2;
        double sim = (1.0 - (double)inv / maxInv) * 100.0;
        if (sim > bestSim) {
            bestSim = sim;
            bestFriend = friendId;
        }
        friendId++;
    }

cout << "Best friend: friend " << bestFriend << ", similarity = " << fixed << setprecision(2) << bestSim << "%" << endl;
cout << "Total algorithm time (BIT): " << total_algo_time << " ms" << endl;

    delete[] myRank; delete[] pos; delete[] friendRank; delete[] mapped;
    return 0;
}
