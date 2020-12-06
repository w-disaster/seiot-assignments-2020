package model;

import view.Graph;
import view.GraphImpl;

public final class GraphFactory {

    private GraphFactory() { }

    public static Graph createDistanceGraph() {
        return createGraph(Measures.DISTANCE);
    }

    public static Graph createSpeedGraph() {
        return createGraph(Measures.SPEED);
    }

    public static Graph createAccelerationGraph() {
        return createGraph(Measures.ACCELERATION);
    }

    private static Graph createGraph(final Measures measure) {
        return new GraphImpl(measure.getName(), measure.getUnitOfMeasure(), measure.getGraphLineColor());
    }

}
