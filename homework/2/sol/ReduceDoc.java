import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import data.IntermediateResult;
import data.Result;
import mapReduce.ReduceOperation;
import utils.Fibonacci;

public class ReduceDoc extends ReduceOperation<String, IntermediateResult, Result> {
    /**
     * @param documentPath
     * @param intermediates
     */
    public ReduceDoc(final String documentPath, final List<IntermediateResult> intermediates) {
        super(documentPath, intermediates);
    }

    @Override
    public List<Result> call() throws Exception {
        final String documentPath = key;
        final List<IntermediateResult> intermediates = values.stream().filter((e) -> !e.getLongestWords().isEmpty())
                .collect(Collectors.toList());

        final Map<Integer, Integer> totalWordCount = new HashMap<>();
        int maxLength = Integer.MIN_VALUE;
        int maxLengthCount = 0;

        for (final IntermediateResult interRes : intermediates) {
            for (final Map.Entry<Integer, Integer> entry : interRes.getWordCount().entrySet()) {
                totalWordCount.putIfAbsent(entry.getKey(), 0);
                totalWordCount.compute(entry.getKey(), (k, v) -> entry.getValue() + v);
            }
            final int fragmentLength = interRes.getLongestWords().get(0).length();
            if (fragmentLength > maxLength) {
                maxLength = fragmentLength;
                maxLengthCount = 0;
            }
            if (fragmentLength == maxLength) {
                maxLengthCount += interRes.getLongestWords().size();
            }
        }
        Fibonacci.get(maxLength + 1);

        long sum = 0;
        long totalWords = 0;
        for (final Map.Entry<Integer, Integer> entry : totalWordCount.entrySet()) {
            sum += Fibonacci.get(entry.getKey() + 1) * entry.getValue();
            totalWords += entry.getValue();
        }
        final double rank = (double) sum / totalWords;
        final int lengthOfLongestWord = maxLength;
        final int numberOfLongestWords = maxLengthCount;

        return new LinkedList<>() {
            {
                add(new Result(documentPath, rank, lengthOfLongestWord, numberOfLongestWords));
            }
        };
    }
}
