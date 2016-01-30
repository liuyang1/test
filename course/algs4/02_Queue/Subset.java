import edu.princeton.cs.algs4.StdRandom;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
public class Subset {
    private static void usage() {
        System.out.printf("java Subset [k]\n"
                + "\tinput sequence split with whitespace\n");
    }
    public static void main(String[] args) {
        if (args.length != 1) {
            usage();
            return;
        }
        int k;
        try {
            k = Integer.parseInt(args[0]);
        } catch (NumberFormatException e) {
            usage();
            return;
        }
        RandomizedQueue<String> rq = new RandomizedQueue<String>();
        String s;
        int count = 0;
        while (true) {
            try {
                s = StdIn.readString();
            } catch (java.util.NoSuchElementException e) {
                break;
            }
            count++;
            if (count <= k) {
                rq.enqueue(s);
            } else {
                int rnd = StdRandom.uniform(count);
                if (rnd < k) {
                    rq.dequeue();
                    rq.enqueue(s);
                }
            }
        }
        for (int i = 0; i != k; i++) {
            s = rq.dequeue();
            StdOut.println(s);
        }
    }
}
