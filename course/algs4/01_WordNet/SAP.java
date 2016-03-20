import java.util.ArrayList;
import java.util.LinkedList;
import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.Digraph;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

public class SAP {

    Digraph mG;
    // constructor takes a digraph (not necessarily a DAG)
    public SAP(Digraph G) {
        mG = G;
    }

    // length of shortest ancestral path between v and w; -1 if no such path
    public int length(int v, int w) {
        LinkedList<Integer> vs = new LinkedList<Integer>();
        vs.add(v);
        LinkedList<Integer> ws = new LinkedList<Integer>();
        ws.add(w);
        return length(vs, ws);
    }

    // a common ancestor of v and w that participates in a shortest ancestral path; -1 if no such path
    public int ancestor(int v, int w) {
        LinkedList<Integer> vs = new LinkedList<Integer>();
        vs.add(v);
        LinkedList<Integer> ws = new LinkedList<Integer>();
        ws.add(w);
        return ancestor(vs, ws);
    }

    private LinkedList<Integer> adj(LinkedList<Integer> v) {
        LinkedList<Integer> lst = new LinkedList<Integer>();
        for (int vv: v) {
            for (int w: mG.adj(vv)) {
                lst.add(w);
            }
        }
        return lst;
    }
    private final static int NOTFIND = -1;
    private int findAncestor(Iterable<Integer> v, Iterable<Integer> w) {
        for (int a: v) {
            for (int b: w) {
                if (a == b) {
                    return a;
                }
            }
        }
        return NOTFIND;
    }
    private class Result {
        int mStep;
        int mAncestor;
        Result(int step, int ancestor) {
            mStep = step;
            mAncestor = ancestor;
        }
    }
    private Result search(LinkedList<Integer> v, LinkedList<Integer> w) {
        int step = 0;
        int size = mG.V();
        ArrayList< LinkedList<Integer> > vs = new ArrayList< LinkedList<Integer> >();
        ArrayList< LinkedList<Integer> > ws = new ArrayList< LinkedList<Integer> >();
        vs.add(v);
        ws.add(w);
        int i;
        for (; step <= size; step++) {
            for (i = 0; i != step + 1; i++) {
                v = vs.get(i);
                w = ws.get(step - i);
                int r = findAncestor(v, w);
                if (r != NOTFIND) {
                    return new Result(step, r);
                }
            }
            vs.add(adj(vs.get(step)));
            ws.add(adj(ws.get(step)));
        }
        return new Result(-1, NOTFIND);
    }
    private LinkedList<Integer> trans(Iterable<Integer> lst) {
        LinkedList<Integer> ret = new LinkedList<Integer>();
        for (int i: lst) {
            ret.add(i);
        }
        return ret;
    }
    // length of shortest ancestral path between any vertex in v and any vertex in w; -1 if no such path
    public int length(Iterable<Integer> v, Iterable<Integer> w) {
        Result ret = search(trans(v), trans(w));
        return ret.mStep;
    }

    // a common ancestor that participates in shortest ancestral path; -1 if no such path
    public int ancestor(Iterable<Integer> v, Iterable<Integer> w) {
        Result ret = search(trans(v), trans(w));
        return ret.mAncestor;
    }

    public static void testCase(String fnDigraph) {
        In in = new In(fnDigraph);
        Digraph G = new Digraph(in);
        SAP sap = new SAP(G);
        while (!StdIn.isEmpty()) {
            int v = StdIn.readInt();
            int w = StdIn.readInt();
            int length   = sap.length(v, w);
            int ancestor = sap.ancestor(v, w);
            StdOut.printf("length = %d, ancestor = %d\n", length, ancestor);
        }
    }
    // do unit testing of this class
    public static void main(String[] args) {
        testCase("test/wordnet/digraph1.txt");
        return;
    }
}
