package com.dm.dam_mobile;

public enum ControlMode {
    MANUAL("Manual", "#5c5c5c", 0),
    AUTOMATIC("Automatic","#00c400", 1);

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

