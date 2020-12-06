package view;

import javafx.scene.chart.LineChart;
import javafx.scene.chart.XYChart;

public interface Graph {

    LineChart<Number, Number> getChart();

    void updatePlot(XYChart.Data<Number, Number> newCoordinates);

    void reset();

}
