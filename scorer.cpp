#include "bits/stdc++.h"

using namespace std;

using ll = long long;

double dist(const pair<ll, ll>& a, const pair<ll, ll>& b) {
    ll dx = a.first - b.first;
    ll dy = a.second - b.second;
    return sqrt((double)(dx * dx + dy * dy));
}

int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    ifstream fin(argv[1]);
    ifstream fout(argv[2]);

    int n;
    fin >> n;
    vector<pair<long, long>> orig(n);
    for (auto& [x, y] : orig)
        fin >> x >> y;

    vector<pair<long, long>> v(n);
    for (auto& [x, y] : v)
        fout >> x >> y;

    {
        sort(orig.begin(), orig.end());
        auto vv = v;
        sort(vv.begin(), vv.end());
        if (orig != vv) {
            cerr << "The set of points in the output is incorrect" << endl;
            assert(false);
        }
    }

    double res = 0;
    for (int i = 0; i < n; ++i) {
        res += dist(v[i], v[(i + 1) % n]);
    }
    cout << fixed << setprecision(9) << res << '\n';

    return 0;
}
