import edu.princeton.cs.algs4.WeightedQuickUnionUF;

public class Percolation {
    private WeightedQuickUnionUF mUnionUF;
    private boolean []mIsOpen;
    private int mHeight;
    private int mWidth;
    public Percolation(int N) {
        mUnionUF = new WeightedQuickUnionUF(N * N + 2);
        mHeight = N;
        mWidth = N;
        mIsOpen = new boolean[N * N + 2];
        int i;
        for (i = 0; i != N * N; i++) {
            mIsOpen[i] = false;
        }
    }
    private static int oneOffset(int x) {
        return x - 1;
    }
    // virtual-top virtual-bottom, this make isFull and percolates faster
    private int topIndex() {
        return mHeight * mWidth;
    }
    private int bottomIndex() {
        return mHeight * mWidth + 1;
    }
    private boolean isBound2D(int y, int x) {
        return y >= 0 && y < mHeight && x >= 0 && x < mWidth;
    }
    private int xyTo1D(int y, int x) {
        return y * mWidth + x;
    }
    private void chkConnect(int pos1, int i, int j) {
        if (isBound2D(i, j) && isOpenI(i, j)) {
            mUnionUF.union(pos1, xyTo1D(i, j));
        }
    }
    private void openI(int i, int j) {
        int pos = xyTo1D(i, j);
        mIsOpen[pos] = true;
        chkConnect(pos, i - 1, j);
        chkConnect(pos, i + 1, j);
        chkConnect(pos, i, j - 1);
        chkConnect(pos, i, j + 1);
        if (i == 0) {
            mUnionUF.union(pos, topIndex());
        }
        if (i == mHeight - 1) {
            mUnionUF.union(pos, bottomIndex());
        }
    }
    private boolean isOpenI(int i, int j) {
        int pos = xyTo1D(i, j);
        return mIsOpen[pos];
    }
    private boolean isFullI(int i, int j) {
        return isOpenI(i, j) && mUnionUF.connected(xyTo1D(i, j), topIndex());
    }
    // API use [1, N], but internal use [0, N), wrap them
    public void open(int i, int j) {
        openI(oneOffset(i), oneOffset(j));
    }
    public boolean isOpen(int i, int j) {
        return isOpenI(oneOffset(i), oneOffset(j));
    }
    public boolean isFull(int i, int j) {
        return isFullI(oneOffset(i), oneOffset(j));
    }
    public boolean percolates() {
        return mUnionUF.connected(topIndex(), bottomIndex());
    }
    public static void main(String[] args) {
        return;
    }
}
