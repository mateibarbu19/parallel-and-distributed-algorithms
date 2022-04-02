import java.io.RandomAccessFile;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import data.InitialData;
import data.IntermediateResult;
import mapReduce.MapOperation;
import mapReduce.MapReduceEntry;

import java.util.StringTokenizer;

public class MapDoc extends MapOperation<String, InitialData, String, IntermediateResult> {
    private static final String Delimitators = ";:/?~\\.,><`[]{}()!@#$%^&-_+'=*\"| \t\r\n\0";

    public MapDoc(final String documentPath, final InitialData initialData) {
        super(documentPath, initialData);
    }

    @Override
    public List<Entry<String, IntermediateResult>> call() throws Exception {
        final String documentPath = key;
        final int fragmentDimension = value.getFragmentDimension();
        final long position = value.getPosition();

        final RandomAccessFile document = new RandomAccessFile(documentPath, "r");

        document.seek(position + fragmentDimension - 1);
        int ch = document.read();
        while (ch != -1 && Delimitators.indexOf(ch) == -1) {
            ch = document.read();
        }
        final long endPosition = document.getFilePointer();

        boolean skipFirstToken = false;
        // if the position is the middle of a token, move it right
        if (position > 0) {
            document.seek(position - 1);
            final int before = document.read();
            final int first = document.read();
            if (Delimitators.indexOf(before) == -1 && Delimitators.indexOf(first) == -1) {
                skipFirstToken = true;
            }
        }
        document.seek(position);

        final byte[] buffer = new byte[(int) (endPosition - position)];
        document.read(buffer);
        document.close();

        final String fragment = new String(buffer);
        final StringTokenizer tokenizer = new StringTokenizer(fragment, Delimitators);
        if (skipFirstToken) {
            tokenizer.nextToken();
        }

        final Map<Integer, Integer> wordCount = new HashMap<>();
        final List<String> longestWords = new LinkedList<>();
        int maxWordLength = Integer.MIN_VALUE;
        while (tokenizer.hasMoreTokens()) {
            final String token = tokenizer.nextToken();
            if (token.length() > maxWordLength) {
                maxWordLength = token.length();
                longestWords.clear();
            }
            if (token.length() == maxWordLength) {
                longestWords.add(token);
            }
            wordCount.putIfAbsent(token.length(), 0);
            wordCount.compute(token.length(), (k, v) -> v + 1);
        }

        return new LinkedList<>() {
            {
                add(new MapReduceEntry<>(documentPath, new IntermediateResult(wordCount, longestWords)));
            }
        };
    }

}
