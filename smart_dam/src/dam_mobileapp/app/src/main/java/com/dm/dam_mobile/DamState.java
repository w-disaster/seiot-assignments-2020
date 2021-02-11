package com.dm.dam_mobile;

public enum DamState {
    CONNECTING("Connessione in corso...", "#5c5c5c", 0),
    NORMAL("Normale","#00c400", 0),
    PRE_ALARM("Pre-Allarme", "#dbac00", 1),
    ALARM("Allarme","#e60000", 2);

    private final String text;
    private final String color;
    private final int alertLevel;

    DamState(final String text, final String color, final int alertLevel){
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
