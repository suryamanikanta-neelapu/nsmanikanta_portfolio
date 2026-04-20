import java.io.*;
import java.util.*;

public class RecipeReorder {
    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        int N = Integer.parseInt(br.readLine());
        br.readLine();
        String[] shuffled = new String[N];
        for (int i = 0; i < N; i++) {
            shuffled[i] = br.readLine();
        }
        br.readLine();
        Map<String, Integer> originalIndex = new HashMap<>();
        for (int i = 0; i < N; i++) {
            originalIndex.put(br.readLine(), i);
        }
        int[] indexList = new int[N];
        for (int i = 0; i < N; i++) {
            indexList[i] = originalIndex.get(shuffled[i]);
        }
        int segments = 1;
        for (int i = 1; i < N; i++) {
            if (indexList[i] < indexList[i - 1]) {
                segments++;
            }
        }
        System.out.println(segments - 1);
    }
}