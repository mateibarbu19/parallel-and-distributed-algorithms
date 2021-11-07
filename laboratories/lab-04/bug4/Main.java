package bug4;

/**
 * Why doesn't this program end? (Hint: volatile)
 * Fix the problem changing a single line of code.
 */
public class Main extends Thread {
    volatile boolean keepRunning = true;

    public void run() {
        long count = 0;
        while (keepRunning) {
            count++;
        }

        System.out.println("Thread terminated with count:" + count + ".");
    }

    public static void main(String[] args) throws InterruptedException {
        Main t = new Main();
        t.start();
        Thread.sleep(1000);
        t.keepRunning = false;
        System.out.println("keepRunning set to false.");
    }
}
