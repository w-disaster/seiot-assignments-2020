package application;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.layout.FlowPane;
import javafx.stage.Stage;

import jssc.SerialPortList;

import model.GraphFactory;

import controller.SerialController;
import view.Graph;

/**
 * This class represent the Main class of the JavaFX-based application.
 */
public final class Main extends Application {

    private static final int SCENE_WIDTH = 1080;
    private static final int SCENE_HEIGHT = 720;

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

        final Label status = new Label();
        root.getChildren().addAll(g1.getChart(), g2.getChart(), g3.getChart(), status);

        final Scene scene = new Scene(root, SCENE_WIDTH, SCENE_HEIGHT);

        try {
            final String[] ports = SerialPortList.getPortNames();
            serialController = new SerialController(ports[0], g1, g2, g3, status);
            status.setText("READY");
        } catch (ArrayIndexOutOfBoundsException e) {
            System.out.println("No Serial Port connected\nRE-LAUNCH THE APPLICATION ONCE THE PORT IS CONNECTED");
            status.setText("NOT CONNECTED");
        }

        stage.setScene(scene);
        stage.show();
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
