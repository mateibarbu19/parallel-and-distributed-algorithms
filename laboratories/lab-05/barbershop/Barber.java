package barbershop;

import java.util.NoSuchElementException;
import java.util.concurrent.Semaphore;

public class Barber extends Thread {
    @Override
    public void run() {
        do {
            try {
                // Are there any customers available?
                Main.customer.acquire();
                Main.lock.lock();
                try {
                    final Client c = Main.queue.remove();
                    final Semaphore sem = c.getSem(); 
                    Main.lock.unlock();

                    // Signal the client to come sit in the barber chair
                    sem.release();
                    
                    try {
                        System.out.println("Barber is cutting client's " + c.getIden() + " hair.");
                        // The time necessary to cut hair
                        Thread.sleep(100);
                    } catch (final InterruptedException e) {
                        e.printStackTrace();
                    }
    
                    System.out.println("Barber served client " + c.getIden() + ".");

                    // the barber is done with the current client
                    Main.barberDone.release();
                    try {
                        // Are you satisfied?
                        Main.customerDone.acquire();
                    } catch (final InterruptedException e) {
                        System.err.println("The client left unsatisfied!");
                        e.printStackTrace();
                    }
                } catch (final NoSuchElementException e) {
                    Main.lock.unlock();
                    System.err.println("The client that was waiting disappeared!");
                }
            } catch (final InterruptedException e1) {
                System.out.println("No client has arrived for a long time.");
                e1.printStackTrace();
            }
        } while (Main.areThereUnCutPeople());
    }
}
