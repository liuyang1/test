import java.util.Arrays;
public class BruteCollinearPoints {
    private LineSegment[] mSegs;
    public BruteCollinearPoints(Point[] points) {
        System.out.printf("BruteCollinearPoints length=%d\n", points.length);
        Arrays.sort(points);
        for (int p = 0; p != points.length; p++) {
            for (int q = p + 1; q != points.length; q++) {
                for (int r = q + 1; r != points.length; r++) {
                    for (int s = r + 1; s != points.length; s++) {
                        System.out.printf("check %d %d %d %d\n", p, q, r, s);
                        if (isCollinear(points[p], points[q], points[r],
                                    points[s])) {
                            System.out.printf("%s %s %s %s collinear\n",
                                    points[p], points[q], points[r], points[s]);
                        }
                    }
                }
            }
        }
    }
    private boolean isCollinear(Point p, Point q, Point r, Point s) {
        double slope0 = p.slopeTo(q);
        double slope1 = p.slopeTo(r);
        double slope2 = p.slopeTo(s);
        return slope0 == slope1 && slope0 == slope2;
    }
    public int numberOfSegments() {
        return 0;
    }
    public LineSegment[] segments()
    {
        LineSegment[] segs = new LineSegment[0];
        return segs;
    }
    public static boolean testOneBCP(Point[] points) {
        BruteCollinearPoints bcp = new BruteCollinearPoints(points);
        LineSegment[] segs = bcp.segments();
        for (LineSegment seg: segs) {
            System.out.printf("seg: %s\n", seg);
        }
        return true;
    }
    public static boolean basicTest() {
        Point[] points = new Point[4];
        for (int i = 0; i != 4; i++) {
            points[i] = new Point(i, i + 1);
        }
        System.out.printf("positive case\n");
        // testOneBCP(points);
        points[3] = new Point(1, -1);
        System.out.printf("negative case\n");
        testOneBCP(points);
        return true;
    }
    public static void main(String[] args) {
        basicTest();
    }
}
