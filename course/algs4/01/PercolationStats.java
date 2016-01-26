public class PercolationStats {
    public PercolationStats(int N, int T) {
    }
    public double mean() {
        return 0;
    }
    public double stddev() {
        return 0;
    }
    public double confidenceLo() {
        return 0;
    }
    public double confidenceHi() {
        return 0;
    }
    public static void usage() {
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
        PercolationStats stat = new PercolationStats(N, T);
        System.out.printf("mean                    = %f\n", stat.mean());
        System.out.printf("stddev                  = %f\n", stat.stddev());
        System.out.printf("95%% confidence interval = %f %f\n",
                stat.confidenceLo(), stat.confidenceHi());
    }
}
