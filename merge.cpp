#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
using namespace std;
using namespace std::chrono;

long long Merge(int arr[], int n) {
    if (n <= 1) return 0;
    int leftSize = n / 2;
    int rightSize = n - leftSize;
    int* left = new int[leftSize];
    int* right = new int[rightSize];
    for (int i = 0; i < leftSize; i++) left[i] = arr[i];
    for (int i = 0; i < rightSize; i++) right[i] = arr[i + leftSize];

    long long leftInv = Merge(left, leftSize);
    long long rightInv = Merge(right, rightSize);

    long long crossInv = 0;
    int i = 0, j = 0, k = 0;
    while (i < leftSize && j < rightSize) {
        if (left[i] <= right[j]) {
            arr[k++] = left[i++];
        } else {
            crossInv += (leftSize - i);
            arr[k++] = right[j++];
        }
    }
    while (i < leftSize) arr[k++] = left[i++];
    while (j < rightSize) arr[k++] = right[j++];

    delete[] left; delete[] right;
    return leftInv + rightInv + crossInv;
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
        long long inv = Merge(map, n);
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
    cout << "Total algorithm time (merge sort): " << total_time << " ms" << endl;

    delete[] my; delete[] pos; delete[] fr; delete[] map;
    return 0;
}
