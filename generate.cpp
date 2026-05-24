#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

// 生成一个 1..n 的随机排列
void randomPermutation(int arr[], int n) {
    // 初始化为 1,2,3,...,n
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
    // 从后往前随机交换
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);   // 随机选一个 0..i 的位置
        // 交换 arr[i] 和 arr[j]
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

int main() {
    srand(time(0));                 // 用当前时间作为随机种子
    int N = 50000;                    // 歌曲数量（可调整）
    int M = 20;                     // 朋友数量（可调整）

    ofstream out("rankings50k.txt");
    if (!out) {
        cerr << "Failed to open file" << endl;
        return 1;
    }

    out << N << endl;

    // 生成你的排名
    int* myRank = new int[N];
    randomPermutation(myRank, N);
    for (int i = 0; i < N; i++) {
        out << myRank[i];
        if (i != N - 1) out << " ";
    }
    out << endl;

    // 生成每个朋友的排名
    int* friendRank = new int[N];
    for (int f = 0; f < M; f++) {
        randomPermutation(friendRank, N);
        for (int i = 0; i < N; i++) {
            out << friendRank[i];
            if (i != N - 1) out << " ";
        }
        out << endl;
    }

    out.close();
 

    delete[] myRank;
    delete[] friendRank;
    return 0;
}
