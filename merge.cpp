#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
using namespace std;
using namespace std::chrono;

// 归并排序统计逆序对（原函数不变）
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
        }
        else {
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
    if (!in) { cerr << "Cannot open rankings.txt" << endl; return 1; }
    int n;
    in >> n;

    // 读取你的排名
    int* myRank = new int[n];
    for (int i = 0; i < n; i++) in >> myRank[i];

    // 建立映射：歌曲 -> 在你心中的位置（0-based）
    int* pos = new int[n + 1];
    for (int i = 0; i < n; i++) pos[myRank[i]] = i;

    int* friendRank = new int[n];
    int* mapped = new int[n];
    int bestFriend = -1;
    double bestSim = -1.0;
    int friendId = 1;

    // 总计时（只累计 Merge 调用的时间）
    long long total_ms = 0;

    while (in >> friendRank[0]) {
        for (int i = 1; i < n; i++) in >> friendRank[i];
        // 映射到你的顺序（转换为 1..n 的排列）
        for (int i = 0; i < n; i++) mapped[i] = pos[friendRank[i]] + 1;

        auto start = high_resolution_clock::now();
        long long inv = Merge(mapped, n);
        auto end = high_resolution_clock::now();
        total_ms += duration_cast<milliseconds>(end - start).count();

        long long maxInv = (long long)n * (n - 1) / 2;
        double sim = (1.0 - (double)inv / maxInv) * 100.0;

        if (sim > bestSim) {
            bestSim = sim;
            bestFriend = friendId;
        }
        friendId++;
    }

    if (bestFriend != -1) {
        cout << "Your most similar friend is: Friend " << bestFriend
            << ", similarity " << fixed << setprecision(2) << bestSim << "%" << endl;
    }
    else {
        cout << "No friend data found" << endl;
    }
    cout << "Total time for Merge algorithm: " << total_ms << " ms" << endl;

    delete[] myRank; delete[] pos; delete[] friendRank; delete[] mapped;
    return 0;
}