#include "bits/stdc++.h"

using namespace std;

using ll = long long;

mt19937_64 rng(42);

double dist(const pair<ll, ll>& a, const pair<ll, ll>& b) {
    ll dx = a.first - b.first;
    ll dy = a.second - b.second;
    return sqrt((double)(dx * dx + dy * dy));
}

template<typename Cin, typename Cout, typename Cerr>
void solve(Cin& cin, Cout& cout, Cerr& cerr) {
    // just some simple randomized TSP solver
    int n;
    cin >> n;
    vector<pair<ll, ll>> points(n);
    for (auto& [x, y] : points)
        cin >> x >> y;
    shuffle(points.begin(), points.end(), rng);

    auto calc = [&]() -> double {
        double res = 0;
        for (int i = 0; i < n; ++i) {
            res += dist(points[i], points[(i + 1) % n]);
        }
        return res;
    };

    double score = calc();
    const ll iters = 3e6;
    for (ll it = 0; it < iters; ++it) {
        int i = rng() % n;
        int j = rng() % n;
        if (i == j) {
            continue;
        }
        swap(points[i], points[j]);
        double new_score = calc();
        if (new_score < score) {
            score = new_score;
        } else {
            swap(points[i], points[j]);
        }
    }

    for (int i = 0; i < n; ++i)
        cout << points[i].first << ' ' << points[i].second << '\n';
}

int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false); cin.tie(nullptr);

    if (argc == 1) {
        // for manual testing/debugging normal <in >out should be more convenient
        solve(cin, cout, cerr);
    } else {
        const std::string input = argv[1];
        const std::string output = argv[2];
        const std::string log = output.substr(0, output.size() - 3) + "log";
        ifstream fin(input);
        ofstream fout(output);
        ofstream flog(log);

        // creating tens/hundreds of threads in parallel in the span of milliseconds might give some collisions
        uint64_t seed = chrono::steady_clock::now().time_since_epoch().count();
        uint64_t pw = 1;
        for (size_t i = 0; i < output.size(); ++i) {
            seed += uint64_t(output[i]) * pw;
            pw *= uint64_t(1e9 + 7);
        }
        rng.seed(seed);

        solve(fin, fout, flog);
    }

    return 0;
}
