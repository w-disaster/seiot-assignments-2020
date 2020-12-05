package view;

import javafx.scene.Node;
import javafx.scene.chart.LineChart;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.XYChart;
import javafx.scene.chart.XYChart.Series;

public class GraphImpl implements Graph {

    private static final double MAX_EXP_TIME = 22_000.0;

    private final LineChart<Number, Number> chart;

    private final Series<Number, Number> coordinates;

    private final String lineColor;

    public GraphImpl(final String yName, final String unitOfMeasure, final String color) {
        final NumberAxis xAxis = new NumberAxis();
        final NumberAxis yAxis = new NumberAxis();

        yAxis.setLabel(unitOfMeasure);
        xAxis.setLabel("millis");

        /* no zoom */
        xAxis.setAutoRanging(false);
        xAxis.setUpperBound(MAX_EXP_TIME);

        this.coordinates = new Series<>();
        this.coordinates.setName(yName);

        this.chart = new LineChart<>(xAxis, yAxis);
        this.chart.setTitle(yName);
        this.chart.getData().add(coordinates);

        this.lineColor = color;
    }

    @Override
    public final void updatePlot(final XYChart.Data<Number, Number> newCoordinates) {
        this.coordinates.getData().add(newCoordinates);
        for (final Node node : this.chart.lookupAll(".series" + 0)) {
            node.setStyle(" -fx-stroke: " + this.lineColor + ";\n"
                    + "     -fx-background-color: " + this.lineColor + ", white;\n"
                    + "     -fx-background-insets: 0, 2;\n"
                    + "     -fx-background-radius: 5px;\n"
                    + "     -fx-padding: 2px;");
        }

    }

    @Override
    public final void reset() {
        this.coordinates.getData().clear();
    }

    @Override
    public final LineChart<Number, Number> getChart() {
        return this.chart;
    }

}
