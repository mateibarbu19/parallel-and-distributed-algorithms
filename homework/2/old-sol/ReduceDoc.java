import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class ReduceDoc extends ReduceOperation<String, IntermediateResult, Result> {
    /**
     * @param documentPath
     * @param intermediates
     */
    public ReduceDoc(String documentPath, List<IntermediateResult> intermediates) {
        super(documentPath, intermediates);
    }

    @Override
    public Result call() throws Exception {
        String documentPath = key;
        List<IntermediateResult> intermediates = values;

        Map <Integer, Integer> totalWordCount = new HashMap<>();
        int maxLength = Integer.MIN_VALUE;
        int maxLengthCount = 0;

        for (IntermediateResult interRes : intermediates) {
            for (Map.Entry<Integer, Integer> entry : interRes.getWordCount().entrySet()) {
                totalWordCount.putIfAbsent(entry.getKey(), 0);
                totalWordCount.compute(entry.getKey(), (k, v) -> entry.getValue() + v);
            }
            int fragmentLength = interRes.getLongestWords().get(0).length();
            if (fragmentLength > maxLength) {
                maxLength = fragmentLength;
                maxLengthCount = 0;
            }
            if (fragmentLength == maxLength) {
                maxLengthCount += interRes.getLongestWords().size();
            }
        }
        Fibonacci.get(maxLength + 1);
        
        double rank = 0;
        long totalWords = 0;
        for (Map.Entry<Integer, Integer> entry : totalWordCount.entrySet()) {
            rank += Fibonacci.get(entry.getKey() + 1) * entry.getValue();
            totalWords += entry.getValue();
        }
        rank /= totalWords;
        
        return new Result(documentPath, rank, maxLength, maxLengthCount);
    }
}
