import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.Digraph;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

public class Outcast {
   public Outcast(WordNet wordnet)         // constructor takes a WordNet object
   {
   }
   public String outcast(String[] nouns)   // given an array of WordNet nouns, return an outcast
   {
       return null;
   }
   public static void testCase(String[] args) {
       WordNet wordnet = new WordNet(args[0], args[1]);
       Outcast outcast = new Outcast(wordnet);
       for (int t = 2; t < args.length; t++) {
           In in = new In(args[t]);
           String[] nouns = in.readAllStrings();
           StdOut.println(args[t] + ": " + outcast.outcast(nouns));
       }
   }
   public static void main(String[] args)  // see test client below
   {
   }
}
