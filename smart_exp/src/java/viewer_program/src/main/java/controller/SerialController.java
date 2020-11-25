package controller;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javafx.scene.chart.XYChart.Data;
import javafx.scene.control.Label;
import jssc.*;

import view.Graph;

public class SerialController implements SerialPortEventListener {

    private static final String SEPARATOR = ":";

    private final SerialPort port;

    private final Label status;
    private final Graph positionGraph;
    private final Graph speedGraph;
    private final Graph accelerationGraph;

    public SerialController(final String portName, final Graph positionGraph, final Graph speedGraph,
            final Graph accelerationGraph, final Label status) {
        /* set up the serial port */
        port = new SerialPort(portName);

        try {
            if (!port.isOpened()) {
                port.openPort();
            }

            port.setParams(SerialPort.BAUDRATE_9600, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
        } catch (SerialPortException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        /* set up status */
        this.status = status;

        /* set up graph reference */
        this.positionGraph = positionGraph;
        this.speedGraph = speedGraph;
        this.accelerationGraph = accelerationGraph;
    }

    @Override
    public final synchronized void serialEvent(final SerialPortEvent event) {
        if (event.isRXCHAR() && event.getEventValue() > 0) {
            try {
                final String recivedData = port.readString(event.getEventValue());
                if (startsWithNumbers(recivedData)) {
                    updatePlots(getCoordinates(recivedData));
                } else {
                    status.setText(recivedData);
                }
            } catch (SerialPortException e) {
                System.out.println("Error during SerialPort getting the data");
                e.printStackTrace();
            }
        }

    }

    private Map<String, Data<Number, Number>> getCoordinates(final String data) {
        final Map<String, Data<Number, Number>> coordinates = new HashMap<>();
        int i = 0;
        final Number time = Integer.parseInt(data.split(SEPARATOR)[i]);

        for (final String key : List.of("position", "speed", "acceleration")) {
            i++;
            coordinates.put(key, new Data<Number, Number>(time, Integer.parseInt(data.split(SEPARATOR)[i])));
        }

        return coordinates;
    }

    private void updatePlots(final Map<String, Data<Number, Number>> coordinates) {
        positionGraph.updatePlot(coordinates.get("position"));
        speedGraph.updatePlot(coordinates.get("speed"));
        accelerationGraph.updatePlot(coordinates.get("acceleration"));
    }

    private boolean startsWithNumbers(final String recivedData) {
        for (int i = 0; i < 10; i++) {
            if (recivedData.startsWith(String.valueOf(i))) {
                return true;
            }
        }
        return false;
    }

}
