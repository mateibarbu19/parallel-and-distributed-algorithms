package philosophersProblem;

public class Philosopher implements Runnable {
    private final Object leftFork;
    private final Object rightFork;
    private final int id;

    public Philosopher(final int id, final Object leftFork, final Object rightFork) {
        this.leftFork = leftFork;
        this.rightFork = rightFork;
        this.id = id;
    }

    private void sleep() {
        try {
            Thread.sleep(100);
        } catch (final InterruptedException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void run() {
        if (id % 2 == 1) {
            synchronized (leftFork) {
                sleep(); // delay added to make sure the dead-lock is visible
                synchronized (rightFork) {
                    System.out.println("Philosopher " + id + " is eating");
                }
            }
        }
        else {
            synchronized (rightFork) {
                sleep(); // delay added to make sure the dead-lock is visible
                synchronized (leftFork) {
                    System.out.println("Philosopher " + id + " is eating");
                }
            }
        }
    }
}
