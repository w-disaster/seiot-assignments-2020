package model;

import javafx.scene.paint.Color;
import javafx.scene.paint.Paint;

public enum State {

    /**
     * No device is connected.
     */
    NOT_CONNECTED("NO DEVICE CONNECTED", "000000"),

    /**
     * The peer is calibrating before being ready.
     */
    CALIBRATING("CALIBRATING PIR...", "#808080"),

    /**
     * The machine is ready.
     */
    READY("READY", "00FF00"),

    /**
     * There is an experiment in action.
     */
    EXPERIMENTING("EXPERIMENTING", "#00bbff"),

    /**
     * The experiment is over.
     */
    EXPERIMENT_CONLUDED("EXPERIMENT ENDED", "#ffc400"),

    /**
     * An error has occurred.
     */
    ERROR("ERROR", "FF0000"),

    /**
     * The machine is in power saving mode.
     */
    SUSPENDED("SUSPENDED",  "#808080");

    private final String labelText;
    private final Paint color;

    State(final String labelText, final String color) {
        this.labelText = labelText;
        this.color = Color.web(color);
    }

    public final String getLabelText() {
        return this.labelText;
    }

    public final Paint getColor() {
        return this.color;
    }
}
