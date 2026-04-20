import java.io.*;
import java.util.*;

public class RecipeReorder {

    static int minOps(List<Integer> a) {
        int n = a.size();
        StringBuilder s = new StringBuilder();
        StringBuilder g = new StringBuilder();
        for (int i = 0; i < n; i++) s.append((char) a.get(i).intValue());
        for (int i = 0; i < n; i++) g.append((char) i);
        if (s.toString().equals(g.toString())) return 0;

        List<int[]> moves = new ArrayList<>();
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j <= n; j++)
                for (int k = 0; k <= n - (j - i); k++)
                    if (k != i) moves.add(new int[]{i, j, k});

        Deque<String> q1 = new ArrayDeque<>(), q2 = new ArrayDeque<>();
        Map<String, Integer> d1 = new HashMap<>(), d2 = new HashMap<>();
        String start = s.toString(), goal = g.toString();
        q1.add(start); d1.put(start, 0);
        q2.add(goal); d2.put(goal, 0);

        while (!q1.isEmpty() && !q2.isEmpty()) {
            if (q1.size() <= q2.size()) {
                int ans = expand(q1, d1, d2, moves);
                if (ans != -1) return ans;
            } else {
                int ans = expand(q2, d2, d1, moves);
                if (ans != -1) return ans;
            }
        }
        return 0;
    }

    static int expand(Deque<String> q, Map<String, Integer> dSelf, Map<String, Integer> dOther, List<int[]> moves) {
        int m = q.size();
        while (m-- > 0) {
            String x = q.poll();
            int dx = dSelf.get(x);
            for (int[] t : moves) {
                int i = t[0], j = t[1], k = t[2];
                String b = x.substring(i, j);
                String r = x.substring(0, i) + x.substring(j);
                String y = r.substring(0, k) + b + r.substring(k);
                if (dSelf.containsKey(y)) continue;
                int nd = dx + 1;
                if (dOther.containsKey(y)) return nd + dOther.get(y);
                dSelf.put(y, nd);
                q.add(y);
            }
        }
        return -1;
    }

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        String line;
        while ((line = br.readLine()) != null && !line.isEmpty()) {
            int n = Integer.parseInt(line);
            br.readLine();
            List<String> shuffled = new ArrayList<>();
            for (int i = 0; i < n; i++) shuffled.add(br.readLine());
            br.readLine();
            List<String> original = new ArrayList<>();
            for (int i = 0; i < n; i++) original.add(br.readLine());

            Map<String, Integer> indexMap = new HashMap<>();
            for (int i = 0; i < n; i++) indexMap.put(original.get(i), i);

            List<Integer> a = new ArrayList<>();
            for (int i = 0; i < n; i++) a.add(indexMap.get(shuffled.get(i)));

            System.out.println(minOps(a));
        }
    }
}