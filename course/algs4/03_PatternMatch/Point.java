/******************************************************************************
 *  Compilation:  javac Point.java
 *  Execution:    java Point
 *  Dependencies: none
 *  
 *  An immutable data type for points in the plane.
 *  For use on Coursera, Algorithms Part I programming assignment.
 *
 ******************************************************************************/

import java.util.Comparator;
import java.util.Arrays;
import edu.princeton.cs.algs4.StdDraw;

public class Point implements Comparable<Point> {

    private final int x;     // x-coordinate of this point
    private final int y;     // y-coordinate of this point

    /**
     * Initializes a new point.
     *
     * @param  x the <em>x</em>-coordinate of the point
     * @param  y the <em>y</em>-coordinate of the point
     */
    public Point(int x, int y) {
        /* DO NOT MODIFY */
        this.x = x;
        this.y = y;
    }

    /**
     * Draws this point to standard draw.
     */
    public void draw() {
        /* DO NOT MODIFY */
        StdDraw.point(x, y);
    }

    /**
     * Draws the line segment between this point and the specified point
     * to standard draw.
     *
     * @param that the other point
     */
    public void drawTo(Point that) {
        /* DO NOT MODIFY */
        StdDraw.line(this.x, this.y, that.x, that.y);
    }

    /**
     * Returns the slope between this point and the specified point.
     * Formally, if the two points are (x0, y0) and (x1, y1), then the slope
     * is (y1 - y0) / (x1 - x0). For completeness, the slope is defined to be
     * +0.0 if the line segment connecting the two points is horizontal;
     * Double.POSITIVE_INFINITY if the line segment is vertical;
     * and Double.NEGATIVE_INFINITY if (x0, y0) and (x1, y1) are equal.
     *
     * @param  that the other point
     * @return the slope between this point and the specified point
     */
    public double slopeTo(Point that) {
        if (x == that.x) {
            if (that.y != y) {
                return Double.POSITIVE_INFINITY;
            } else {
                return Double.NEGATIVE_INFINITY;
            }
        } else {
            return (that.y - y) / (that.x - x + 0.0);
        }
    }

    /**
     * Compares two points by y-coordinate, breaking ties by x-coordinate.
     * Formally, the invoking point (x0, y0) is less than the argument point
     * (x1, y1) if and only if either y0 < y1 or if y0 = y1 and x0 < x1.
     *
     * @param  that the other point
     * @return the value <tt>0</tt> if this point is equal to the argument
     *         point (x0 = x1 and y0 = y1);
     *         a negative integer if this point is less than the argument
     *         point; and a positive integer if this point is greater than the
     *         argument point
     */
    public int compareTo(Point that) {
        if (that.y != y) {
            return Integer.compare(y, that.y);
        } else {
            return Integer.compare(x, that.x);
        }
    }

    /**
     * Compares two points by the slope they make with this point.
     * The slope is defined as in the slopeTo() method.
     *
     * @return the Comparator that defines this ordering on points
     */
    public Comparator<Point> slopeOrder() {
        return new Comparator<Point>() {
            public int compare(Point p0, Point p1) {
                return Double.compare(slopeTo(p0), slopeTo(p1));
            }
        };
    }


    /**
     * Returns a string representation of this point.
     * This method is provide for debugging;
     * your program should not rely on the format of the string representation.
     *
     * @return a string representation of this point
     */
    public String toString() {
        /* DO NOT MODIFY */
        return "(" + x + ", " + y + ")";
    }

    public static boolean testOneSlopeTo(Point p0, Point p1, double expect) {
        double slope = p0.slopeTo(p1);
        System.out.printf("slopeTo(%s, %s) => %f ?= %f %s\n",
                p0, p1, slope, expect, slope == expect);
        return slope == expect;
    }
    public static boolean testSlopeTo() {
        Point origin = new Point(0, 0);
        Point p0 = new Point(1, 1);
        Point p1 = new Point(1, 2);
        if (!testOneSlopeTo(origin, p0, 1.0)) { return false; }
        if (!testOneSlopeTo(p0, origin, 1.0)) { return false; }
        if (!testOneSlopeTo(origin, p1, 2.0)) { return false; }
        if (!testOneSlopeTo(p0, p1, Double.POSITIVE_INFINITY)) { return false; }
        if (!testOneSlopeTo(p0, p0, Double.NEGATIVE_INFINITY)) { return false; }
        return true;
    }
    public static boolean testOneCompareTo(Point p0, Point p1, int expect) {
        int cmp = p0.compareTo(p1);
        System.out.printf("compareTo(%s, %s) => %d ?= %d %s\n",
                p0, p1, cmp, expect, cmp == expect);
        int cmpRev = p1.compareTo(p0);
        System.out.printf("compareTo(%s, %s) => %d ?= %d %s\n",
                p1, p0, cmpRev, -expect, cmpRev == -expect);
        return cmp == expect && cmpRev == -expect;
    }
    public static boolean testCompareTo() {
        Point origin = new Point(0, 0);
        Point p1 = new Point(1, 1);
        Point p2 = new Point(1, 2);
        testOneCompareTo(origin, origin, 0);
        testOneCompareTo(origin, p1, -1);
        testOneCompareTo(p2, p1, 1);
        return true;

    }
    public static void showPoints(Point []points) {
        System.out.printf("[");
        boolean start = true;
        for (Point pt: points) {
            if (!start) {
                System.out.printf(", ");
            } else {
                start = false;
            }
            System.out.printf("%s", pt);
        }
        System.out.printf("]\n");
    }
    public static boolean testComparator() {
        final int sz = 6;
        Point[] points = new Point[sz];
        points[0] = new Point(0, 3);
        points[1] = new Point(1, 2);
        points[2] = new Point(1, 1);
        points[3] = new Point(0, 0);
        points[4] = new Point(-1, 2);
        points[5] = new Point(2, -1);
        System.out.printf("origin list:\n");
        showPoints(points);
        System.out.printf("sort by y then x\n");
        Arrays.sort(points);
        showPoints(points);
        Point center = new Point(0, 0);
        System.out.printf("sort by slopeOrder with centeral %s\n", center);
        Arrays.sort(points, center.slopeOrder());
        showPoints(points);
        center = new Point(0, 1);
        System.out.printf("sort by slopeOrder with centeral %s\n", center);
        Arrays.sort(points, center.slopeOrder());
        showPoints(points);
        return true;
    }
    public static boolean basicTest() {
        testSlopeTo();
        testCompareTo();
        testComparator();
        return true;
    }
    /**
     * Unit tests the Point data type.
     */
    public static void main(String[] args) {
        basicTest();
    }
}
