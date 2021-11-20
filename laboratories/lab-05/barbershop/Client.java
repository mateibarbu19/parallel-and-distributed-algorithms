package barbershop;

import java.util.concurrent.Semaphore;

public class Client extends Thread {
    private final int iden;
    private final Semaphore sem;

    public int getIden() {
        return iden;
    }

    public Semaphore getSem() {
        return sem;
    }

    public Client(final int iden) {
        super();
        this.iden = iden;
        sem = new Semaphore(0);
    }

    @Override
    public void run() {
        boolean entered = false;
        Main.leftClients[iden] = Main.UNSERVED_CLIENT;
        while (!entered) {
            Main.lock.lock();
            System.out.println("Client " + iden + " arrived at the barbershop door.");
            // Can I sit in the waiting room?
            if (Main.availableChairs == 0) {
                Main.lock.unlock();
                System.out.println("Client " + iden + " left unserved.");
            }
            else {
                entered = true;
            }
        }

        // take a place in the waiting room
        Main.queue.add(this);
        Main.availableChairs--;
        System.out.print("Client " + iden + " has taken a seat in the waiting room. ");
        System.out.println("Available seats: " + Main.availableChairs);
        Main.lock.unlock();

        // signal the barber that I am ready
        Main.customer.release();
        try {
            // wait for him to call me
            sem.acquire();
        } catch (final InterruptedException e) {
            Main.lock.lock();
            Main.availableChairs++;
            Main.lock.unlock();
            System.err.println("I was kicked out of the waiting room!");
            e.printStackTrace();
        }
        
        System.out.println("Client " + iden + " is being served by the barber.");
        Main.leftClients[iden] = Main.SERVED_CLIENT;

        try {
            // handshake between customer and barber
            // is the barber done?
            Main.barberDone.acquire();

            // then I am leaving
            Main.lock.lock();
            Main.servedClients++;
            Main.availableChairs++;
            Main.lock.unlock();
            
            System.out.println("Client " + iden + " left the barbershop.");
            Main.customerDone.release();
        } catch (final InterruptedException e) {
            System.err.println("I was kicked out of the barber chair!");
            Main.lock.lock();
            Main.availableChairs++;
            Main.lock.unlock();
            e.printStackTrace();
        }
    }
}
