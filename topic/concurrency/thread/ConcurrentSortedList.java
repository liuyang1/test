import java.util.concurrent.locks.ReentrantLock;
import java.util.Random;

class ConcurrentSortedList {
    private class Node {
        int value;
        Node prev;
        Node next;
        ReentrantLock lock = new ReentrantLock();

        Node() {};
        Node(int value, Node prev, Node next) {
            this.value = value;
            this.prev = prev;
            this.next = next;
        }
    }

    private final Node head;
    private final Node tail;

    public ConcurrentSortedList() {
        head = new Node();
        tail = new Node();
        head.next = tail;
        tail.prev = head;
    }

    public void insert(int value) {
        Node current = head;
        current.lock.lock();
        Node next = current.next;
        try {
            while (true) {
                next.lock.lock();
                try {
                    if (next == tail || next.value < value) {
                        Node node = new Node(value, current, next);
                        next.prev = node;
                        current.next = node;
                        return;
                    }
                } finally {
                    current.lock.unlock();
                }
                current = next;
                next = current.next;
            }
        } finally {
            next.lock.unlock();
        }
    }

    public int size() {
        Node current = tail;
        int count = 0;
        while (current.prev != head) {
            ReentrantLock lock = current.lock;
            lock.lock();
            try {
                ++count;
                current = current.prev;
            } finally {
                lock.unlock();
            }
        }
        return count;
    }

    public void show() {
        int len = size();
        System.out.printf("size=%2d [", len);
        Node current = head;
        ReentrantLock lock;
        boolean first = true;
        while (current.next != tail) {
            lock = current.lock;
            lock.lock();
            int value;
            try {
                current = current.next;
            } finally {
                lock.unlock();
            }
            if (first) {
                System.out.printf("%d", current.value);
                first = false;
            } else {
                System.out.printf(", %d", current.value);
            }
        }
        System.out.printf("]\n");
    }

    private static class Worker extends Thread {
        private int id;
        private ConcurrentSortedList lst = null;
        private Random rand;

        public Worker(int id, ConcurrentSortedList lst) {
            this.id = id;
            this.lst = lst;
            this.rand = new Random();
        }
        public void run() {
            for (int x = 0; x != 5; x++) {
                int value = rand.nextInt(1000);
                System.out.printf("worker=%d insert value=%d\n", id, value);
                lst.insert(value);
                try {
                    Thread.sleep(rand.nextInt(1000));
                } catch (InterruptedException e) {
                    System.out.printf("InterruptedException\n");
                    break;
                }
            }
        }
    }
    public static boolean testUnit() {
        ConcurrentSortedList list = new ConcurrentSortedList();
        Worker w0 = new Worker(0, list);
        Worker w1 = new Worker(1, list);
        w0.start();
        w1.start();

        try {
            for (int x = 0; x != 5; x++) {
                list.show();
                Thread.sleep(500);
            }
            w0.join();
            w1.join();
        } catch (InterruptedException e) {
            System.out.printf("join thread interrupted");
        }
        return true;
    }
    public static void main(String[] args) {
        testUnit();
        return;
    }

}
