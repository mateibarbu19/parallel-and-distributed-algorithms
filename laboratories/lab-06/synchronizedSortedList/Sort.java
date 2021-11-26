package synchronizedSortedList;

import java.util.Collections;
import java.util.List;

public class Sort extends Thread {
    private final List<Integer> list;

    public Sort(List<Integer> list) {
        super();
        this.list = list;
    }

    @Override
    public void run() {
        Main.sem.acquireUninterruptibly(Main.NUMBER_OF_THREADS - 1);
        Collections.sort(list);
    }
}
