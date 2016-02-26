import java.util.Arrays;
import java.util.ArrayList;
import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;
public class FastCollinearPoints {
    private class Pair implements Comparable<Pair> {
        private int mIndex;
        private double mSlope;
        Pair(int index, double slope) {
            mIndex = index;
            mSlope = slope;
        }
        public int compareTo(Pair that) {
            return Double.compare(mSlope, that.mSlope);
        }
    }

    private LineSegment[] mSegs;
    public FastCollinearPoints(Point[] pts) {
        Point[] points = pts.clone();
        precheck(points);
        Arrays.sort(points);
        ArrayList<LineSegment> lst = new ArrayList<LineSegment>();
        int len = points.length;
        for (int origin = 0; origin < points.length - 3; origin++) {
            Pair[] others = new Pair[len - origin - 1];
            for (int j = origin + 1; j != points.length; j++) {
                double slope = points[origin].slopeTo(points[j]);
                others[j - origin - 1] = new Pair(j, slope);
            }
            Arrays.sort(others);
            /*
            System.out.printf("origin: %s\n", points[origin]);
            for (int j = 0; j != others.length; j++) {
                System.out.printf("%2d %2d %s %f\n", j, others[j].mIndex, points[others[j].mIndex], others[j].mSlope);
            }
            System.out.printf("\n");
            */
            int acc, j;
            for (j = 1, acc = 1; j < others.length; j++) {
                if (others[j].mSlope == others[j - 1].mSlope) {
                    acc++;
                } else {
                    if (acc >= 3) {
                        if (!isPointsInSeg(points, 0, origin - 1, points[origin], points[others[j - 1].mIndex])) {
                        LineSegment seg = collinear(
                                points[others[j - 1].mIndex],
                                points[others[j - 2].mIndex],
                                points[others[j - 3].mIndex],
                                points[origin]);
                        lst.add(seg);
                        }
                    }
                    acc = 1;
                }
            }
            if (acc >= 3 && !isPointsInSeg(points, 0, origin - 1, points[origin], points[others[j - 1].mIndex])) {
                LineSegment seg = collinear(
                        points[others[j - 1].mIndex],
                        points[others[j - 2].mIndex],
                        points[others[j - 3].mIndex],
                        points[origin]);
                lst.add(seg);
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
    private boolean isInSeg(Point p, Point beg, Point end) {
        return p.slopeTo(beg) == p.slopeTo(end);
    }
    private boolean isPointsInSeg(Point[] pts, int startIdx, int endIdx, Point beg, Point end) {
        if (endIdx < startIdx) {
            return false;
        }
        for (int i = startIdx; i <= endIdx; i++) {
            Point pt = pts[i];
            if (isInSeg(pt, beg, end)) {
                // System.out.printf("xxx %s in %s %s\n", pt, beg, end);
                return true;
            } else {
                // System.out.printf("xxx %s not in %s %s\n", pt, beg, end);
            }
        }
        return false;
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
    private static boolean testOneFCP(Point[] points) {
        FastCollinearPoints fcp = new FastCollinearPoints(points);
        LineSegment[] segs = fcp.segments();
        for (LineSegment seg: segs) {
            System.out.printf("seg: %s\n", seg);
        }
        return true;
    }
    private static boolean basicTest() {
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
    private static boolean nullExceptTest() {
        Point[] points = new Point[1];
        points[0] = null;
        try {
            testOneFCP(points);
        } catch (java.lang.NullPointerException nullExcept) {
            return true;
        }
        return false;
    }
    private static boolean repeatExceptTest() {
        Point[] points = new Point[2];
        points[0] = new Point(1, 1);
        points[1] = new Point(1, 1);
        try {
            testOneFCP(points);
        } catch (java.lang.IllegalArgumentException except) {
            return true;
        }
        return false;
    }
    private static boolean exceptTest() {
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
    private static void caseTest() {
        basicTest();
        exceptTest();
    }
    private static void sampleTest(String filename) {
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

