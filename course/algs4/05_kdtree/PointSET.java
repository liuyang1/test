import java.util.LinkedList;
import edu.princeton.cs.algs4.RedBlackBST;
import edu.princeton.cs.algs4.Point2D;
import edu.princeton.cs.algs4.RectHV;

public class PointSET {
    // limit to regin [0, 1] x [0, 1], so max of squred dist is 2
    private static final double MAX_DIST2 = 2 + 0.1;
    private RedBlackBST<Point2D, Double> mList;
    // construct an empty set of points
    public PointSET() {
        mList = new RedBlackBST<Point2D, Double>();
    }
    // is the set empty?
    public boolean isEmpty() {
        return mList.size() == 0;
    }
    // number of points in the set
    public int size() {
        return mList.size();
    }
    // add the point to the set (if it is not already in the set)
    public void insert(Point2D p) {
        mList.put(p, p.x() + p.y());
    }
    // does the set contain point p?
    public boolean contains(Point2D p) {
        return mList.contains(p);
    }
    // draw all points to standard draw
    public void draw() {
        for (Point2D iPt: mList.keys()) {
            iPt.draw();
        }
    }
    // all points that are inside the rectangle
    public Iterable<Point2D> range(RectHV rect) {
        LinkedList<Point2D> ret = new LinkedList<Point2D>();
        for (Point2D iPt: mList.keys()) {
            if (rect.contains(iPt)) {
                ret.add(iPt);
            }
        }
        return ret;
    }
    // a nearest neighbor in the set to point p; null if the set is empty
    public Point2D nearest(Point2D p) {
        if (isEmpty()) {
            return null;
        }
        double minDist2 = MAX_DIST2, dist2;
        Point2D minPt = null;
        for (Point2D iPt: mList.keys()) {
            dist2 = p.distanceSquaredTo(iPt);
            if (dist2 <= minDist2) {
                minDist2 = dist2;
                minPt = iPt;
            }
        }
        return minPt;
    }

    public static void main(String[] args) {
        return;
    }
}
