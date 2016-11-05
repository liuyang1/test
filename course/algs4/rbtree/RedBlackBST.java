public class RedBlackBST<Key extends Comparable<Key>, Value> {
    private static final boolean RED = true;
    private static final boolean BLACK = false;
    private class Node {
        Key key;
        Value val;
        int N;
        boolean color;
        Node left, right;
        Node(Key key, Value val, int N, boolean color) {
            this.key = key;
            this.val = val;
            this.N = N;
            this.color = color;
        }
    }
    private int size(Node x) {
        if (x == null) {
            return 0;
        }
        return x.N;
    }
    private boolean isRed(Node x) {
        if (x == null) {
            return false;
        }
        return x.color == RED;
    }
    private Node rotateLeft(Node h) {
        Node x = h.right;
        h.right = x.left;
        x.left = h;
        x.color = h.color;
        h.color = RED;
        x.N = h.N;
        h.N = 1 + size(h.left) + size(h.right);
        return x;
    }
    private Node rotateRight(Node h) {
        Node x = h.left;
        h.left = x.right;
        x.right = h;
        x.color = h.color;
        h.color = RED;
        x.N = h.N;
        h.N = 1 + size(h.left) + size(h.right);
        return x;
    }
    private void flipColors(Node h) {
        h.color = RED;
        h.left.color = BLACK;
        h.right.color = BLACK;
    }
    private int height(Node h) {
        if (h == null) {
            return 0;
        }
        return Math.max(height(h.left), height(h.right)) + 1;
    }
    private Node root;
    public RedBlackBST() {
        root = null;
    }
    public void put(Key key, Value val) {
        root = put(root, key, val);
        root.color = BLACK;
    }
    private Node put(Node h, Key key, Value val) {
        if (h == null) {
            return new Node(key, val, 1, RED);
        }
        int cmp = key.compareTo(h.key);
        if (cmp < 0) {
            h.left = put(h.left, key, val);
        } else if (cmp > 0) {
            h.right = put(h.right, key, val);
        } else {
            h.val = val;
        }

        // keep red-black tree
        if (isRed(h.right) && !isRed(h.left)) {
            h = rotateLeft(h);
        }
        if (isRed(h.left) && isRed(h.left.left)) {
            h = rotateRight(h);
        }
        if (isRed(h.left) && isRed(h.right)) {
            flipColors(h);
        }
        h.N = size(h.left) + size(h.right) + 1;
        return h;
    }
    public Value get(Key key) {
        return get(root, key);
    }
    private Value get(Node h, Key key) {
        if (h == null) {
            return null;
        }
        int cmp = key.compareTo(h.key);
        if (cmp < 0) {
            return get(h.left, key);
        } else if (cmp > 0) {
            return get(h.right, key);
        } else {
            return h.val;
        }
    }
    public boolean contains(Key key) {
        return contains(root, key);
    }
    private boolean contains(Node h, Key key) {
        if (h == null) {
            return false;
        }
        int cmp = key.compareTo(h.key);
        switch(cmp) {
            case -1: return contains(h.left, key);
            case +1: return contains(h.right, key);
            case 0: return true;
            default: return false;
        }
    }
    public void leadingSpace(int level) {
        for (int i = 0; i != level; i++) {
            System.out.printf("    ");
        }
    }
    public void show(Node h, int level) {
        if (h == null) {
            leadingSpace(level);
            System.out.printf("nil\n");
        } else {
            leadingSpace(level);
            System.out.printf("key=%s val=%s N=%d color=%s\n",
                              h.key, h.val, h.N, h.color ? "RED" : "BLACK");
            show(h.left, level + 1);
            show(h.right, level + 1);
        }
    }
    public void show() {
        show(root, 0);
    }
    public int height() {
        return height(root);
    }
    public int size() {
        return size(root);
    }
    public boolean isEmpty() {
        return size(root) == 0;
    }
    // TEST code
    // - red link lean left
    // - No red node connected to each other
    // - Black Height is equal for all leaf node
    public boolean testHeight() {
        int ht = height();
        int sz = size();
        // ht <= 2 * lg sz
        // 2 ** (ht / 2) <= sz
        boolean ret = 1 << (height() / 2) <= size();
        if (ret == false) {
            System.out.printf("testHeight fail\n");
            System.out.printf("height=%d size=%d\n", ht, sz);
        }
        return ret;
    }
    public boolean testProperty() {
        testHeight();
        return true;
    }


    public static void main(String[] args) {
        RedBlackBST<Integer, String> t = new RedBlackBST<Integer, String>();
        for (int i = 0; i != 24; i++) {
            t.put(i, Integer.toString(i));
        }
        t.show();
        t.testProperty();

        int key = 21;
        boolean cont = t.contains(key);
        String val = t.get(key);
        System.out.printf("key=%d cont=%s val=%s\n", key, cont, val);

        key = 30;
        cont = t.contains(key);
        val = t.get(key);
        System.out.printf("key=%d cont=%s val=%s\n", key, cont, val);

        return;
    }
}
