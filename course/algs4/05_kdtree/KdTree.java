import java.util.LinkedList;
import edu.princeton.cs.algs4.Point2D;
import edu.princeton.cs.algs4.Interval1D;
import edu.princeton.cs.algs4.Interval2D;
import edu.princeton.cs.algs4.RectHV;
import edu.princeton.cs.algs4.StdDraw;

public class KdTree {
    static final RectHV CANVAS = new RectHV(0, 0, 1, 1);
    private class Node {
        private Point2D mKey;
        private int mLevel;
        private RectHV mRect;
        private Node mLeft = null, mRight = null;
        private int mSubCnt = 0;
        public Node(Point2D key, int level, RectHV rect) {
            mKey = key;
            mLevel = level;
            mRect = rect;
        }
        // when horizontal, that mean horizontal split
        private boolean isHoriOrVert() {
            return mLevel % 2 == 0;
        }
        public boolean isLeftOrRight(Point2D pt) {
            if (isHoriOrVert()) {
                return pt.y() < mKey.y();
            } else {
                return pt.x() < mKey.x();
            }
        }
        private void drawSeg() {
            StdDraw.setPenRadius(1/200.);
            if (isHoriOrVert()) {
                StdDraw.setPenColor(StdDraw.BLUE);
                StdDraw.line(mRect.xmin(), mKey.y(), mRect.xmax(), mKey.y());
            } else {
                StdDraw.setPenColor(StdDraw.RED);
                StdDraw.line(mKey.x(), mRect.ymin(), mKey.x(), mRect.ymax());
            }
            StdDraw.setPenColor(StdDraw.BLACK);
            StdDraw.setPenRadius(1/50.);
            mKey.draw();
        }
        private RectHV splitRect(boolean leftOrRight) {
            if (isHoriOrVert()) {
                if (leftOrRight) {
                    return new RectHV(mRect.xmin(), mRect.ymin(), mRect.xmax(), mKey.y());
                } else {
                    return new RectHV(mRect.xmin(), mKey.y(), mRect.xmax(), mRect.ymax());
                }
            } else {
                if (leftOrRight) {
                    return new RectHV(mRect.xmin(), mRect.ymin(), mKey.x(), mRect.ymax());
                } else {
                    return new RectHV(mKey.x(), mRect.ymin(), mRect.xmax(), mRect.ymax());
                }
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
    private Node insert(Node node, Point2D pt, int level, RectHV rect) {
        if (node == null) {
            return new Node(pt, level + 1, rect);
        }
        if (node.isLeftOrRight(pt)) {
            node.mLeft = insert(node.mLeft, pt, node.mLevel, node.splitRect(true));
        } else {
            node.mRight = insert(node.mRight, pt, node.mLevel, node.splitRect(false));
        }
        return node;
    }
    // add the point to the set (if it is not already in the set)
    public void insert(Point2D pt) {
        mRoot = insert(mRoot, pt, 0, CANVAS);
    }
    // does the set contain point p?
    public boolean contains(Point2D p) {
        if (mRoot == null) {
            return false;
        }
        return mRoot.contains(p);
    }
    public void drawAll(Node node) {
        if (node == null) {
            return;
        }
        node.drawSeg();
        drawAll(node.mLeft);
        drawAll(node.mRight);
    }
    // draw all points to standard draw
    public void draw() {
        drawAll(mRoot);
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
