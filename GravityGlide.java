import java.io.*;
import java.util.*;

public class GravityGlide {
    static class Point {
        int x, y;
        Point(int x, int y) { this.x = x; this.y = y; }
        @Override public boolean equals(Object o) {
            if (this == o) return true;
            if (!(o instanceof Point)) return false;
            Point p = (Point) o;
            return x == p.x && y == p.y;
        }
        @Override public int hashCode() {
            return Objects.hash(x, y);
        }
    }

    static class Slide {
        Point start, end;
        int dx, dy, length;
        Slide(int x1, int y1, int x2, int y2) {
            this.start = new Point(x1, y1);
            this.end = new Point(x2, y2);
            this.dx = Integer.compare(x2, x1);
            this.dy = Integer.compare(y2, y1);
            this.length = Math.abs(x2 - x1);
        }
    }

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        int N = Integer.parseInt(br.readLine());
        List<Slide> slides = new ArrayList<>();
        Map<Point, Slide> slideMap = new HashMap<>();

        for (int i = 0; i < N; i++) {
            String[] parts = br.readLine().split(" ");
            int x1 = Integer.parseInt(parts[0]), y1 = Integer.parseInt(parts[1]);
            int x2 = Integer.parseInt(parts[2]), y2 = Integer.parseInt(parts[3]);
            Slide slide = new Slide(x1, y1, x2, y2);
            for (int j = 0; j <= slide.length; j++) {
                int px = slide.start.x + j * slide.dx;
                int py = slide.start.y + j * slide.dy;
                slideMap.put(new Point(px, py), slide);
            }
        }

        String[] last = br.readLine().split(" ");
        int x = Integer.parseInt(last[0]);
        int y = Integer.parseInt(last[1]);
        int energy = Integer.parseInt(last[2]);

        while (true) {
            Point current = new Point(x, y);
            if (slideMap.containsKey(current)) {
                Slide s = slideMap.get(current);
                if (energy == 0) break;
                x += s.dx;
                y += s.dy;
                energy--;
            } else {
                if (y == 0) break;
                y--;
                current = new Point(x, y);
                if (!slideMap.containsKey(current)) {
                    int unlockCost = x * y;
                    if (energy >= unlockCost) {
                        energy -= unlockCost;
                    } else {
                        y++; // undo last gravity drop
                        break;
                    }
                }
            }
            if (x == 0 || energy <= 0) break;
        }

        System.out.println(x + " " + y);
    }
}