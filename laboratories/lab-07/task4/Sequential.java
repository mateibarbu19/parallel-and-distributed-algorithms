package task4;

import java.util.ArrayList;

public class Sequential {
    static int[][] graph = { { 0, 1 }, { 0, 4 }, { 0, 5 }, { 1, 0 }, { 1, 2 }, { 1, 6 }, { 2, 1 }, { 2, 3 }, { 2, 7 },
            { 3, 2 }, { 3, 4 }, { 3, 8 }, { 4, 0 }, { 4, 3 }, { 4, 9 }, { 5, 0 }, { 5, 7 }, { 5, 8 }, { 6, 1 },
            { 6, 8 }, { 6, 9 }, { 7, 2 }, { 7, 5 }, { 7, 9 }, { 8, 3 }, { 8, 5 }, { 8, 6 }, { 9, 4 }, { 9, 6 },
            { 9, 7 } };
    static Boolean success = false;

    static void getPath(final ArrayList<Integer> partialPath, final int destination) {
        if (partialPath.get(partialPath.size() - 1) == destination) {
            System.out.println(partialPath);
            success = true;
            return;
        }

        // se verifica nodurile pentru a evita ciclarea in graf
        final int lastNodeInPath = partialPath.get(partialPath.size() - 1);
        for (final int[] ints : graph) {
            if (ints[0] == lastNodeInPath) {
                if (partialPath.contains(ints[1]))
                    continue;
                final ArrayList<Integer> newPartialPath = new ArrayList<>(partialPath);
                newPartialPath.add(ints[1]);
                getPath(newPartialPath, destination);
            }
        }
    }

    public static void main(final String[] args) {
        final ArrayList<Integer> partialPath = new ArrayList<>();
        partialPath.add(0);
        getPath(partialPath, 3);

        if (success) {
            System.out.println("The path finding was successful.");
        } else {
            System.out.println("The path finding was not successful");
        }
    }
}
