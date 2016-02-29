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
        mBlocks = blocks;
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
    // a board that is obtained by exchanging any pair of blocks
    public Board twin()
    {
        return new Board(null);
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
                if (this.mBlocks[i][j] != that.mBlocks[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
    public Iterable<Board> neighbors() // all neighboring boards
    {
        return null;
    }
    // string representation of this board
    public String toString()
    {
        return "";
    }

    // test code
    public static boolean testDist() {
        int[][] blocks = {{8, 1, 3}, {4, 0, 2}, {7, 6, 5}};
        Board brd = new Board(blocks);
        System.out.printf("%s\n", brd);
        System.out.printf("dim=%d\n", brd.dimension());
        System.out.printf("hamming=%d manhattan=%d\n", brd.hamming(),
                          brd.manhattan());
        return true;
    }
    public static void main(String[] args) // unit tests (not graded)
    {
        testDist();
    }
}
