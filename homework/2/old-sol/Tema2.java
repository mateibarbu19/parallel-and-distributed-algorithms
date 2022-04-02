import java.io.*;
import java.util.*;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.stream.Collectors;

public class Tema2 {
    public static void main(String[] args) {
        if (args.length < 3) {
            System.err.println("Usage: Tema2 <workers> <in_file> <out_file>");
            return;
        }

        final int numberOfWorkers = Integer.parseInt(args[0]);
        File input = new File(args[1]);
        Scanner scanner = null;

        try {
            scanner = new Scanner(input);
        } catch (FileNotFoundException e) {
            System.err.println("Input file not found!");
            e.printStackTrace();
            System.exit(1);
        }

        final ExecutorService mapPool = Executors.newFixedThreadPool(numberOfWorkers);

        final int fragmentDimension = scanner.nextInt();
        final int numberOfDocuments = scanner.nextInt();
        final String[] documentPaths = new String[numberOfDocuments];
        final List<List<Future<IntermediateResult>>> futureIntermediates = new ArrayList<>();

        int i = 0;
        while (i < numberOfDocuments && scanner.hasNext()) {
            documentPaths[i] = scanner.next();
            File document = new File(documentPaths[i]);
            List<Future<IntermediateResult>> list = new LinkedList<>();
            for (long pos = 0; pos <= document.length(); pos += fragmentDimension) {
                list.add(
                    mapPool.submit(new MapDoc(documentPaths[i], new InitialData(fragmentDimension, pos)))
                );
            }
            futureIntermediates.add(list);

            i++;
        }
        scanner.close();
        if (i != numberOfDocuments) {
            System.err.println("Could not read all the paths for the number of documents specified in the input file.");
            System.exit(2);
        }

        mapPool.shutdown();

        while (!mapPool.isTerminated()) {
            try {
                Thread.sleep(500);
            } catch (final InterruptedException e) {
                System.err.println("Main thread was interrupted!");
                e.printStackTrace();
                System.exit(3);
            }
        }

        final ExecutorService reducePool = Executors.newFixedThreadPool(numberOfWorkers);
        final List<Future<Result>> futureResults = new ArrayList<>();
        for (i = 0; i < numberOfDocuments; i++) {
            List<Future<IntermediateResult>> fIntermediates = futureIntermediates.get(i);
            List<IntermediateResult> intermediates = fIntermediates.stream().map(t -> {
                try {
                    return t.get();
                } catch (final InterruptedException | ExecutionException e) {
                    e.printStackTrace();
                }
                return null;
            }).filter((e) -> !e.getLongestWords().isEmpty()).collect(Collectors.toList());

            futureResults.add(
                reducePool.submit(new ReduceDoc(documentPaths[i], intermediates))
            );
        }

        reducePool.shutdown();

        while (!reducePool.isTerminated()) {
            try {
                Thread.sleep(500);
            } catch (final InterruptedException e) {
                System.err.println("Main thread was interrupted!");
                e.printStackTrace();
                System.exit(4);
            }
        }

        List<Result> results = futureResults.stream().map(f -> {
            try {
                return f.get();
            } catch (final InterruptedException | ExecutionException e) {
                e.printStackTrace();
            }
            return null;
        }).sorted().collect(Collectors.toList());

        FileWriter fWriter = null;
        try {
            fWriter = new FileWriter(args[2]);    
        }
        catch (IOException e) {
            System.err.println("Could not open the output file!");
            System.exit(5);
        }

        for (i = 0; i < numberOfDocuments; i++) {
            try {
                fWriter.write(results.get(i).toOutputFormat() + "\n");
            } catch (IOException e) {
                System.err.println("Could not write to the output file!");
                System.exit(6);
            }
        }
        try {
            fWriter.close();
        } catch (IOException e) {
            System.err.println("Could not close to the output file!");
            System.exit(7);
        }
    }
}
