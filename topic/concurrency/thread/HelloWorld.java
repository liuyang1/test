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
        Thread.yield();

        System.out.println("hello from main thread");

        myThread.join();
    }
}
