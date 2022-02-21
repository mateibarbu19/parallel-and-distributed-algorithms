package data;

import java.nio.file.Paths;

public class Result implements Comparable<Result> {
    private final String documentPath;
    private final double rank;
    private final int lengthOfLongestWord;
    private final int numberOfLongestWords;

    /**
     * @param documentPath
     * @param rank
     * @param lengthOfLongestWord
     * @param numberOfLongestWords
     */
    public Result(final String documentPath, final double rank, final int lengthOfLongestWord,
            final int numberOfLongestWords) {
        this.documentPath = documentPath;
        this.rank = rank;
        this.lengthOfLongestWord = lengthOfLongestWord;
        this.numberOfLongestWords = numberOfLongestWords;
    }

    /**
     * @return the documentPath
     */
    public String getDocumentPath() {
        return documentPath;
    }

    /**
     * @return the rank
     */
    public double getRank() {
        return rank;
    }

    /**
     * @return the lengthOfLongestWord
     */
    public int getLengthOfLongestWord() {
        return lengthOfLongestWord;
    }

    /**
     * @return the numberOfLongestWords
     */
    public int getNumberOfLongestWords() {
        return numberOfLongestWords;
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.lang.Object#toString()
     */
    @Override
    public String toString() {
        return "Result [documentPath=" + documentPath + ", lengthOfLongestWord=" + lengthOfLongestWord
                + ", numberOfLongestWords=" + numberOfLongestWords + ", rank=" + rank + "]";
    }

    public String toOutputFormat() {
        return Paths.get(documentPath).getFileName() + "," + String.format("%.2f,", rank) + lengthOfLongestWord + ","
                + numberOfLongestWords;
    }

    @Override
    public int compareTo(final Result o) {
        return Double.compare(o.getRank(), rank);
    }
}
