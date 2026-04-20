import java.io.*;
import java.util.*;

public class RecipeReorder {
    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        String line;
        while ((line = br.readLine()) != null && !line.isEmpty()) {
            int n = Integer.parseInt(line);
            br.readLine();
            String[] shuffled = new String[n];
            for (int i = 0; i < n; i++) shuffled[i] = br.readLine();
            br.readLine();
            Map<String, Integer> originalIndex = new HashMap<>();
            for (int i = 0; i < n; i++) originalIndex.put(br.readLine(), i);

            int[] indexList = new int[n];
            for (int i = 0; i < n; i++) indexList[i] = originalIndex.get(shuffled[i]);

            System.out.println(n - longestIncreasingSubsequence(indexList));
        }
    }

    static int longestIncreasingSubsequence(int[] arr) {
        int n = arr.length;
        int[] dp = new int[n];
        int len = 0;
        for (int num : arr) {
            int pos = Arrays.binarySearch(dp, 0, len, num);
            if (pos < 0) pos = -(pos + 1);
            dp[pos] = num;
            if (pos == len) len++;
        }
        return len;
    }
}