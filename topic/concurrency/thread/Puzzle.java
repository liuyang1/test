public class Puzzle {
    static boolean answerReady = false;
    static int answer = 0;
    static Thread t0 = new Thread() {
        public void run() {
            answer = 42;
            answerReady = true;
        }
    };

    static Thread t1 = new Thread() {
        public void run() {
            if (answerReady) {
                System.out.println("The meaning of life is : " + answer);
            } else {
                System.out.println("I don't know the answer.");
            }
        }
    };

    public static void main(String[] args)
        throws InterruptedException
    {
        // This example is bad.
        // cannot get "The meaning of life is : 0" result.
        // understand this is possible. But I cannot get this reuslt.
        t0.start();
        t1.start();
        t1.join();
        t0.join();
    }
}
