#include "bits/stdc++.h"

using namespace std;

mt19937_64 rng(42);
long long rnd(long long l, long long r) {
    return (long long)(rng() % (r - l + 1)) + l;
}

int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    rng.seed(stoi(argv[1]));
    int n = stoi(argv[2]);

    int C = 1000;
    cout << n << '\n';
    for (int i = 0; i < n; ++i) {
        cout << rnd(-C, C) << ' ' << rnd(-C, C) << '\n';
    }

    return 0;
}
