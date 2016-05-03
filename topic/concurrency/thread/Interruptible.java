import java.util.concurrent.locks.ReentrantLock;

public class Interruptible {
    public static void main(String[] args) throws InterruptedException {
        final ReentrantLock loc0 = new ReentrantLock();
        final ReentrantLock loc1 = new ReentrantLock();
        Thread t0 = new Thread() {
            public void run() {
                try {
                    loc0.lockInterruptibly();
                    Thread.sleep(1000);
                    loc1.lockInterruptibly();
                } catch (InterruptedException e) {
                    System.out.println("t0 interrupted");
                }
            }
        };
        Thread t1 = new Thread() {
            public void run() {
                try {
                    loc1.lockInterruptibly();
                    Thread.sleep(1000);
                    loc0.lockInterruptibly();
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
