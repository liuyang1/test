import edu.princeton.cs.algs4.StdRandom;
import java.util.Iterator;
public class RandomizedQueue<Item> implements Iterable<Item> {
    private Item []mArray;
    private int mSize;
    private int mCap;
    public RandomizedQueue() {
        mSize = 0;
        mCap = 0;
        mArray = null;
    }
    public boolean isEmpty() {
        return mSize == 0;
    }
    public int size() {
        return mSize;
    }
    private void addPrecheck(Item item) {
        if (item == null) {
            throw new java.lang.NullPointerException();
        }
    }
    private void removePrecheck() {
        if (mSize == 0) {
            throw new java.util.NoSuchElementException();
        }
    }
    private void resize(int cap) {
        if (cap < mSize) {
            // not enough memory
            throw new java.util.NoSuchElementException();
        }
        Item []mOldArray = mArray;
        mArray = (Item[]) new Object[cap];
        int i;
        for (i = 0; i != mSize; i++) {
            mArray[i] = mOldArray[i];
        }
        mCap = cap;
    }
    public void enqueue(Item item) {
        addPrecheck(item);
        if (mSize >= mCap) {
            if (mCap == 0) {
                resize(2);
            } else {
                resize(2 * mCap);
            }
        }
        mArray[mSize] = item;
        mSize++;
    }
    public Item dequeue() {
        removePrecheck();
        int randidx = StdRandom.uniform(mSize);
        Item it = mArray[randidx];
        if (randidx != mSize - 1) {
            mArray[randidx] = mArray[mSize - 1];
        }
        mArray[mSize - 1] = null; // make it relase
        mSize--;
        if (mSize < mCap / 4) {
            resize(mCap / 4);
        }
        return it;
    }
    public Item sample() {
        removePrecheck();
        int randidx = StdRandom.uniform(mSize);
        Item it = mArray[randidx];
        return it;
    }
    public Iterator<Item> iterator() {
        return new RandomizedQueueIterator(mSize);
    }
    private class RandomizedQueueIterator implements Iterator<Item> {
        private int mCurrent;
        private int mSize;
        private int []mSeq;
        public RandomizedQueueIterator(int size) {
            mCurrent = 0;
            mSize = size;
            mSeq = new int[size];
            int i;
            for (i = 0; i != size; i++) {
                mSeq[i] = i;
            }
            StdRandom.shuffle(mSeq);
        }
        public boolean hasNext() {
            return mCurrent != mSize;
        }
        public void remove() {
            throw new java.lang.UnsupportedOperationException();
        }
        public Item next() {
            if (!hasNext()) {
                throw new java.util.NoSuchElementException();
            }
            Item it = mArray[mSeq[mCurrent]];
            mCurrent++;
            return it;
        }
    }
    private void show() {
        System.out.printf("size=%d/%d\t", mSize, mCap);
        int i;
        if (mArray == null) {
            System.out.printf("nil\n");
            return;
        }
        System.out.printf("[");
        for (i = 0; i != mSize; i++) {
            String ns;
            if (mArray[i] != null) {
                ns = mArray[i].toString();
            } else {
                ns = "nil";
            }
            if (i == 0) {
                System.out.printf(ns);
            } else {
                System.out.printf(", " + ns);
            }
        }
        System.out.printf("]\n");
    }

    private static String expect(boolean b) {
        if (b) {
            return "PASS";
        } else {
            return "fail";
        }
    }
    private static boolean testBasic() {
        RandomizedQueue<Integer> rq = new RandomizedQueue<Integer>();
        System.out.printf("empty check: %s\n", expect(rq.isEmpty()));
        int i, n = 10;
        for (i = 0; i != n; i++) {
            rq.enqueue(i);
        }
        rq.show();
        while (!rq.isEmpty()) {
            System.out.printf(" %d: ", rq.size());
            int rt = rq.dequeue();
            System.out.printf("%d,", rt);
        }
        System.out.printf("\n");
        try {
            rq.dequeue();
        } catch (java.util.NoSuchElementException e) {
            System.out.printf("no such element except normal");
        }
        return true;
    }
    private static void testIter() {
        RandomizedQueue<Integer> rq = new RandomizedQueue<Integer>();
        int i, n = 10;
        for (i = 0; i != n; i++) {
            rq.enqueue(i);
        }
        rq.show();
        Iterator<Integer> iter = rq.iterator();
        System.out.printf("[");
        while (iter.hasNext()) {
            int rt = iter.next();
            System.out.printf(", %d", rt);
        }
        System.out.printf("]\n");
    }
    public static void main(String[] args) {
        testBasic();
        testIter();
        testIter();
    }
}
