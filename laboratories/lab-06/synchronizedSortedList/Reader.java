package synchronizedSortedList;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.List;
import java.util.Scanner;

public class Reader extends Thread {
    private final String filename;
    private final List<Integer> list;

    public Reader(String filename, List<Integer> list) {
        super();
        this.filename = filename;
        this.list = list;
    }

    @Override
    public void run() {
        try {
            Scanner scanner = new Scanner(new File(filename));
            while (scanner.hasNextInt()) {
                list.add(scanner.nextInt());
            }
        } catch (FileNotFoundException e) {
            System.err.println("[Reader]: File not found!");
            e.printStackTrace();
        }
        Main.sem.release();
    }
}
