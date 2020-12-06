package controller;

import java.util.HashMap;
import java.util.Map;
import javafx.application.Platform;
import javafx.scene.chart.XYChart.Data;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.text.Text;
import jssc.SerialPort;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;
import jssc.SerialPortException;
import model.State;
import view.Graph;

public class SerialController implements SerialPortEventListener {

    private static final String SEPARATOR = ":";
    private static final String STATE_CHAR = "!";
    private static final String END_MSG = ";";
    private static final String END_EXP_OK = ">";

    private final SerialPort port;

    private final Label stateLabel;
    private final Button okButton;
    private final Text okButtonDescription;

    private final Graph distanceGraph;
    private final Graph speedGraph;
    private final Graph accelerationGraph;

    private String msg;
    private State state;

    public SerialController(final String portName, final Graph distanceGraph, final Graph speedGraph,
            final Graph accelerationGraph, final Label stateLabel, final Button okButton, final Text okButtonDescription) {
        /* msg set up */
        msg = "";

        /* set up the serial port */
        port = new SerialPort(portName);

        try {
            if (!port.isOpened()) {
                port.openPort();
            }

            port.setParams(SerialPort.BAUDRATE_115200, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);

            port.setFlowControlMode(SerialPort.FLOWCONTROL_RTSCTS_IN | SerialPort.FLOWCONTROL_RTSCTS_OUT);

            port.addEventListener(this, SerialPort.MASK_RXCHAR);

        } catch (SerialPortException e) {
            e.printStackTrace();
        }

        /* set up state */
        this.state = State.CALIBRATING;
        this.stateLabel = stateLabel;
        updateStateLabel();

        /* set up graph reference */
        this.distanceGraph = distanceGraph;
        this.speedGraph = speedGraph;
        this.accelerationGraph = accelerationGraph;

        /* set up okButton utility */
        this.okButton = okButton;
        this.okButtonDescription = okButtonDescription;

        this.okButton.setOnMouseClicked(event -> {
                if (port.isOpened()) {
                    sendMessage(END_EXP_OK);
                }

                this.okButtonDescription.setVisible(false);
                this.okButton.setDisable(true);
                this.okButton.setVisible(false);
            });
    }

    /**
     * Every event caused by a message sent it fetches the data, 
     * puts it in the right format and then processes the data to update UI. 
     */
    @Override
    public final synchronized void serialEvent(final SerialPortEvent event) {
        if (event.isRXCHAR() && event.getEventValue() > 0) {
            try {

                String recivedData = port.readString(event.getEventValue());
                recivedData = recivedData.replaceAll("\r", "");

                if (recivedData.equals(END_MSG)) {

                    /* data usage */
                    processData();

                } else if (recivedData.endsWith(END_MSG)) {

                    msg = msg.concat(recivedData.split(END_MSG)[0]);

                    /* data usage */
                    processData();

                } else if (recivedData.contains(END_MSG)) { 
                    final String oldData = recivedData.split(END_MSG)[0];
                    final String newData = recivedData.split(END_MSG)[1];

                    msg = msg.concat(oldData);

                    /* data usage */
                    processData();

                    /* ready next msg */
                    msg = newData;

                } else {
                    msg = msg.concat(recivedData);
                }

            } catch (SerialPortException e) {
                System.out.println("Error during SerialPort getting the data");
                e.printStackTrace();
            }
        }

    }

    public final synchronized void close() {
        try {
            if (port != null) {
                port.removeEventListener();
                port.closePort();
            }
        } catch (SerialPortException e) {
            System.out.println("Error during SerialPort closing");
            e.printStackTrace();
        }
    }

    /**
     * @return
     * Returns whether the port is open or not.
     */
    public final boolean isPortOpen() {
        return this.port.isOpened();
    }

    /**
     * Divides the messages from the serial port in two categories:
     * <b> data for the graphs </b> or <b> state for the label </b>
     * and call their methods to update the UI.
     */
    private void processData() {
        if (isState(msg)) {
            updateState(msg);
            updateStateLabel();
        } else {
            updatePlots(msg);
        }
        /* ready next msg */
        msg = "";
    }

    /**
     * @param string
     * The message to check
     * @return
     * True if it is a State updating message, False otherwise.
     */
    private boolean isState(final String string) {
        return string.endsWith(STATE_CHAR);
    }

    /**
     * @param data
     * The message containing the data to map.
     * @return
     * Returns a map with the graph name as key and the coordinates to add as value.
     */
    private Map<String, Data<Number, Number>> getCoordinates(final String data) {

        final Map<String, Data<Number, Number>> coordinates = new HashMap<>();
        final String[] dataArray = data.split(SEPARATOR);

        if (dataArray.length == 4) {
            final double t = Double.parseDouble(dataArray[0]) / 1000;
            final double s = Double.parseDouble(dataArray[1]);
            final double v = Double.parseDouble(dataArray[2]);
            final double a = Double.parseDouble(dataArray[3]);
            coordinates.put("distance", new Data<Number, Number>(t, s));
            coordinates.put("speed", new Data<Number, Number>(t, v));
            coordinates.put("acceleration", new Data<Number, Number>(t, a));

            return coordinates;
        } else {
            throw new IllegalArgumentException();
        }
    }

    /**
     * Updates all the plots without colliding with the JavaFx Thread.
     * @param data
     * The message containing the data.
     */
    private void updatePlots(final String data) {
        try {
            final Map<String, Data<Number, Number>> coordinates = getCoordinates(data);

            Platform.runLater(() -> {
                    distanceGraph.updatePlot(coordinates.get("distance"));
                    speedGraph.updatePlot(coordinates.get("speed"));
                    accelerationGraph.updatePlot(coordinates.get("acceleration"));
                });
        } catch (IllegalArgumentException e) {
            System.out.println(msg);
            System.out.println("Clean up input before starting");
        }
    }

    /**
     * Updates the Label with the current State without colliding with the JavaFx Thread.
     */
    private void updateStateLabel() {
        Platform.runLater(new Runnable() {
            @Override
            public void run() {
                stateLabel.setText(state.getLabelText());
                stateLabel.setTextFill(state.getColor());
            }
        });
    }

    /**
     * Sets the current state from the code recived.
     * @param recivedData
     * The message containing the State code.
     */
    private void updateState(final String recivedData) {
        switch (recivedData) {
        case "ready!":
            state = State.READY;
            break;
        case "suspended!":
            state = State.SUSPENDED;
            break;
        case "exp!":
            Platform.runLater(() -> {
                    /* clear any old graph before the experiment starts */
                    distanceGraph.reset();
                    speedGraph.reset();
                    accelerationGraph.reset();
                });

            state = State.EXPERIMENTING;
            break;
        case "over!":
            state = State.EXPERIMENT_CONLUDED;

            /* open the ok menù */
            Platform.runLater(() -> {
                okButtonDescription.setVisible(true);
                okButton.setVisible(true);
                okButton.setDisable(false);
            });

            break;
        case "error!":
            state = State.ERROR;
            Platform.runLater(() -> {
                okButtonDescription.setVisible(false);
                okButton.setVisible(false);
                okButton.setDisable(true);
            });
            break;
        default:
            state = State.NOT_CONNECTED;
            Platform.runLater(() -> {
                okButtonDescription.setVisible(false);
                okButton.setVisible(false);
                okButton.setDisable(true);
            });
            break;
        }
    }

    private void sendMessage(final String msg) {
        try {
            port.writeString(msg);
        } catch (SerialPortException e) {
            e.printStackTrace();
        }
    }

}
