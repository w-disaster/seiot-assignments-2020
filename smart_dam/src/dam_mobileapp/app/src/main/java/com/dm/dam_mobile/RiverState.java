package com.dm.dam_mobile;

public enum RiverState {
    CONNECTING("Connecting...", "#5c5c5c", -1),
    NORMAL("Normal","#0080ff", 0),
    PRE_ALARM("Pre-Alarm", "#dbac00", 1),
    ALARM("Alarm","#e60000", 2);

    private final String text;
    private final String color;
    private final int alertLevel;

    RiverState(final String text, final String color, final int alertLevel){
        this.text = text;
        this.color = color;
        this.alertLevel = alertLevel;
    }

    public final String getColor() {
        return this.color;
    }

    public final int getAlertLevel() {
        return this.alertLevel;
    }

    @Override
    public final String toString() {
        return this.text;
    }
}
