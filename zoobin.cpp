#include <bits/stdc++.h>
using namespace std;

vector<int> compose_perm(const vector<int>& p1, const vector<int>& p2, int size) {
    vector<int> res(size + 1);
    for (int i = 1; i <= size; ++i)
        res[i] = p1[p2[i]];
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int edge_count;
    if (!(cin >> edge_count)) return 0;

    vector<pair<int, int>> edges_A, edges_B;
    int max_vertex = 0;

    for (int i = 0; i < edge_count; ++i) {
        int x, y;
        cin >> x >> y;
        if (x > y) swap(x, y);
        edges_A.emplace_back(x, y);
        max_vertex = max(max_vertex, y);
    }

    for (int i = 0; i < edge_count; ++i) {
        int x, y;
        cin >> x >> y;
        if (x > y) swap(x, y);
        edges_B.emplace_back(x, y);
        max_vertex = max(max_vertex, y);
    }

    int n = max_vertex;
    vector<vector<int>> graphA(n + 1, vector<int>(n + 1));
    vector<vector<int>> graphB(n + 1, vector<int>(n + 1));

    for (auto [u, v] : edges_A) graphA[u][v] = graphA[v][u] = 1;
    for (auto [u, v] : edges_B) graphB[u][v] = graphB[v][u] = 1;

    vector<int> degA(n + 1), degB(n + 1);
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j) {
            degA[i] += graphA[i][j];
            degB[i] += graphB[i][j];
        }

    vector<int> node_order(n);
    iota(node_order.begin(), node_order.end(), 1);
    sort(node_order.begin(), node_order.end(), [&](int x, int y) {
        return make_pair(-degA[x], x) < make_pair(-degA[y], y);
    });

    vector<int> used_nodes(n + 1), perm_map(n + 1);
    function<bool(int)> dfs_iso = [&](int idx) {
        if (idx == node_order.size()) return true;
        int v = node_order[idx];
        for (int w = 1; w <= n; ++w) {
            if (used_nodes[w] || degA[v] != degB[w]) continue;
            bool valid = true;
            for (int k = 0; k < idx; ++k) {
                int u = node_order[k];
                if ((graphA[v][u] && perm_map[u] && !graphB[w][perm_map[u]]) ||
                    (!graphA[v][u] && perm_map[u] && graphB[w][perm_map[u]])) {
                    valid = false;
                    break;
                }
            }
            if (!valid) continue;
            used_nodes[w] = 1;
            perm_map[v] = w;
            if (dfs_iso(idx + 1)) return true;
            used_nodes[w] = 0;
            perm_map[v] = 0;
        }
        return false;
    };

    dfs_iso(0);

    vector<int> inverse_perm(n + 1);
    for (int i = 1; i <= n; ++i)
        if (perm_map[i]) inverse_perm[perm_map[i]] = i;

    vector<vector<int>> adj_list(n + 1);
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j)
            if (graphA[i][j]) adj_list[i].push_back(j);

    vector<vector<int>> all_cycles;
    vector<int> visited(n + 1), path;

    function<void(int, int)> dfs_cycle = [&](int start, int u) {
        for (int v : adj_list[u]) {
            if (v < start) continue;
            if (v == start && path.size() >= 3) {
                all_cycles.push_back(path);
                continue;
            }
            if (!visited[v] && v > start) {
                visited[v] = 1;
                path.push_back(v);
                dfs_cycle(start, v);
                path.pop_back();
                visited[v] = 0;
            }
        }
    };

    for (int s = 1; s <= n; ++s) {
        fill(visited.begin(), visited.end(), 0);
        path = {s};
        visited[s] = 1;
        dfs_cycle(s, s);
    }

    vector<vector<int>> generators;
    vector<int> identity(n + 1);
    iota(identity.begin(), identity.end(), 0);

    for (const auto& cycle : all_cycles) {
        int k = cycle.size();
        if (k == 0) continue;

        vector<int> forward = identity;
        for (int i = 0; i < k; ++i)
            forward[cycle[i]] = cycle[(i + 1) % k];
        generators.push_back(forward);

        vector<int> backward = identity;
        for (int i = 0; i < k; ++i)
            backward[cycle[i]] = cycle[(i - 1 + k) % k];
        generators.push_back(backward);
    }

    if (inverse_perm == identity) {
        cout << 0;
        return 0;
    }

    queue<vector<int>> q;
    map<vector<int>, int> dist_map;
    q.push(identity);
    dist_map[identity] = 0;

    while (!q.empty()) {
        auto curr = q.front(); q.pop();
        int steps = dist_map[curr];

        for (const auto& gen : generators) {
            auto nxt = compose_perm(curr, gen, n);
            if (!dist_map.count(nxt)) {
                dist_map[nxt] = steps + 1;
                if (nxt == inverse_perm) {
                    cout << steps + 1;
                    return 0;
                }
                q.push(nxt);
            }
        }
    }

    cout << -1;
    return 0;
}