import java.util.LinkedList;
import java.util.Comparator;
import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.MinPQ;
public class Solver {
    private class SearchNode {
        Board mBoard;
        int mMoves;
        SearchNode mPrev;
        int mHamming;
        int mManhattan;
        public SearchNode(Board brd, int moves, SearchNode prev) {
            mBoard = brd;
            mMoves = moves;
            mPrev = prev;
            mHamming = mBoard.hamming();
            mManhattan = mBoard.manhattan();
        }
    }
    private Board mInitial;
    private LinkedList<Board> mSolutionSeq;
    // find a solution to the initial board (using the A* algorithm)
    public Solver(Board initial)
    {
        mInitial = initial;
        mSolutionSeq = new LinkedList<Board>();
    }
    private static Comparator<SearchNode> hammingOrder() {
        return new Comparator<SearchNode>() {
            public int compare(SearchNode n0, SearchNode n1) {
                // return Integer.compare(n0.mBoard.hamming(), n1.mBoard.hamming());
                return Integer.compare(n0.mHamming, n1.mHamming);
            }
        };
    }
    private void backtraceSln(SearchNode goal) {
        while (goal != null) {
            mSolutionSeq.addFirst(goal.mBoard);
            goal = goal.mPrev;
        }
    }
    private void search() {
        SearchNode node = new SearchNode(mInitial, 0, null);
        MinPQ<SearchNode> q = new MinPQ<SearchNode>(hammingOrder());
        q.insert(node);
        int i = 0;
        while (true) {
            node = q.delMin();
            System.out.printf("pop min board hamming=%d moves=%d\n%s", node.mHamming, node.mMoves, node.mBoard);
            if (node.mBoard.isGoal()) {
                System.out.printf("find goal\n");
                backtraceSln(node);
                break;
            }
            for (Board brd: node.mBoard.neighbors()) {
                // System.out.printf("push neighbor hamming=%d\n%s", brd.hamming(), brd);
                if (node.mPrev != null && brd.equals(node.mPrev.mBoard)) {
                    // System.out.printf("disallow neighbors which is equals to previous\n");
                }
                SearchNode nn = new SearchNode(brd, node.mMoves + 1, node);
                q.insert(nn);
            }
            i++;
            if (i == 20) {
                break;
            }
        }
    }
    public boolean isSolvable() // is the initial board solvable?
    {
        return true;
    }
    // min number of moves to solve initial board; -1 if unsolvable
    public int moves()
    {
        return mSolutionSeq.size() - 1;
    }
    // sequence of boards in a shortest solution; null if unsolvable
    public Iterable<Board> solution()
    {
        return mSolutionSeq;
    }
    // solve a slider puzzle
    public static void testExtCase(String[] args) {
        // create initial board from file
        In in = new In(args[0]);
        int N = in.readInt();
        int[][] blocks = new int[N][N];
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                blocks[i][j] = in.readInt();
        Board initial = new Board(blocks);

        // solve the puzzle
        Solver solver = new Solver(initial);

        // print solution to standard output
        if (!solver.isSolvable())
            StdOut.println("No solution possible");
        else {
            StdOut.println("Minimum number of moves = " + solver.moves());
            for (Board board : solver.solution())
                StdOut.println(board);
        }
    }
    public static boolean testUnit() {
        int[][] blocks = {{0, 1, 3}, {4, 2, 5}, {7, 8, 6}};
        System.out.printf("Solvable case\n");
        Board brd = new Board(blocks);
        testOne(brd);
        System.out.printf("Unsolvable case\n");
        Board brd1 = brd.twin();
        testOne(brd1);
        return true;
    }
    public static boolean testOne(Board brd) {
        Solver solver = new Solver(brd);
        if (solver.isSolvable()) {
            solver.search();
            System.out.printf("move to goal need %d moves\n", solver.moves());
            int moves = 0;
            for (Board ibrd: solver.solution()) {
                System.out.printf("move step=%d\n%s", moves, ibrd);
                moves++;
            }
            return true;
        } else {
            System.out.printf("No Solution possible\n");
            return false;
        }
    }
    public static void main(String[] args) {
        testUnit();
    }
}
