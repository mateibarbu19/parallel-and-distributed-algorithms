import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.List;
import java.util.Scanner;
import java.util.stream.Collectors;

public class Tema2 {
    public static void main(final String[] args) {
        if (args.length < 3) {
            System.err.println("Usage: Tema2 <workers> <in_file> <out_file>");
            return;
        }

        final int numberOfWorkers = Integer.parseInt(args[0]);
        final File input = new File(args[1]);
        Scanner scanner = null;

        try {
            scanner = new Scanner(input);
        } catch (final FileNotFoundException e) {
            System.err.println("Input file not found!");
            e.printStackTrace();
            System.exit(1);
        }

        final MasterMapReducer<String, InitialData, String, IntermediateResult, Result> master = new MasterMapReducer<>(
                numberOfWorkers);

        final int fragmentDimension = scanner.nextInt();
        final int numberOfDocuments = scanner.nextInt();
        final String[] documentPaths = new String[numberOfDocuments];

        int i = 0;
        while (i < numberOfDocuments && scanner.hasNext()) {
            documentPaths[i] = scanner.next();
            final File document = new File(documentPaths[i]);
            for (long pos = 0; pos <= document.length(); pos += fragmentDimension) {
                master.getMapTasks().add(
                        new MapDoc(documentPaths[i], new InitialData(fragmentDimension, pos)));
            }
            i++;
        }
        scanner.close();
        if (i != numberOfDocuments) {
            System.err.println("Could not read all the paths for the number of documents specified in the input file.");
            System.exit(2);
        }

        for (final var entry : master.runMapPhase().entrySet()) {
            master.getReduceTasks().add(
                    new ReduceDoc(entry.getKey(), entry.getValue()));
        }

        final List<Result> results = master.runReducePhase().stream().sorted().collect(Collectors.toList());

        FileWriter fWriter = null;
        try {
            fWriter = new FileWriter(args[2]);
        } catch (final IOException e) {
            System.err.println("Could not open the output file!");
            System.exit(5);
        }

        for (final var result : results) {
            try {
                fWriter.write(result.toOutputFormat() + "\n");
            } catch (final IOException e) {
                System.err.println("Could not write to the output file!");
                System.exit(6);
            }
        }

        try {
            fWriter.close();
        } catch (final IOException e) {
            System.err.println("Could not close to the output file!");
            System.exit(7);
        }
    }
}
