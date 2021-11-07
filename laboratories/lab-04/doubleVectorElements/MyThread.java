package doubleVectorElements;

public class MyThread extends Thread {
    private final int[] v;
    private final int start;
    private final int end;

    public MyThread(int[] v, int start, int end) {
        this.v = v;
        this.start = start;
        this.end = end;
    }

    @Override
    public void run() {
        for (int i = start; i < end; i++) {
            v[i] *= 2;
        }
    }
}