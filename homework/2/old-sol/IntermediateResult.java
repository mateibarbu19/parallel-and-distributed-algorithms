import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

public class IntermediateResult {
    private final Map<Integer, Integer> wordCount;
    private final List<String> longestWords;

    /**
     * @param wordCount
     * @param longestWords
     */
    public IntermediateResult(final Map<Integer, Integer> wordCount, final List<String> longestWords) {
        this.wordCount = wordCount;
        this.longestWords = longestWords;
    }

    public IntermediateResult() {
        this(new HashMap<>(), new LinkedList<>());
    }

    /**
     * @return the wordCount
     */
    public Map<Integer, Integer> getWordCount() {
        return wordCount;
    }

    /**
     * @return the longestWords
     */
    public List<String> getLongestWords() {
        return longestWords;
    }

    /* (non-Javadoc)
     * @see java.lang.Object#toString()
     */
    @Override
    public String toString() {
        return "IntermediateResult [longestWords=" + longestWords + ", wordCount=" + wordCount + "]";
    }

    
}
