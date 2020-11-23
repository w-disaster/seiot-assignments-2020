package controller;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javafx.scene.chart.XYChart.Data;
import jssc.*;

import view.Graph;

public class SerialController implements SerialPortEventListener {

    private static final String SEPARATOR = ":";

    private final SerialPort port;

    private final Graph positionGraph;
    private final Graph speedGraph;
    private final Graph accelerationGraph;

    public SerialController(final String portName, final Graph positionGraph, final Graph speedGraph,
            final Graph accelerationGraph) {
        /* set up the serial port */
        port = new SerialPort(portName);

        try {
            port.openPort();
        } catch (SerialPortException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        System.out.println(port.isOpened());

        /* set up graph reference */
        this.positionGraph = positionGraph;
        this.speedGraph = speedGraph;
        this.accelerationGraph = accelerationGraph;
    }

    @Override
    public synchronized void serialEvent(SerialPortEvent event) {
        if (event.isRXCHAR() && event.getEventValue() > 0) {
            try {
                final String recivedData = port.readString(event.getEventValue());
                final Map<String, Data<Number, Number>> coordinates = getCoordinates(recivedData);

                /* every event this updates the plots */
                positionGraph.updatePlot(coordinates.get("position"));
                speedGraph.updatePlot(coordinates.get("speed"));
                accelerationGraph.updatePlot(coordinates.get("acceleration"));

            } catch (SerialPortException e) {
                System.out.println("Error during SerialPort getting the data");
                e.printStackTrace();
            }
        }

    }

    private Map<String, Data<Number, Number>> getCoordinates(final String data) {
        final Map<String, Data<Number, Number>> coordinates = new HashMap<>();
        int i = 0;
        Number time = Integer.parseInt(data.split(SEPARATOR)[i]);

        for (String key : List.of("position", "speed", "acceleration")) {
            i++;
            coordinates.put(key, new Data<Number, Number>(time, Integer.parseInt(data.split(SEPARATOR)[i])));
        }

        return coordinates;
    }

}