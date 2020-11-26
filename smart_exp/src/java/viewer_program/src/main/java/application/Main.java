package application;

import javafx.application.Application;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.layout.FlowPane;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;
import jssc.SerialPortList;

import model.GraphFactory;
import model.State;
import controller.SerialController;
import view.Graph;

/**
 * This class represent the Main class of the JavaFX-based application.
 */
public final class Main extends Application {

    private static final int SCENE_WIDTH = 1500;
    private static final int SCENE_HEIGHT = 500;

    private SerialController serialController;

    private Graph g1;
    private Graph g2;
    private Graph g3;

    @Override
    public void start(final Stage stage) throws Exception {

        final FlowPane root = new FlowPane();

        this.g1 = GraphFactory.createPositionGraph();
        this.g2 = GraphFactory.createSpeedGraph();
        this.g3 = GraphFactory.createAccelerationGraph();

        final Label stateLabel = new Label();
        stateLabel.setStyle("-fx-font-size: 25pt;"
                + "-fx-font-weight:bold;");
        root.getChildren().addAll(g1.getChart(), g2.getChart(), g3.getChart(), stateLabel);

        final Scene scene = new Scene(root, SCENE_WIDTH, SCENE_HEIGHT);

        try {
            final String[] ports = SerialPortList.getPortNames();
            serialController = new SerialController(ports[0], g1, g2, g3, stateLabel);
        } catch (ArrayIndexOutOfBoundsException e) {
            stateLabel.setText(State.NOT_CONNECTED.getLabelText());
        }

        stage.setScene(scene);
        stage.show();

        stage.setOnCloseRequest(new EventHandler<WindowEvent>() {
            @Override
            public void handle(final WindowEvent event) {
                try {
                    if (serialController.isPortOpen()) {
                        serialController.close();
                    }
                } catch (NullPointerException e) {
                    System.out.println("Application closed without connecting any board");
                }
            }
        });
    }

    /**
     * 
     * @param args
     *                 unused
     */
    public static void main(final String[] args) {
        launch();
    }

}
