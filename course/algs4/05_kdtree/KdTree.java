import java.util.LinkedList;
import edu.princeton.cs.algs4.Point2D;
import edu.princeton.cs.algs4.Interval1D;
import edu.princeton.cs.algs4.Interval2D;
import edu.princeton.cs.algs4.RectHV;
import edu.princeton.cs.algs4.StdDraw;

public class KdTree {
    private class Node {
        private Point2D mKey;
        private int mLevel;
        private Node mLeft = null, mRight = null;
        private int mSubCnt = 0;
        public Node(Point2D key, int level) {
            mKey = key;
            mLevel = level;
        }
        public boolean isLeftOrRight(Point2D pt) {
            if (mLevel % 2 == 0) {
                return pt.x() < mKey.x();
            } else {
                return pt.y() < mKey.y();
            }
        }
        public void insert(Point2D pt) {
            String str;
            if (isLeftOrRight(pt)) {
                str = "left";
                if (mLeft == null) {
                    mLeft = new Node(pt, mLevel + 1);
                } else {
                    mLeft.insert(pt);
                }
            } else {
                str = "right";
                if (mRight == null) {
                    mRight = new Node(pt, mLevel + 1);
                } else {
                    mRight.insert(pt);
                }
            }
            // System.out.printf("insert pt=%s node=%s %s\n", pt, mKey, str);
        }
        public void drawAll() {
            // System.out.printf("pt=%s mLevel=%d\n", mKey, mLevel);
            StdDraw.setPenRadius(1/200.);
            if (mLevel % 2 == 0) {
                StdDraw.setPenColor(StdDraw.RED);
                StdDraw.line(mKey.x(), 0, mKey.x(), 1);
            } else {
                StdDraw.setPenColor(StdDraw.BLUE);
                StdDraw.line(0, mKey.y(), 1, mKey.y());
            }
            StdDraw.setPenColor(StdDraw.BLACK);
            StdDraw.setPenRadius(1/20.);
            mKey.draw();
            if (mLeft != null) {
                mLeft.drawAll();
            }
            if (mRight != null) {
                mRight.drawAll();
            }
        }
        public boolean contains(Point2D pt) {
            if (mKey == pt) {
                return true;
            }
            if (mLeft != null && mLeft.contains(pt)) {
                return true;
            }
            if (mRight != null && mRight.contains(pt)) {
                return true;
            }
            return false;

        }
        public Iterable<Point2D> range(RectHV rect) {
            Point2D pt0 = new Point2D(rect.xmin(), rect.ymin());
            Point2D pt1 = new Point2D(rect.xmax(), rect.ymax());
            boolean b0 = isLeftOrRight(pt0);
            boolean b1 = isLeftOrRight(pt1);
            if (b0 && b1) {
                return mLeft.range(rect);
            }
            if (b0 == false && b1 == false) {
                return mRight.range(rect);
            }
            LinkedList<Point2D> lst = new LinkedList<Point2D>();
            for (Point2D pt: mLeft.range(rect)) {
                lst.add(pt);
            }
            for (Point2D pt: mRight.range(rect)) {
                lst.add(pt);
            }
            return lst;
        }
    }
    private Node mRoot;
    // construct an empty set of points
    public KdTree() {
        mRoot = null;
    }
    // is the set empty?
    public boolean isEmpty() {
        return mRoot != null;
    }
    // number of points in the set
    public int size() {
        if (isEmpty()) {
            return 0;
        } else {
            return mRoot.mSubCnt + 1;
        }
    }
    // add the point to the set (if it is not already in the set)
    public void insert(Point2D p) {
        if (mRoot == null) {
            mRoot = new Node(p, 0);
        } else {
            mRoot.insert(p);
        }
    }
    // does the set contain point p?
    public boolean contains(Point2D p) {
        if (mRoot == null) {
            return false;
        }
        return mRoot.contains(p);
    }
    // draw all points to standard draw
    public void draw() {
        if (mRoot != null) {
            mRoot.drawAll();
        }
    }
    // all points that are inside the rectangle
    public Iterable<Point2D> range(RectHV rect) {
        if (mRoot == null) {
            return null;
        }
        return mRoot.range(rect);
    }
    // a nearest neighbor in the set to point p; null if the set is empty
    static final double maxDist2 = 2 + 0.1;
    public Point2D nearest(Point2D p) {
        if (isEmpty()) {
            return null;
        }
        return null;
    }

    public static void main(String[] args) {
        return;
    }
}
