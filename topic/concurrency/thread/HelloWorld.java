public class HelloWorld {
    public static void main(String[] args)
        throws InterruptedException
    {
        Thread myThread = new Thread() {
            public void run() {
                System.out.println("hello from new thread");
            }
        };
        myThread.start();
        myThread.yield(); // yield to TRY to schedule on this thread

        // Thread.sleep(1);
        System.out.println("hello from main thread");

        myThread.join();
    }
}
