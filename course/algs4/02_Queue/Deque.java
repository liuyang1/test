import java.util.Iterator;
public class Deque<Item> implements Iterable<Item> {
    private Node mHead = null;
    private Node mTail = null;
    private int mSize = 0;
    private class Node {
        private Item item;
        private Node prev;
        private Node next;
        Node(Item it, Node pv, Node nx) {
            item = it;
            prev = pv;
            next = nx;
        }
    }
    public Deque() {
    }
    public boolean isEmpty() {
        return mHead == null;
    }
    public int size() {
        return mSize;
    }
    private void show() {
        System.out.printf("size=%d\t", mSize);
        Node n;
        System.out.printf("[");
        for (n = mHead; n != null; n = n.next) {
            String ns = n.toString();
            if (n.item == null) {
                ns += " null";
            } else {
                ns += " " + n.item.toString();
            }
            if (n == mHead) {
                System.out.printf(ns);
            } else {
                System.out.printf(", " + ns);
            }
        }
        System.out.printf("]\n");
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
    public void addFirst(Item item) {
        addPrecheck(item);
        Node n = new Node(item, null, mHead);
        if (mHead != null) {
            mHead.prev = n;
        }
        mHead = n;
        if (mSize == 0) {
            mTail = n;
        }
        mSize++;
    }
    public void addLast(Item item) {
        addPrecheck(item);
        Node n = new Node(item, mTail, null);
        if (mTail != null) {
            mTail.next = n;
        }
        mTail = n;
        if (mSize == 0) {
            mHead = n;
        }
        mSize++;
    }
    public Item removeFirst() {
        removePrecheck();
        Item it = mHead.item;
        mHead = mHead.next;
        if (mHead != null) {
            mHead.prev = null;
        } else {
            mTail = null;
        }
        mSize--;
        return it;
    }
    public Item removeLast() {
        removePrecheck();
        Item it = mTail.item;
        mTail = mTail.prev;
        if (mTail != null) {
            mTail.next = null;
        } else if (mTail == null) {
            mHead = null;
        }
        mSize--;
        return it;
    }
    public Iterator<Item> iterator() {
        return new DequeIterator(mHead);
    }
    private class DequeIterator implements Iterator<Item> {
        private Node mCurrent;
        public DequeIterator(Node head) {
            mCurrent = head;
        }
        public boolean hasNext() {
            return mCurrent != null;
        }
        public void remove() {
            throw new java.lang.UnsupportedOperationException();
        }
        public Item next() {
            if (!hasNext()) {
                throw new java.util.NoSuchElementException();
            }
            Item it = mCurrent.item;
            mCurrent = mCurrent.next;
            return it;
        }

    }
    // test code
    private static String expect(boolean b) {
        if (b) {
            return "PASS";
        } else {
            return "fail";
        }
    }
    private static void case0() {
        Deque<Integer> dq = new Deque<Integer>();
        System.out.printf("empty check: %s\n", expect(dq.isEmpty()));
        dq.addFirst(1);
        dq.addFirst(2);
        dq.addLast(3);
        int exsize = 3;
        System.out.printf("check size: %d == %d %s\n",
                dq.size(), exsize, expect(dq.size() == exsize));
        int ex = 2;
        int rt = dq.removeFirst();
        System.out.printf("check val: %d == %d %s\n",
                rt, ex, expect(rt == ex));
        ex = 3;
        rt = dq.removeLast();
        System.out.printf("check val: %d == %d %s\n",
                rt, ex, expect(rt == ex));
        ex = 1;
        rt = dq.removeLast();
        System.out.printf("check val: %d == %d %s\n",
                rt, ex, expect(rt == ex));
        System.out.printf("empty check: %s\n", expect(dq.isEmpty()));
    }
    private static boolean testFF() {
        Deque<Integer> dq = new Deque<Integer>();
        int i, n = 3;
        for (i = 0; i != n; i++) {
            dq.addFirst(i);
        }
        for (i = 0; i != n; i++) {
            int rt  = dq.removeFirst();
            int ex = n - 1 - i;
            System.out.printf("check val: %d == %d %s\n",
                    rt, ex, expect(rt == ex));
            if (rt != ex) {
                return false;
            }
        }
        return true;
    }
    private static boolean testLL() {
        Deque<Integer> dq = new Deque<Integer>();
        int i, n = 3;
        for (i = 0; i != n; i++) {
            dq.addLast(i);
        }
        dq.show();
        for (i = 0; i != n; i++) {
            int rt  = dq.removeLast();
            int ex = n - 1 -i;
            System.out.printf("check val: %d == %d %s\n",
                    rt, ex, expect(rt == ex));
            if (rt != ex) {
                return false;
            }
        }
        return true;
    }
    private static boolean testLF() {
        Deque<Integer> dq = new Deque<Integer>();
        int i, n = 3;
        for (i = 0; i != n; i++) {
            dq.addLast(i);
        }
        dq.show();
        for (i = 0; i != n; i++) {
            int rt  = dq.removeFirst();
            int ex = i;
            System.out.printf("check val: %d == %d %s\n",
                    rt, ex, expect(rt == ex));
            if (rt != ex) {
                return false;
            }
        }
        return true;
    }
    private static boolean testFL() {
        Deque<Integer> dq = new Deque<Integer>();
        int i, n = 3;
        for (i = 0; i != n; i++) {
            dq.addFirst(i);
        }
        for (i = 0; i != n; i++) {
            int rt  = dq.removeLast();
            int ex = i;
            System.out.printf("check val: %d == %d %s\n",
                    rt, ex, expect(rt == ex));
            if (rt != ex) {
                return false;
            }
        }
        return true;
    }
    private static void testIter() {
        Deque<Integer> dq = new Deque<Integer>();
        int i, n = 10;
        for (i = 0; i != n; i++) {
            dq.addFirst(i);
        }
        dq.show();
        Iterator<Integer> iter = dq.iterator();
        int ex = n - 1;
        while (iter.hasNext()) {
            int rt = iter.next();
            System.out.printf("check val: %d == %d %s\n",
                    rt, ex, expect(rt == ex));
            ex--;
        }
    }
    public static void main(String[] args)
    {
        case0();
        testFF();
        testFL();
        testLL();
        testLF();
        testIter();
    }
}
