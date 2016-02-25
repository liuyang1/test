import java.util.Arrays;
import java.util.ArrayList;
import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;
public class FastCollinearPoints {
    private LineSegment[] mSegs;
    public FastCollinearPoints(Point[] points) {
        precheck(points);
        // System.out.printf("FastCollinearPoints length=%d\n", points.length);
        Arrays.sort(points);
        // XXX: improve this ugly code
        ArrayList lst = new ArrayList();
        for (int p = 0; p != points.length; p++) {
            for (int q = p + 1; q != points.length; q++) {
                for (int r = q + 1; r != points.length; r++) {
                    for (int s = r + 1; s != points.length; s++) {
                        // System.out.printf("check %d %d %d %d\n", p, q, r, s);
                        if (isCollinear(points[p], points[q], points[r],
                                    points[s])) {
                            // System.out.printf("%s %s %s %s collinear\n",
                            //         points[p], points[q], points[r], points[s]);
                            LineSegment seg = collinear(points[p], points[q],
                                    points[r], points[s]);
                            lst.add(seg);
                        }
                    }
                }
            }
        }
        mSegs = new LineSegment[lst.size()];
        lst.toArray(mSegs);
    }
    private void precheck(Point[] points) {
        for (Point pt: points) {
            if (pt == null) {
                throw new java.lang.NullPointerException();
            }
        }
        for (int i = 0; i != points.length; i++) {
            for (int j = i + 1; j != points.length; j++) {
                if (points[i].compareTo(points[j]) == 0) {
                    throw new java.lang.IllegalArgumentException();
                }
            }
        }
    }
    private LineSegment collinear(Point p, Point q, Point r, Point s) {
        Point[] points = new Point[4];
        points[0] = p;
        points[1] = q;
        points[2] = r;
        points[3] = s;
        Arrays.sort(points);
        return new LineSegment(points[0], points[3]);
    }
    private boolean isCollinear(Point p, Point q, Point r, Point s) {
        double slope0 = p.slopeTo(q);
        double slope1 = p.slopeTo(r);
        double slope2 = p.slopeTo(s);
        return slope0 == slope1 && slope0 == slope2;
    }
    public int numberOfSegments() {
        return mSegs.length;
    }
    public LineSegment[] segments()
    {
        // Don't directly return reference of internal var
        return mSegs.clone();
    }
    public static boolean testOneFCP(Point[] points) {
        FastCollinearPoints fcp = new FastCollinearPoints(points);
        LineSegment[] segs = fcp.segments();
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
        testOneFCP(points);
        points[3] = new Point(1, -1);
        System.out.printf("negative case\n");
        testOneFCP(points);
        return true;
    }
    public static boolean nullExceptTest() {
        Point[] points = new Point[1];
        points[0] = null;
        try {
            // dead store
            FastCollinearPoints fcp = new FastCollinearPoints(points);
        } catch (java.lang.NullPointerException nullExcept) {
            return true;
        }
        return false;
    }
    public static boolean repeatExceptTest() {
        Point[] points = new Point[2];
        points[0] = new Point(1, 1);
        points[1] = new Point(1, 1);
        try {
            FastCollinearPoints fcp = new FastCollinearPoints(points);
        } catch (java.lang.IllegalArgumentException except) {
            return true;
        }
        return false;
    }
    public static boolean exceptTest() {
        if (!nullExceptTest()) {
            System.out.printf("nullExceptTest fail\n");
            return false;
        }
        if (!repeatExceptTest()) {
            System.out.printf("repeatExceptTest fail\n");
            return false;
        }
        return true;
    }
    public static void caseTest() {
        basicTest();
        exceptTest();
    }
    public static void sampleTest(String filename) {
        In in = new In(filename);
        int N = in.readInt();
        Point[] points = new Point[N];
        for (int i = 0; i != N; i++) {
            int x = in.readInt();
            int y = in.readInt();
            points[i] = new Point(x, y);
        }
        StdDraw.show(0);
        StdDraw.setXscale(0, 32768);
        StdDraw.setYscale(0, 32768);
        for (Point p: points) {
            p.draw();
        }
        StdDraw.show();
        FastCollinearPoints collinear = new FastCollinearPoints(points);
        for (LineSegment seg: collinear.segments()) {
            StdOut.println(seg);
            seg.draw();
        }
    }
    public static void main(String[] args) {
        sampleTest(args[0]);
    }
}

