import sys
from collections import deque

sys.setrecursionlimit(2000)

def compose_perm(p1, p2, size):
    res = [0] * (size + 1)
    for i in range(1, size + 1):
        res[i] = p1[p2[i]]
    return res

def to_tuple_key(perm):
    return tuple(perm[1:])

def main():
    try:
        try:
            E_line = input()
            if not E_line:
                return
            edge_count = int(E_line)
        except EOFError:
            return

        edges_A = []
        edges_B = []
        max_vertex = 0

        for _ in range(edge_count):
            x, y = map(int, input().split())
            if x > y:
                x, y = y, x
            edges_A.append((x, y))
            max_vertex = max(max_vertex, y)

        for _ in range(edge_count):
            x, y = map(int, input().split())
            if x > y:
                x, y = y, x
            edges_B.append((x, y))
            max_vertex = max(max_vertex, y)

        n = max_vertex
        graphA = [[0] * (n + 1) for _ in range(n + 1)]
        graphB = [[0] * (n + 1) for _ in range(n + 1)]

        for u, v in edges_A:
            graphA[u][v] = graphA[v][u] = 1
        for u, v in edges_B:
            graphB[u][v] = graphB[v][u] = 1

        degA = [0] * (n + 1)
        degB = [0] * (n + 1)
        for i in range(1, n + 1):
            for j in range(1, n + 1):
                degA[i] += graphA[i][j]
                degB[i] += graphB[i][j]

        node_order = list(range(1, n + 1))
        node_order.sort(key=lambda x: (-degA[x], x))
        used_nodes = [0] * (n + 1)
        perm_map = [0] * (n + 1)

        def dfs_iso(idx):
            if idx == len(node_order):
                return True
            v = node_order[idx]
            for w in range(1, n + 1):
                if used_nodes[w]:
                    continue
                if degA[v] != degB[w]:
                    continue
                valid = True
                for k in range(idx):
                    u = node_order[k]
                    if graphA[v][u] and perm_map[u] != 0 and not graphB[w][perm_map[u]]:
                        valid = False
                        break
                    if not graphA[v][u] and perm_map[u] != 0 and graphB[w][perm_map[u]]:
                        valid = False
                        break
                if not valid:
                    continue
                used_nodes[w] = 1
                perm_map[v] = w
                if dfs_iso(idx + 1):
                    return True
                used_nodes[w] = 0
                perm_map[v] = 0
            return False

        dfs_iso(0)

        inverse_perm = [0] * (n + 1)
        for i in range(1, n + 1):
            if perm_map[i] != 0:
                inverse_perm[perm_map[i]] = i

        adj_list = [[] for _ in range(n + 1)]
        for i in range(1, n + 1):
            for j in range(1, n + 1):
                if graphA[i][j]:
                    adj_list[i].append(j)
                    adj_list[j].append(i)

        all_cycles = []
        visited = [0] * (n + 1)
        path = []

        def dfs_cycle(start, u):
            for v in adj_list[u]:
                if v < start:
                    continue
                if v == start and len(path) >= 3:
                    all_cycles.append(list(path))
                    continue
                if not visited[v] and v > start:
                    visited[v] = 1
                    path.append(v)
                    dfs_cycle(start, v)
                    path.pop()
                    visited[v] = 0

        for s in range(1, n + 1):
            visited = [0] * (n + 1)
            path = [s]
            visited[s] = 1
            dfs_cycle(s, s)

        generators = []
        identity = list(range(n + 1))

        for cycle in all_cycles:
            k = len(cycle)
            if k == 0:
                continue

            forward = list(identity)
            for i in range(k):
                a = cycle[i]
                b = cycle[(i + 1) % k]
                forward[a] = b
            generators.append(forward)

            backward = list(identity)
            for i in range(k):
                a = cycle[i]
                b = cycle[(i - 1 + k) % k]
                backward[a] = b
            generators.append(backward)

        identity_perm = list(range(n + 1))
        if inverse_perm == identity_perm:
            print(0, end="")
            return

        q = deque()
        dist_map = {}
        key_identity = to_tuple_key(identity_perm)
        q.append(identity_perm)
        dist_map[key_identity] = 0
        result = -1

        while q:
            curr = q.popleft()
            key_curr = to_tuple_key(curr)
            steps = dist_map[key_curr]

            for gen in generators:
                nxt = compose_perm(curr, gen, n)
                nxt_key = to_tuple_key(nxt)

                if nxt_key not in dist_map:
                    dist_map[nxt_key] = steps + 1
                    if nxt == inverse_perm:
                        result = steps + 1
                        print(result, end="")
                        return
                    q.append(nxt)
        print(result, end="")

    except EOFError:
        pass

main()