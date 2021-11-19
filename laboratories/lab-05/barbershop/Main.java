package barbershop;

import java.util.LinkedList;
import java.util.Queue;
import java.util.concurrent.Semaphore;
import java.util.concurrent.locks.ReentrantLock;

public class Main {
    public final static int TOTAL_CHAIRS = 3;
    public final static int TOTAL_CLIENTS = 7;
    
    public static int availableChairs = TOTAL_CHAIRS;
    public static int servedClients = 0;
    // a client can leave the shop served
    public final static int SERVED_CLIENT = 1;
    // a client can leave the shop unserved, as the barber was too busy and there no seats available
    public final static int UNSERVED_CLIENT = 2;
    // an array which contains the served status of each client
    public static int[] leftClients = new int[TOTAL_CLIENTS];

    public static Queue<Client> queue = new LinkedList<Client>();

    // see Little Book Of Semaphores, FIFO barbershop
    // the queue and availableChairs are protected by this lock
    public static ReentrantLock lock = new ReentrantLock();
    // the next semaphore signals if 
    public static Semaphore customer = new Semaphore(0);
    public static Semaphore customerDone = new Semaphore(0);
    public static Semaphore barberDone = new Semaphore(0);


    public static void main(String[] args) throws InterruptedException {
        Thread barberThread = new Barber();
        Thread[] clientThreads = new Client[TOTAL_CLIENTS];

        for (int i = 0; i < TOTAL_CLIENTS; i++) {
            clientThreads[i] = new Client(i);
        }

        barberThread.start();
        for (Thread clientThread : clientThreads) {
            clientThread.start();
            Thread.sleep(100);
        }

        barberThread.join();
        for (var thread: clientThreads) {
            thread.join();
        }

        int unservedClients = 0;
        for (var client: leftClients) {
            if (client == UNSERVED_CLIENT) {
                unservedClients++;
            }
        }

        System.out.println("There were " + unservedClients + " out of " + TOTAL_CLIENTS + " unserved clients");
    }

    public static boolean areThereUnCutPeople() {
        return TOTAL_CLIENTS - servedClients > 0;
    }
}
