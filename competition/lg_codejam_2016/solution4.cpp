#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <vector>
using namespace std;

void dijkstra_shortest_path(unordered_map<int, unordered_map<int, int>>& edges, vector<int>& length, int src)   // dijkstra shortest path (one to all)
{
    length[src] = 0;
    priority_queue <pair<int, int>> pq;
    pq.push(make_pair(src, 0));
    while (!pq.empty()) {
        int here = pq.top().first;
        int cost = pq.top().second;
        pq.pop();
        if (length[here] < cost) continue;
        for (auto i : edges[here]) {
            int there = i.first;
            int nextDist = cost + i.second;
            if (length[there] > nextDist) {
                length[there] = nextDist;
                pq.push(make_pair(there, nextDist));
            }
        }
    }
}

void make_lca(unordered_map<int, unordered_map<int, int>>& tree, int n, vector<int>& lca_tree, vector<int>& dist, vector<int>& height, int src)
{
    queue<int> q;
    q.push(src);
    lca_tree[src] = 1, dist[src] = 0, height[src] = 1;
    while (q.size() > 0) {
        int current = q.front();
        q.pop();
        for (auto i : tree[current]) {
            if (lca_tree[i.first] == 0) {
                lca_tree[i.first] = current;
                dist[i.first] = dist[current] + i.second;
                height[i.first] = height[current] + 1;
                q.push(i.first);
            }
        }
    }
}

int get_shortest_path(vector<int>& lca_tree, vector<int>& dist, vector<int>& h, int src, int end)
{
    int s1 = src, s2 = end;
    if (h[s1] > h[s2]) swap(s1, s2);
    while (h[s1] != h[s2]) s2 = lca_tree[s2];
    while (s1 != s2)       s1 = lca_tree[s1], s2 = lca_tree[s2];  // s1(s1) = LCA
    return dist[src] + dist[end] - 2 * dist[s1];
}

int main()
{
    int testcase, n, q, a, b, m;
    ifstream fin("input6.txt");
    fin >> testcase;
    while (testcase--) {
        fin >> n >> q;
        // make tree
        unordered_map<int, unordered_map<int, int>> tree;
        for (int i = 2; i <= n; i++) {
            fin >> a >> b;
            tree[i][a] = tree[a][i] = b;
        }

        // make graph (added node)
        unordered_map<int, unordered_map<int, int>> graph(tree);
        for (int i = 2; i <= n; i++) {
            if (graph[i].size() == 1) {
                fin >> m;
                if (graph[i].find(1) == graph[i].end() || graph[i][1] > m)
                    graph[i][1] = graph[1][i] = m;
            }
        }

        // make lca tree & get_shortest_path
        vector<int> lca_tree(n + 1, 0), dist(n + 1, 0), height(n + 1, 0);
        make_lca(tree, n, lca_tree, dist, height, 1);
        // get shortest length from root
        vector<int> short_len(n + 1, 10000000);
        dijkstra_shortest_path(graph, short_len, 1);
        long long fst = 0, snd = 0;
        for (int i = 0; i < q; i++) {
            fin >> a >> b;
            int prev = get_shortest_path(lca_tree, dist, height, a, b);
            int next = min(prev, short_len[a] + short_len[b]);
            fst += prev, snd += next;
        }
        cout << fst << " " << snd << endl;
    }
    fin.close();
}
