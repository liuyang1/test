public class Counting {
    private static class Counter {
        private int count = 0;
        // keep sync to write or read on global count
        public synchronized void increment() { count++; }
        public synchronized int getCount() { return count; }
    }

    private static final Counter counter = new Counter();

    private static class CountingThread extends Thread {
        public void run() {
            for (int x = 0; x < 1000; x++) {
                counter.increment();
            }
        }
    }

    public static void main(String[] args)
        throws InterruptedException
    {
        CountingThread t0 = new CountingThread();
        CountingThread t1 = new CountingThread();

        t0.start();
        t1.start();
        t0.join();
        t1.join();

        System.out.println(counter.getCount());
    }
}
