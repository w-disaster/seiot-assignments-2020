package model;

import javafx.scene.paint.Color;
import javafx.scene.paint.Paint;

public enum State {
    
    NOT_CONNECTED("NO DEVICE CONNECTED", "000000"),
    CALIBRATING("CALIBRATING PIR...", "#808080"),
    READY("READY", "00FF00"),
    EXPERIMENTING("EXPERIMENTING", "#00bbff"),
    EXPERIMENT_CONLUDED("EXPERIMENT ENDED", "#ffc400"),
    ERROR("ERROR", "FF0000"),
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
