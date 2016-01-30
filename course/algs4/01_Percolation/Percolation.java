import edu.princeton.cs.algs4.WeightedQuickUnionUF;

public class Percolation {
    private WeightedQuickUnionUF mUnionUF;
    private boolean []mIsOpen;
    private int mHeight;
    private int mWidth;
    private boolean mIsPercolates;
    private int []mOpenBottom;
    private int mOpenBottomNum;
    public Percolation(int N) {
        if (N <= 0) {
            throw new java.lang.IllegalArgumentException();
        }
        int size = N * N + 2;
        mUnionUF = new WeightedQuickUnionUF(size);
        mHeight = N;
        mWidth = N;
        mIsPercolates = false;
        mIsOpen = new boolean[size];
        mOpenBottom = new int[N];
        mOpenBottomNum = 0;
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
    private void chkBound2DEx(int y, int x) {
        if (!isBound2D(oneOffset(y), oneOffset(x))) {
            throw new java.lang.IndexOutOfBoundsException("pos = " + y + " " + x);
        }
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
    private boolean isConnectBottom(int y, int x) {
        int comp = xyTo1D(y, x);
        int i;
        for (i = 0; i != mOpenBottomNum; i++) {
            if (mUnionUF.connected(comp, xyTo1D(mHeight - 1, mOpenBottom[i]))) {
                return true;
            }
        }
        return false;
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
            mOpenBottom[mOpenBottomNum] = j;
            mOpenBottomNum++;
        }
        // if (!percolates() && i == mHeight - 1) {
        if (isFullI(i, j) && isConnectBottom(i, j)) {
            mUnionUF.union(pos, bottomIndex());
            mIsPercolates = true;
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
        chkBound2DEx(i, j);
        openI(oneOffset(i), oneOffset(j));
    }
    public boolean isOpen(int i, int j) {
        chkBound2DEx(i, j);
        return isOpenI(oneOffset(i), oneOffset(j));
    }
    public boolean isFull(int i, int j) {
        chkBound2DEx(i, j);
        return isFullI(oneOffset(i), oneOffset(j));
    }
    public boolean percolates() {
        return mIsPercolates;
    }
    public static void main(String[] args) {
        return;
    }
}
