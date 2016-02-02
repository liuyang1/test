import edu.princeton.cs.algs4.WeightedQuickUnionUF;

public class Percolation {
    private WeightedQuickUnionUF mUnionUF;
    private char []mGridSt;
    private int mHeight;
    private int mWidth;
    private boolean mIsPercolates;
    public Percolation(int N) {
        if (N <= 0) {
            throw new java.lang.IllegalArgumentException();
        }
        int size = N * N + 2;
        mUnionUF = new WeightedQuickUnionUF(size);
        mHeight = N;
        mWidth = N;
        mIsPercolates = false;
        mGridSt = new char[size];
        int i;
        for (i = 0; i != size; i++) {
            mGridSt[i] = 0;
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
    private void chkConnect(int pos, int i, int j) {
        if (isBound2D(i, j) && isOpenI(i, j)) {
            int comp0 = mUnionUF.find(pos);
            int comp1 = mUnionUF.find(xyTo1D(i, j));
            mUnionUF.union(comp0, comp1);
            int newcomp = mUnionUF.find(comp0);
            if (isGridTop(comp0) || isGridTop(comp1)) {
                setGridTop(newcomp);
                if (isOpenI(i, j)) {
                    mUnionUF.union(comp0, topIndex());
                }
            }
            if (isGridBot(comp0) || isGridBot(comp1)) {
                setGridBot(newcomp);
            }
        }
    }
    private void setGridOpen(int pos) {
        mGridSt[pos] |= 1;
    }
    private void setGridTop(int pos) {
        mGridSt[pos] |= 2;
    }
    private void setGridBot(int pos) {
        mGridSt[pos] |= 4;
    }
    private boolean isGridOpen(int pos) {
        return (mGridSt[pos] & 1) != 0;
    }
    private boolean isGridTop(int pos) {
        return (mGridSt[pos] & 2) != 0;
    }
    private boolean isGridBot(int pos) {
        return (mGridSt[pos] & 4) != 0;
    }
    private void openI(int i, int j) {
        int pos = xyTo1D(i, j);
        setGridOpen(pos);
        if (i == 0) {
            setGridTop(pos);
            mUnionUF.union(pos, topIndex());
        }
        if (i == mHeight - 1) {
            setGridBot(pos);
        }
        chkConnect(pos, i - 1, j);
        chkConnect(pos, i + 1, j);
        chkConnect(pos, i, j - 1);
        chkConnect(pos, i, j + 1);
        int newcomp = mUnionUF.find(pos);
        if (isGridTop(newcomp) && isGridBot(newcomp)) {
            mIsPercolates = true;
        }
    }
    private boolean isOpenI(int i, int j) {
        int pos = xyTo1D(i, j);
        return isGridOpen(pos);
    }
    private boolean isFullI(int i, int j) {
        return isOpenI(i, j) && mUnionUF.connected(xyTo1D(i, j), topIndex());
    }
    private boolean isBackI(int i, int j) {
        return isOpenI(i, j) && mUnionUF.connected(xyTo1D(i, j), bottomIndex());
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
