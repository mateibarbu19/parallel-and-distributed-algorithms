package task5;

public class Sequential {
    static int N = 10;
    static int COLORS = 3;
    static int[][] graph = { { 0, 1 }, { 0, 4 }, { 0, 5 }, { 1, 0 }, { 1, 2 }, { 1, 6 }, { 2, 1 }, { 2, 3 }, { 2, 7 },
            { 3, 2 }, { 3, 4 }, { 3, 8 }, { 4, 0 }, { 4, 3 }, { 4, 9 }, { 5, 0 }, { 5, 7 }, { 5, 8 }, { 6, 1 },
            { 6, 8 }, { 6, 9 }, { 7, 2 }, { 7, 5 }, { 7, 9 }, { 8, 3 }, { 8, 5 }, { 8, 6 }, { 9, 4 }, { 9, 6 },
            { 9, 7 } };
    static Boolean success = false;

    static void colorGraph(final int[] colors, final int step) {
        if (step == N) {
            printColors(colors);
            success = true;
            return;
        }

        // for the node at position step try all possible colors
        for (int i = 0; i < COLORS; i++) {
            final int[] newColors = colors.clone();
            newColors[step] = i;
            if (verifyColors(newColors, step))
                colorGraph(newColors, step + 1);
        }
    }

    private static boolean verifyColors(final int[] colors, final int step) {
        for (int i = 0; i < step; i++) {
            if (colors[i] == colors[step] && isEdge(i, step))
                return false;
        }
        return true;
    }

    private static boolean isEdge(final int a, final int b) {
        for (final int[] ints : graph) {
            if (ints[0] == a && ints[1] == b)
                return true;
        }
        return false;
    }

    static void printColors(final int[] colors) {
        final StringBuilder aux = new StringBuilder();
        for (final int color : colors) {
            aux.append(color).append(" ");
        }
        System.out.println(aux);
    }

    public static void main(final String[] args) {
        final int[] colors = new int[N];
        colorGraph(colors, 0);

        if (success) {
            System.out.println("The graph can be colored successfully.");
        } else {
            System.out.println("The graph cannot be colored successfully.");
        }
    }
}
