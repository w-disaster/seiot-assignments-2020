package com.dm.dam_mobile;

public enum ControlMode {
    MANUAL("Modalità Manuale", "#dbac00", 1),
    AUTOMATIC("Modalità Automatica","#009688", 0);

    private final String text;
    private final String color;
    private final int code;

    ControlMode(final String text, final String color, final int code){
        this.text = text;
        this.color = color;
        this.code = code;
    }

    public final String getColor() {
        return this.color;
    }

    public final int getCode() {
        return this.code;
    }

    @Override
    public final String toString() {
        return this.text;
    }

}

