import java.util.List;
import java.util.LinkedList;
public class Board {
    private int[][] mBlocks;
    private int mDim;
    // construct a board from an N-by-N array of blocks
    // where blocks[i][j] = block in row i, column j
    public Board(int[][] blocks) {
        mDim = blocks.length;
        if (mDim == 0 || mDim != blocks[0].length) {
            throw new java.lang.IllegalArgumentException();
        }
        // deep copy
        mBlocks = new int[mDim][mDim];
        for (int i = 0; i != mDim; i++) {
            for (int j = 0; j != mDim; j++) {
                mBlocks[i][j] = blocks[i][j];
            }
        }
    }
    public int dimension() // board dimension N
    {
        return mDim;
    }
    public int hamming() // number of blocks out of place
    {
        int dist = 0;
        int last = mDim * mDim;
        for (int i = 0; i != mDim; i++) {
            for (int j = 0; j != mDim; j++) {
                int offset = 1 + i * mDim + j;
                if (offset != last && mBlocks[i][j] != offset) {
                    dist += 1;
                }
            }
        }
        return dist;
    }
    public int manhattan() // sum of Manhattan distances between blocks and goal
    {
        int dist = 0;
        int val, x, y, delta;
        for (int i = 0; i != mDim; i++) {
            for (int j = 0; j != mDim; j++) {
                val = mBlocks[i][j];
                if (val == 0) {
                    continue;
                }
                x = (val - 1) / mDim;
                y = (val - 1) % mDim;
                delta = Math.abs(x - i) + Math.abs(y - j);
                dist += delta;
                // System.out.printf("(%d, %d) %d -> (%d, %d) delta=%d\n",
                //         i, j, val, x, y, delta);
            }
        }
        return dist;
    }
    public boolean isGoal() // is this board the goal board?
    {
        return hamming() == 0;
    }
    // k combinator, to swap x and y
    // y = k(x, x=y);
    private static int kCombinator(int a, int b) {
        return a;
    }
    private Board twinPos(int x, int y, int x1, int y1) {
        Board brd = new Board(mBlocks);
        brd.mBlocks[x1][y1] = kCombinator(brd.mBlocks[x][y],
                brd.mBlocks[x][y] = brd.mBlocks[x1][y1]);
        return brd;
    }
    // a board that is obtained by exchanging any pair of blocks
    public Board twin()
    {
        if (mBlocks[0][0] != 0 && mBlocks[0][1] != 0) {
            return twinPos(0, 0, 0, 1);
        } else {
            return twinPos(1, 0, 1, 1);
        }
    }
    public boolean equals(Object y) // does this board equal y?
    {
        if (this == y) {
            return true;
        }
        if (this == null || this.getClass() != y.getClass()) {
            return false;
        }
        Board that = (Board) y;
        if (this.dimension() != that.dimension()) {
            return false;
        }
        for (int i = 0; i != this.dimension(); i++) {
            for (int j = 0; j != this.dimension(); j++) {
                // System.out.printf("(%d, %d) %d %d\n",
                //         i, j, this.mBlocks[i][j], that.mBlocks[i][j]);
                if (this.mBlocks[i][j] != that.mBlocks[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
    // all neighboring boards
    // find 0 postion first, then swap it with its neighbors
    public Iterable<Board> neighbors()
    {
        List<Board> lst = new LinkedList<Board>();
        int x = -1, y = -1;
        for (int i = 0; i != mDim; i++) {
            for (int j = 0; j != mDim; j++) {
                if (mBlocks[i][j] == 0) {
                    x = i;
                    y = j;
                }
            }
        }
        if (x == -1 || y == -1) {
            // cannot find zero
            throw new java.lang.IllegalArgumentException();
        }
        if (x != 0) {
            lst.add(twinPos(x, y, x - 1, y));
        }
        if (x != mDim - 1) {
            lst.add(twinPos(x, y, x + 1, y));
        }
        if (y != 0) {
            lst.add(twinPos(x, y, x, y - 1));
        }
        if (y != mDim - 1) {
            lst.add(twinPos(x, y, x, y + 1));
        }
        return lst;
    }
    // string representation of this board
    public String toString()
    {
        StringBuilder ret = new StringBuilder();
        String newline = System.getProperty("line.separator");
        ret.append("" + mDim + newline);
        for (int i = 0; i != mDim; i++) {
            for (int j = 0; j != mDim; j++) {
                ret.append(String.format("%2d ", mBlocks[i][j]));
            }
            ret.append(newline);
        }
        return ret.toString();
    }

    // test code
    public static boolean testDist() {
        int[][] blocks = {{8, 1, 3}, {4, 0, 2}, {7, 6, 5}};
        Board brd = new Board(blocks);
        System.out.printf("%s\n", brd);
        System.out.printf("dim=%d\n", brd.dimension());
        System.out.printf("hamming=%d manhattan=%d\n", brd.hamming(),
                          brd.manhattan());

        Board brd1 = new Board(blocks);
        System.out.printf("equal=%s\n", brd.equals(brd1));
        blocks[0][0] = 1;
        blocks[0][1] = 8;
        Board brd2 = new Board(blocks);
        System.out.printf("equal=%s\n", brd.equals(brd2));

        System.out.printf("neighbors\n");
        for (Board bd: brd.neighbors()) {
            System.out.printf("%s\n", bd);
        }

        System.out.printf("twin\n");
        System.out.printf("%s\n", brd.twin());
        return true;
    }
    public static void main(String[] args) // unit tests (not graded)
    {
        testDist();
    }
}
