public class InitialData {
    private final int fragmentDimension;
    private final long position;

    public InitialData(final int fragmentDimension, final long position) {
        this.fragmentDimension = fragmentDimension;
        this.position = position;
    }

    /**
     * @return the fragmentDimension
     */
    public int getFragmentDimension() {
        return fragmentDimension;
    }

    /**
     * @return the position
     */
    public long getPosition() {
        return position;
    }
}
