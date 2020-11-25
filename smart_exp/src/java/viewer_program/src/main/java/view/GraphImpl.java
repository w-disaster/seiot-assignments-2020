package view;

import javafx.scene.chart.LineChart;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.XYChart;
import javafx.scene.chart.XYChart.Series;

public class GraphImpl implements Graph {

    private final LineChart<Number, Number> chart;

    private final Series<Number, Number> coordinates;

    public GraphImpl(final String xName, final String unitOfMeasure) {
        final NumberAxis xAxis = new NumberAxis();
        final NumberAxis yAxis = new NumberAxis();

        xAxis.setLabel(unitOfMeasure);
        yAxis.setLabel("s");

        this.coordinates = new Series<>();
        this.coordinates.setName(xName);

        this.chart = new LineChart<>(xAxis, yAxis);
        this.chart.setTitle(xName);
        this.chart.getData().add(coordinates);

    }

    @Override
    public final void updatePlot(final XYChart.Data<Number, Number> newCoordinates) {
        this.coordinates.getData().add(newCoordinates);
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
