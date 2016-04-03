import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.Digraph;
import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.LinkedList;

public class WordNet {
    private Digraph mG;
    private HashMap<String, LinkedList<Integer>> mTblSyn;
    private SAP mSap;
    // constructor takes the name of the two input files
    public WordNet(String synsets, String hypernyms) {
        List<String[]> lstSyn = loadCSV(synsets);
        mTblSyn = new HashMap<String, LinkedList<Integer>>();
        for (String[] item : lstSyn) {
            int value = Integer.parseInt(item[0]);
            // String noun = item[1];
            for (String noun : item[1].split(" "))
            {
                if (mTblSyn.containsKey(noun)) {
                    LinkedList<Integer> lst = mTblSyn.get(noun);
                    lst.add(value);
                } else {
                    LinkedList<Integer> lst = new LinkedList<Integer>();
                    lst.add(value);
                    mTblSyn.put(noun, lst);
                }
            }
        }

        List<String[]> lstHyp = loadCSV(hypernyms);
        mG = new Digraph(lstSyn.size());
        for (String[] item : lstHyp) {
            int begin = Integer.parseInt(item[0]), end;
            int i;
            for (i = 1; i != item.length; i++) {
                end = Integer.parseInt(item[i]);
                mG.addEdge(begin, end);
            }
        }
        // System.out.printf("mG=%s\n", mG);
        mSap = new SAP(mG);
    }
    private List<String[]> loadCSV(String csv) {
        In f = new In(csv);
        LinkedList<String[]> lst = new LinkedList<String[]>();
        while (f.hasNextLine()) {
            String line = f.readLine();
            if (line == null) {
                break;
            }
            String[] item = line.split(",");
            lst.add(item);
        }
        return lst;
    }

    private void showCSV(List<String[]> lst) {
        for (String[] item : lst) {
            for (String one : item) {
                System.out.printf("%s ", one);
            }
            System.out.printf("\n");
        }
    }
    // returns all WordNet nouns
    public Iterable<String> nouns() {
        return mTblSyn.keySet();
    }

    // is the word a WordNet noun?
    public boolean isNoun(String word) {
        for (String s : nouns()) {
            if (s.equals(word)) {
                return true;
            }
        }
        return false;
    }

    // distance between nounA and nounB
    public int distance(String nounA, String nounB) {
        // StdOut.printf("%s %s\n", nounA, nounB);
        LinkedList<Integer> idA = mTblSyn.get(nounA);
        LinkedList<Integer> idB = mTblSyn.get(nounB);
        // StdOut.printf("%s %s %s %s\n", nounA, idA, nounB, idB);
        return mSap.length(idA, idB);
    }

    private String backGet(int id) {
        for (Map.Entry<String, LinkedList<Integer>> entry :
             mTblSyn.entrySet()) {
            for (int i : entry.getValue()) {
                if (i == id) {
                    return entry.getKey();
                }
            }
        }
        return null;
    }
    // a synset (second field of synsets.txt) that is the common ancestor of
    // nounA and nounB in a shortest ancestral path
    public String sap(String nounA, String nounB) {
        LinkedList<Integer> idA = mTblSyn.get(nounA);
        LinkedList<Integer> idB = mTblSyn.get(nounB);
        int anc = mSap.ancestor(idA, idB);
        return backGet(anc);
    }

    private static void test() {
        String path = "test/wordnet";
        String synsets = path + "/synsets11.txt";
        String hypernyms = path + "/hypernyms11ManyPathsOneAncestor.txt";
        WordNet wn = new WordNet(synsets, hypernyms);
        Iterable<String> ns = wn.nouns();
        for (String s : ns) {
            System.out.printf("%s\n", s);
        }
        String w = "yes";
        System.out.printf("isNoun(%s) = %s\n", w, wn.isNoun(w));
        w = "a";
        System.out.printf("isNoun(%s) = %s\n", w, wn.isNoun(w));
    }
    // do unit testing of this class
    public static void main(String[] args) {
        test();
    }
}
