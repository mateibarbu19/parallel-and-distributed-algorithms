import java.io.RandomAccessFile;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.StringTokenizer;

public class MapDoc extends MapOperation<String, InitialData, IntermediateResult> {
    private static final String Delimitators = ";:/?~\\.,><`[]{}()!@#$%^&-_+'=*\"| \t\r\n\0";

    public MapDoc(String documentPath, InitialData initialData) {
        super(documentPath, initialData);
    }

    @Override
    public IntermediateResult call() throws Exception {
        String documentPath = key;
        final int fragmentDimension = value.getFragmentDimension();
        final long position = value.getPosition();

        RandomAccessFile document = new RandomAccessFile(documentPath, "r");
        
        document.seek(position + fragmentDimension - 1);
        int ch = document.read();
        while (ch != -1 && Delimitators.indexOf(ch) == -1) {
            ch = document.read();
        }
        long endPosition = document.getFilePointer();

        boolean skipFirstToken = false;
        // if the position is the middle of a token, move it right
        if (position > 0) {
            document.seek(position - 1);
            int before = document.read();
            int first = document.read();
            if (Delimitators.indexOf(before) == -1 && Delimitators.indexOf(first) == -1) {
                skipFirstToken = true;
            }
        }
        document.seek(position);

        byte[] buffer = new byte[(int) (endPosition - position)];
        document.read(buffer);
        document.close();
        
        String fragment = new String(buffer);
        StringTokenizer tokenizer = new StringTokenizer(fragment, Delimitators);
        if (skipFirstToken) {
            tokenizer.nextToken();
        }

        final Map<Integer, Integer> wordCount = new HashMap<>();
        final List<String> longestWords = new LinkedList<>();
        int maxWordLength = Integer.MIN_VALUE;
        while (tokenizer.hasMoreTokens()) {
            String token = tokenizer.nextToken();
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

        return new IntermediateResult(wordCount, longestWords);
    }
    
}
