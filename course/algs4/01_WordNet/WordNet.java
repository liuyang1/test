import edu.princeton.cs.algs4.Digraph;
import java.util.*;
import java.io.File;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileNotFoundException;
import java.io.IOException;

public class WordNet {
    private List < String[] > loadCSV(String csv) {
        File f = new File(csv);
        List< String[] > lst = new LinkedList< String[] >();
        String line;
        BufferedReader rd = null;
        try {
            rd = new BufferedReader(new FileReader(f));
            while ((line = rd.readLine()) != null) {
                String[] item = line.split(",");
                lst.add(item);
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (rd != null) {
                try {
                    rd.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return lst;
    }
    private void showCSV(List< String[] > lst) {
        for (String[] item: lst) {
            for (String one: item) {
                System.out.printf("%s ", one);
            }
            System.out.printf("\n");
        }
    }
    Digraph mG;
    HashMap<Integer, String> mTblSyn;
    SAP mSap;
    // constructor takes the name of the two input files
    public WordNet(String synsets, String hypernyms) {
        List< String[] > lstSyn = loadCSV(synsets);
        mTblSyn = new HashMap<Integer, String>();
        for (String[] item: lstSyn) {
            mTblSyn.put(Integer.parseInt(item[0]), item[1]);
        }

        List< String[] > lstHyp = loadCSV(hypernyms);
        mG = new Digraph(mTblSyn.size());
        for (String[] item: lstHyp) {
            int begin = Integer.parseInt(item[0]), end;
            int i;
            for (i = 1; i != item.length; i++) {
                end = Integer.parseInt(item[i]);
                mG.addEdge(begin, end);
            }
        }
        System.out.printf("mG=%s\n", mG);
        mSap = new SAP(mG);
    }

    // returns all WordNet nouns
    public Iterable<String> nouns() {
        return mTblSyn.values();
    }

    // is the word a WordNet noun?
    public boolean isNoun(String word) {
        for (String s: nouns()) {
            if (s.equals(word)) {
                return true;
            }
        }
        return false;
    }

    // distance between nounA and nounB
    public int distance(String nounA, String nounB) {
        return -1;
    }

    // a synset (second field of synsets.txt) that is the common ancestor of nounA and nounB
    // in a shortest ancestral path
    public String sap(String nounA, String nounB) {
        return null;
    }

    private static void test() {
        String path = "test/wordnet";
        String synsets = path + "/synsets11.txt";
        String hypernyms = path + "/hypernyms11ManyPathsOneAncestor.txt";
        WordNet wn = new WordNet(synsets, hypernyms);
        Iterable<String> ns = wn.nouns();
        for (String s: ns) {
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
