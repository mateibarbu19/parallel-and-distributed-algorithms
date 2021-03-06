package task3;

public class Sequential {
    public static int N = 4;

    private static boolean check(final int[] arr, final int step) {
        for (int i = 0; i <= step; i++) {
            for (int j = i + 1; j <= step; j++) {
                if (arr[i] == arr[j] || arr[i] + i == arr[j] + j || arr[i] + j == arr[j] + i)
                    return false;
            }
        }
        return true;
    }

    private static void printQueens(final int[] sol) {
        StringBuilder aux = new StringBuilder();
        for (int i = 0; i < sol.length; i++) {
            aux.append("(").append(sol[i] + 1).append(", ").append(i + 1).append("), ");
        }
        aux = new StringBuilder(aux.substring(0, aux.length() - 2));
        System.out.println("[" + aux + "]");
    }

    public static void queens(final int[] graph, final int step) {
        if (Sequential.N == step) {
            printQueens(graph);
            return;
        }
        for (int i = 0; i < Sequential.N; ++i) {
            final int[] newGraph = graph.clone();
            newGraph[step] = i;

            if (check(newGraph, step)) {
                queens(newGraph, step + 1);
            }
        }
    }

    public static void main(final String[] args) {
        final int[] graph = new int[N];
        queens(graph, 0);
    }
}
