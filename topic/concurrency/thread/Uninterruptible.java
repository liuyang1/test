public class Uninterruptible {
    public static void main(String[] args) throws InterruptedException {
        final Object obj0 = new Object();
        final Object obj1 = new Object();
        Thread t0 = new Thread() {
            public void run() {
                try {
                    synchronized(obj0) {
                        Thread.sleep(1000);
                        synchronized(obj1) {}
                    }
                } catch (InterruptedException e) {
                    System.out.println("t0 interrupted");
                }
            }
        };
        Thread t1 = new Thread() {
            public void run() {
                try {
                    synchronized(obj1) {
                        Thread.sleep(1000);
                        synchronized(obj0) {}
                    }
                } catch (InterruptedException e) {
                    System.out.println("t0 interrupted");
                }
            }
        };

        t0.start();
        t1.start();
        Thread.sleep(3000);
        t1.interrupt();
        t0.interrupt();
        t1.join();
        t0.join();

    }
}
