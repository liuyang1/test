import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;

public class Outcast {
    private WordNet mWordNet;
    public Outcast(WordNet wordnet) // constructor takes a WordNet object
    {
        mWordNet = wordnet;
    }
    // given an array of WordNet nouns return an outcast,
    public String outcast(String[] nouns) {
        int max = -1, dist0, dist;
        String s = null;
        for (String ai : nouns) {
            dist = 0;
            for (String aj : nouns) {
                if (ai.equals(aj)) {
                    continue;
                }
                // StdOut.printf("%s %s ->\n", ai, aj);
                dist0 = mWordNet.distance(ai, aj);
                // StdOut.printf("%s %s -> %d\n", ai, aj, dist0);
                dist += dist0;
            }
            if (dist > max) {
                max = dist;
                s = ai;
            }
        }
        return s;
    }
    private static void testCase(String[] args) {
        WordNet wordnet = new WordNet(args[0], args[1]);
        Outcast outcast = new Outcast(wordnet);
        for (int t = 2; t < args.length; t++) {
            In in = new In(args[t]);
            String[] nouns = in.readAllStrings();
            StdOut.println(args[t] + ": " + outcast.outcast(nouns));
        }
    }
    public static void main(String[] args) // see test client below
    {
        testCase(args);
    }
}
