import java.util.ArrayList;

public class Fibonacci {
    private static final int InitialCapacity = 40;
    private static final ArrayList<Integer> sequence = new ArrayList<>() {
        {
            add(0);
            add(1);
            ensureCapacity(InitialCapacity);
        }
    };

    private static synchronized void cacheMoreValues(final int max) {
        sequence.ensureCapacity(max + 1);

        int beforeLast = sequence.get(sequence.size() - 2);
        int last = sequence.get(sequence.size() - 1);
        for (int i = sequence.size(); i <= max; i++) {
            final int newValue = beforeLast + last;
            beforeLast = last;
            last = newValue;
            sequence.add(newValue);
        }
    }

    public static synchronized int get(final int index) {
        try {
            return sequence.get(index);
        } catch (final IndexOutOfBoundsException e) {
            if (index < 0) {
                return 0;
            } else {
                cacheMoreValues(index);
                return sequence.get(index);
            }
        }
    }
}
