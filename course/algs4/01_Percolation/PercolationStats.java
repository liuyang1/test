import edu.princeton.cs.algs4.Stopwatch;
import edu.princeton.cs.algs4.StdStats;
import edu.princeton.cs.algs4.StdRandom;

public class PercolationStats {
    private double []mSimuResult;
    private int mTime;
    private double mSize2;
    public PercolationStats(int N, int T) {
        if (N <= 0 || T <= 0) {
            throw new java.lang.IllegalArgumentException();
        }
        mTime = T;
        mSize2 = N * N;
        mSimuResult = new double[T];
        int rs, r, i, j;
        for (i = 0; i != T; i++) {
            mSimuResult[i] = simuPercolation(N) / mSize2;
        }
    }
    private static class Shuffle2 {
        private int []mGrid;
        private int mHeight;
        private int mWidth;
        private int mSeq;
        public Shuffle2(int m, int n) {
            mSeq = 0;
            mHeight = m;
            mWidth = n;
            mGrid = new int[mWidth * mHeight];
            int i, j, k;
            for (i = 0, k = 0; i != mHeight; i++) {
                for (j = 0; j != mWidth; j++, k++) {
                    mGrid[mWidth * i + j] = k;
                }
            }
            StdRandom.shuffle(mGrid);
        }
        public int[] acquire() {
            int ret = mGrid[mSeq];
            int i = ret % mWidth;
            int j = ret / mWidth;
            mSeq++;
            return new int[] {i, j};
        }
    }

    private static int simuPercolation(int N) {
        Percolation perc = new Percolation(N);
        Shuffle2 sfl = new Shuffle2(N, N);
        int r = 0;
        while (!perc.percolates()) {
            int []pos = sfl.acquire();
            perc.open(pos[0] + 1, pos[1] + 1);
            r++;
        }
        return r;
    }
    public double mean() {
        return StdStats.mean(mSimuResult);
    }
    public double stddev() {
        return StdStats.stddev(mSimuResult);
    }
    public double confidenceLo() {
        double mean = mean();
        double stddev = stddev();
        double ratio = 1.96 / Math.sqrt(mTime);
        return mean - stddev * ratio;
    }
    public double confidenceHi() {
        double mean = mean();
        double stddev = stddev();
        double ratio = 1.96 / Math.sqrt(mTime);
        return mean + stddev * ratio;
    }
    private static void usage() {
        System.out.printf("java PercolationStats [N] [T]\n");
    }
    public static void main(String[] args) {
        if (args.length != 2) {
            usage();
            return;
        }
        int N, T;
        try {
            N = Integer.parseInt(args[0]);
            T = Integer.parseInt(args[1]);
        } catch (NumberFormatException e) {
            usage();
            return;
        }
        Stopwatch sw = new Stopwatch();
        PercolationStats stat = new PercolationStats(N, T);
        System.out.printf("elapsed time            = %f seconds\n",
                          sw.elapsedTime());
        double expect = 0.59, thresh = 0.01;
        double mean = stat.mean();
        System.out.printf("mean                    = %f(expect: %f)\n",
                          mean, expect);
        System.out.printf("stddev                  = %f\n", stat.stddev());
        System.out.printf("95%% confidence interval = %f %f\n",
                          stat.confidenceLo(), stat.confidenceHi());
    }
}
