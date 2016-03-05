/******************************************************************************
 *  Compilation:  javac KdTreeVisualizer.java
 *  Execution:    java KdTreeVisualizer
 *  Dependencies: KdTree.java
 *
 *  Add the points that the user clicks in the standard draw window
 *  to a kd-tree and draw the resulting kd-tree.
 *
 ******************************************************************************/

import edu.princeton.cs.algs4.Point2D;
import edu.princeton.cs.algs4.RectHV;
import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.StdOut;

public class KdTreeVisualizer {

    public static void main(String[] args) {
        RectHV rect = new RectHV(0.0, 0.0, 1.0, 1.0);
        StdDraw.show(0);
        KdTree kdtree = new KdTree();
        double px = 0, py = 0;
        while (true) {
            if (StdDraw.mousePressed()) {
                double x = StdDraw.mouseX();
                double y = StdDraw.mouseY();
                // avoid double-click point
                // System.out.printf("clink (%f, %f) prev (%f, %f)\n", x, y, px, py);
                boolean isDup = Math.abs(x - px) <= 0.01 && Math.abs(y - py) <= 0.01;
                px = x;
                py = y;
                if (isDup) {
                    continue;
                }
                Point2D p = new Point2D(x, y);
                if (rect.contains(p)) {
                    StdOut.printf("click %8.6f %8.6f\n", x, y);
                    kdtree.insert(p);
                    StdDraw.clear();
                    kdtree.draw();
                }
            }
            StdDraw.show(50);
        }

    }
}
