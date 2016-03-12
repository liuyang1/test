import java.util.LinkedList;
import edu.princeton.cs.algs4.Point2D;
import edu.princeton.cs.algs4.RectHV;
import edu.princeton.cs.algs4.StdDraw;

public class KdTree {
    private static final RectHV CANVAS = new RectHV(0, 0, 1, 1);
    private class Node {
        private Point2D mKey;
        private int mLevel;
        private RectHV mRect;
        private Node mLeft = null, mRight = null;
        public Node(Point2D key, int level, RectHV rect) {
            mKey = key;
            mLevel = level;
            mRect = rect;
        }
        // when horizontal, that mean horizontal split
        private boolean isHoriOrVert() {
            return mLevel % 2 == 0;
        }
        // not use Point2D, to reduce Point2D.x() Point2D.y() calls
        public boolean isLeftOrRight(double x, double y) {
            if (isHoriOrVert()) {
                return y < mKey.y();
            } else {
                return x < mKey.x();
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
                    return new RectHV(mRect.xmin(), mRect.ymin(),
                                      mRect.xmax(), mKey.y());
                } else {
                    return new RectHV(mRect.xmin(), mKey.y(),
                                      mRect.xmax(), mRect.ymax());
                }
            } else {
                if (leftOrRight) {
                    return new RectHV(mRect.xmin(), mRect.ymin(),
                                      mKey.x(), mRect.ymax());
                } else {
                    return new RectHV(mKey.x(), mRect.ymin(),
                                      mRect.xmax(), mRect.ymax());
                }
            }
        }
        public Iterable<Point2D> range(RectHV rect) {
            boolean b0 = isLeftOrRight(rect.xmin(), rect.ymin());
            boolean b1 = isLeftOrRight(rect.xmax(), rect.ymax());
            if (b0 && b1) {
                return mLeft.range(rect);
            }
            if ((!b0) && (!b1)) {
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
    private int mSize;
    // construct an empty set of points
    public KdTree() {
        mRoot = null;
        mSize = 0;
    }
    // is the set empty?
    public boolean isEmpty() {
        return size() == 0;
    }
    // number of points in the set
    public int size() {
        return mSize;
    }
    private Node nodeInsert(Node parent, Point2D pt, double x, double y) {
        RectHV rect;
        int level;
        Node newNode = null;
        if (parent == null) {
            rect = CANVAS;
            level = 0;
            return new Node(pt, level, rect);
        }
        level = parent.mLevel + 1;
        if (parent.isLeftOrRight(x, y)) {
            if (parent.mLeft == null) {
                rect = parent.splitRect(true);
                parent.mLeft = new Node(pt, level, rect);
            } else {
                parent.mLeft = nodeInsert(parent.mLeft, pt, x, y);
            }
        } else {
            if (parent.mRight == null) {
                rect = parent.splitRect(false);
                parent.mRight = new Node(pt, level, rect);
            } else {
                parent.mRight = nodeInsert(parent.mRight, pt, x, y);
            }
        }
        return parent;
    }
    // add the point to the set (if it is not already in the set)
    public void insert(Point2D pt) {
        // avoid insert duplicate point
        if (contains(pt)) {
            return;
        }
        mRoot = nodeInsert(mRoot, pt, pt.x(), pt.y());
        mSize++;
    }
    private boolean nodeContains(Node node, Point2D pt) {
        if (node == null) {
            return false;
        }
        if (!node.mRect.contains(pt)) {
            return false;
        }
        if (node.mKey.equals(pt)) {
            return true;
        }
        return nodeContains(node.mLeft, pt) || nodeContains(node.mRight, pt);
    }
    // does the set contain point pt?
    public boolean contains(Point2D pt) {
        return nodeContains(mRoot, pt);
    }
    private void drawAll(Node node) {
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
    private void showNode(Node node, int level) {
        for (int i = 0; i != level; i++) {
            System.out.printf("- ");
        }
        if (node == null) {
            System.out.printf("%s\n", "nil");
            return;
        }
        System.out.printf("%s\n", node.mKey);
        showNode(node.mLeft, level + 1);
        showNode(node.mRight, level + 1);
    }
    private void show() {
        showNode(mRoot, 0);
    }
    private LinkedList<Point2D> extendList(LinkedList<Point2D> l0,
                                           LinkedList<Point2D> l1) {
        if (l0 == null) {
            return l1;
        }
        if (l1 == null) {
            return l0;
        }
        for (Point2D pt: l1) {
            l0.add(pt);
        }
        return l0;
    }
    private LinkedList<Point2D> nodeRange(Node node, RectHV rect) {
        if (node == null) {
            return null;
        }
        if (!rect.intersects(node.mRect)) {
            return null;
        }
        LinkedList<Point2D> lst = null;
        if (rect.contains(node.mKey)) {
            lst = new LinkedList<Point2D>();
            lst.add(node.mKey);
        }
        lst = extendList(lst, nodeRange(node.mLeft, rect));
        lst = extendList(lst, nodeRange(node.mRight, rect));
        return lst;
    }
    // all points that are inside the rectangle
    public Iterable<Point2D> range(RectHV rect) {
        Iterable<Point2D> lst = nodeRange(mRoot, rect);
        // make sure not return null
        if (lst == null) {
            lst = new LinkedList<Point2D>();
        }
        return lst;
    }
    // a nearest neighbor in the set to point p; null if the set is empty
    public Point2D nearest(Point2D pt) {
        if (isEmpty()) {
            return null;
        }
        double thresh = 2, dist;
        Point2D near = null;
        LinkedList<Node> nodelst = new LinkedList<Node>();
        nodelst.add(mRoot);
        int cnt = 0;
        while (!nodelst.isEmpty()) {
            cnt++;
            Node node = nodelst.pop();
            dist = pt.distanceTo(node.mKey);
            // System.out.printf("pt=%s near=%s(%f) current=%s(%f)\n",
            // pt, near, thresh, node.mKey, dist);
            if (dist < thresh) {
                thresh = dist;
                near = node.mKey;
            }
            if (node.mLeft == null) {
                if (node.mRight == null) {
                    continue;
                } else {
                    // System.out.printf("only add right\n");
                    nodelst.add(node.mRight);
                }
            } else {
                if (node.mRight == null) {
                    // System.out.printf("only add left\n");
                    nodelst.add(node.mLeft);
                } else {
                    double left = node.mLeft.mRect.distanceTo(pt);
                    double right = node.mRight.mRect.distanceTo(pt);
                    if (left < thresh && right < thresh) {
                        // System.out.printf("add left=%f and right=%f\n",
                        // left, right);
                        if (left < right) {
                            nodelst.add(node.mLeft);
                            nodelst.add(node.mRight);
                        } else {
                            nodelst.add(node.mRight);
                            nodelst.add(node.mLeft);
                        }
                    } else if (left < thresh) {
                        // System.out.printf("add left=%f\n", left);
                        nodelst.add(node.mLeft);
                    } else if (right < thresh) {
                        // System.out.printf("add right=%f\n", right);
                        nodelst.add(node.mRight);
                    }
                }
            }
        }
        // System.out.printf("pt=%s cnt=%d\n", pt, cnt);
        return near;
    }

    public static void main(String[] args) {
        return;
    }
}
