package application;

import javafx.application.Application;
import javafx.geometry.HPos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.GridPane;
import javafx.scene.text.Text;
import javafx.stage.Stage;
import jssc.SerialPortList;

import model.GraphFactory;
import model.State;
import controller.SerialController;
import view.Graph;

/**
 * This class represent the Main class of the JavaFX-based application.
 */
public final class Main extends Application {

    private static final String EXP_OVER_TXT = "Press OK to continue.";
    private static final int SCENE_WIDTH = 1500;
    private static final int SCENE_HEIGHT = 600;

    private SerialController serialController;

    @Override
    public void start(final Stage stage) throws Exception {

        final GridPane root = new GridPane();

        final Graph g1 = GraphFactory.createDistanceGraph();
        final Graph g2 = GraphFactory.createSpeedGraph();
        final Graph g3 = GraphFactory.createAccelerationGraph();

        final Label stateLabel = new Label();
        stateLabel.setStyle("-fx-font-size: 25pt;"
                + "-fx-font-weight:bold;");

        final Button okButton = new Button("OK");
        okButton.setDisable(true);
        okButton.setVisible(false);
        okButton.setStyle("-fx-font-size: 15pt;"
                + "-fx-font-weight:bold;"
                + "-fx-padding: 10pt 25pt;" 
                + "-fx-border-insets: 10pt;" 
                + "-fx-background-insets: 5pt;");

        final Text okButtonDescription = new Text(EXP_OVER_TXT);
        okButtonDescription.setVisible(false);
        okButtonDescription.setStyle("-fx-font-size: 10pt;"
                + "-fx-font-weight:bold;"
                + "-fx-padding: 10pt;" 
                + "-fx-border-insets: 10pt;" 
                + "-fx-background-insets: 5pt;");

        /* add the graphs */
        root.add(g1.getChart(), 0, 0);
        root.add(g2.getChart(), 1, 0);
        root.add(g3.getChart(), 2, 0);

        /* add the state label */
        root.add(stateLabel, 1, 1);
        GridPane.setHalignment(stateLabel, HPos.CENTER);

        /* add the ok for later */
        root.add(okButtonDescription, 1, 2);
        GridPane.setHalignment(okButton, HPos.CENTER);
        root.add(okButton, 1, 3);
        GridPane.setHalignment(okButtonDescription, HPos.CENTER);

        final Scene scene = new Scene(root, SCENE_WIDTH, SCENE_HEIGHT);

        try {
            final String[] ports = SerialPortList.getPortNames();
            serialController = new SerialController(ports[0], g1, g2, g3, stateLabel, okButton, okButtonDescription);
        } catch (ArrayIndexOutOfBoundsException e) {
            stateLabel.setText(State.NOT_CONNECTED.getLabelText());
        }

        stage.setScene(scene);
        stage.show();

        stage.setOnCloseRequest((event) -> {
                try {
                    if (serialController.isPortOpen()) {
                        serialController.close();
                    }
                } catch (NullPointerException e) {
                    System.out.println("Application closed without connecting any board");
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
